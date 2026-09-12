# Kubernetes에서 Saga 실습하기

Compose와 같은 Kafka 1대 + Spring 앱 3개를 `kafka-saga-lab` 네임스페이스에 배포합니다. 로컬 학습용이며, 기존 Compose 설정과 Java 코드를 그대로 사용합니다. 두 환경의 볼륨은 별개이므로 데이터가 자동으로 이전되지 않습니다.

## 구성

| 리소스 | 역할 |
|---|---|
| `kustomization.yaml` | 전체 리소스 묶음, 앱 이미지 태그, 공통 환경 변수 |
| `namespace.yaml` | 실습 전용 네임스페이스 |
| `kafka.yaml` | 단일 KRaft StatefulSet, 내부 Service, 고정 DNS용 headless Service, 2Gi PVC |
| `*-service.yaml` | 앱별 Deployment, ClusterIP Service, H2 파일용 1Gi PVC |

Kafka는 클러스터 내부 주소 `kafka:19092`를 광고합니다. `kafka-0.kafka-headless`는 컨트롤러의 고정 주소입니다. 새 Kafka 볼륨의 소유권은 init container가 UID/GID 1000으로 맞추며 브로커는 해당 일반 계정으로 실행합니다.

앱은 init container에서 Kafka의 토픽 조회가 성공할 때까지 기다립니다. 기동 이후의 Kafka 장애는 기존 Outbox 재시도로 복구합니다. 앱의 startup/readiness/liveness probe는 TCP 포트를 검사하므로 DB 상태나 Saga 처리 완료까지 보장하지 않습니다. 배포 후 아래 API 실습으로 메시지 흐름을 확인하세요. Kafka readiness는 실제 토픽 조회를 수행합니다. [Kubernetes probe 동작](https://kubernetes.io/docs/tasks/configure-pod-container/configure-liveness-readiness-startup-probes/)

**앱의 replicas는 1로 유지하세요.** 파일 H2 DB 공유와 Outbox 동시 실행을 피하기 위해 `Recreate`를 사용합니다. 업데이트 중에는 잠시 서비스가 중단됩니다. Kafka도 단일 브로커 설정이므로 replicas만 늘려 확장할 수 없습니다.

## 1. 로컬 클러스터 준비

Docker Engine/Desktop, `kubectl`, `kind`를 준비합니다. macOS에서 kind가 없다면 `brew install kind`로 설치할 수 있습니다. Docker Desktop을 먼저 실행하세요. Docker에 CPU 4개, 메모리 6GiB 이상을 할당하는 것으로 시작하고, 이미지 빌드나 실행 중 부족하면 늘리세요. [kind 설치 및 이미지 로드 가이드](https://kind.sigs.k8s.io/docs/user/quick-start/)

아래 명령은 모두 `kafka-saga-lab` 디렉터리에서 실행합니다.

```bash
cd kafka-saga-lab
docker info
kind create cluster --name kafka-saga-lab --wait 120s
kubectl config use-context kind-kafka-saga-lab
kubectl get nodes
kubectl get storageclass
```

이미 같은 이름의 kind 클러스터가 있다면 생성 명령을 생략합니다. PVC는 클러스터의 **기본 StorageClass**를 사용합니다. kind는 기본 스토리지 프로비저너를 제공합니다. 다른 클러스터라면 기본 StorageClass가 있는지 확인하거나 각 PVC 및 Kafka `volumeClaimTemplates`에 적절한 `storageClassName`을 지정하세요.

## 2. 이미지 빌드 및 로드

기존 Dockerfile의 `SERVICE` 인자를 사용합니다. Java와 Maven은 빌드 이미지에 포함되어 있어 호스트에 설치할 필요가 없습니다.

```bash
for service in order-service payment-service inventory-service; do
  docker build --build-arg SERVICE="$service" \
    -t "kafka-saga-lab/$service:local" . || break
done

kind load docker-image --name kafka-saga-lab \
  kafka-saga-lab/order-service:local \
  kafka-saga-lab/payment-service:local \
  kafka-saga-lab/inventory-service:local
```

세 이미지 빌드가 모두 성공한 뒤 로드하세요. `imagePullPolicy: IfNotPresent`를 사용하므로 kind 노드에 로드한 이미지를 사용할 수 있습니다. Kafka 이미지는 노드에서 내려받습니다.

다른 클러스터에서는 해당 노드가 이미지를 가져올 수 있도록 레지스트리에 push하고, `kustomization.yaml`의 `images`에 `newName`과 `newTag`를 설정하세요. 비공개 레지스트리는 별도 `imagePullSecrets`가 필요합니다.

## 3. 검증 및 배포

`kubectl`에 내장된 Kustomize로 적용합니다. [Kustomize 공식 문서](https://kubernetes.io/docs/tasks/manage-kubernetes-objects/kustomization/)

```bash
# 클러스터 연결 없이 최종 YAML 확인
kubectl kustomize k8s

# 적용 대상이 로컬 실습 클러스터인지 확인
kubectl config current-context
kubectl apply -f k8s/namespace.yaml
kubectl apply --dry-run=server -k k8s
kubectl apply -k k8s

kubectl -n kafka-saga-lab rollout status statefulset/kafka --timeout=300s
kubectl -n kafka-saga-lab rollout status deployment/order-service --timeout=300s
kubectl -n kafka-saga-lab rollout status deployment/payment-service --timeout=300s
kubectl -n kafka-saga-lab rollout status deployment/inventory-service --timeout=300s
kubectl -n kafka-saga-lab get pods,svc,pvc
```

네임스페이스를 먼저 생성하는 것은 최초 server dry-run에서 네임스페이스 미존재 오류를 피하기 위해서입니다. 앱의 토픽 생성 코드는 기존대로 실행됩니다. 정상 상태는 Pod 4개가 `Running`/`1/1`, PVC 4개가 `Bound`입니다. 최초 이미지 다운로드가 오래 걸려 rollout 명령이 시간 초과되면 아래 문제 해결 절차로 확인하세요.

## 4. API와 Kafka 확인

Compose나 로컬 Java 앱이 8080~8082를 사용하고 있다면 먼저 종료하세요. 터미널 3개에서 각각 실행하고 유지합니다.

```bash
kubectl -n kafka-saga-lab port-forward service/order-service 8080:8080
```

```bash
kubectl -n kafka-saga-lab port-forward service/payment-service 8081:8081
```

```bash
kubectl -n kafka-saga-lab port-forward service/inventory-service 8082:8082
```

이제 [기존 README의 세 가지 curl 시나리오](../README.md#2-curl로-세-가지-흐름-실습)를 그대로 실행할 수 있습니다. 정상 주문은 `COMPLETED`, 결제 거절은 `CANCELLED`/`PAYMENT_REJECTED`, 재고 부족은 `CANCELLED`/`OUT_OF_STOCK`과 결제 `REFUNDED`를 확인합니다.

```bash
kubectl -n kafka-saga-lab logs -f deployment/order-service -c app

kubectl -n kafka-saga-lab exec kafka-0 -- /opt/kafka/bin/kafka-topics.sh \
  --bootstrap-server kafka:19092 --list

kubectl -n kafka-saga-lab exec -it kafka-0 -- /opt/kafka/bin/kafka-console-consumer.sh \
  --bootstrap-server kafka:19092 --topic saga.replies --from-beginning \
  --property print.key=true
```

Kafka는 내부 DNS를 광고하므로 호스트에서 19092 포트만 포워딩하는 방식으로는 정상 접속할 수 없습니다. 위처럼 Pod 안에서 Kafka CLI를 실행하세요.

## 5. 재시작 및 장애 실습

결제 서비스를 멈춘 동안 새 주문은 `PAYMENT_PENDING`에 머물고, 다시 켜면 처리가 재개됩니다.

```bash
kubectl -n kafka-saga-lab scale deployment/payment-service --replicas=0
kubectl -n kafka-saga-lab wait --for=delete pod -l app=payment-service --timeout=120s
# POST /orders 후 주문 상태 확인
kubectl -n kafka-saga-lab scale deployment/payment-service --replicas=1
kubectl -n kafka-saga-lab rollout status deployment/payment-service --timeout=300s
```

Pod가 교체되면 해당 서비스의 port-forward도 다시 실행하세요. PVC에 저장된 H2 파일과 Kafka 로그는 Pod 재생성 후에도 유지됩니다. 기존 주문을 조회하고 재고가 10으로 초기화되지 않았는지 확인할 수 있습니다.

```bash
kubectl -n kafka-saga-lab rollout restart deployment/inventory-service
kubectl -n kafka-saga-lab rollout status deployment/inventory-service --timeout=300s
```

코드를 수정한 뒤에는 새 태그(예: `v2`)로 이미지를 빌드하고 kind에 로드한 다음, `kustomization.yaml`의 해당 `newTag`를 바꾸고 `kubectl apply -k k8s`를 실행하세요. 새 태그를 사용하면 이미지 변경으로 Pod가 교체됩니다. 환경 변수 변경도 생성된 ConfigMap 이름의 해시가 바뀌어 앱 재배포를 유발합니다.

## 6. 문제 해결

```bash
kubectl -n kafka-saga-lab get events --sort-by=.metadata.creationTimestamp
kubectl -n kafka-saga-lab describe pod kafka-0
kubectl -n kafka-saga-lab logs kafka-0 -c kafka
kubectl -n kafka-saga-lab logs deployment/order-service -c wait-for-kafka
kubectl -n kafka-saga-lab logs deployment/order-service -c app --previous
kubectl -n kafka-saga-lab get pvc
```

| 증상 | 확인할 항목 |
|---|---|
| 연결 거절 / 컨텍스트 없음 | Docker 실행, kind 클러스터 존재, `kubectl config current-context` |
| `ImagePullBackOff` / `ErrImagePull` | 앱의 태그와 kind 로드 대상 클러스터, Kafka 이미지 다운로드 가능 여부 |
| PVC `Pending` | 기본 StorageClass와 프로비저너, 디스크 공간 |
| Pod `Pending` | 이벤트의 CPU/메모리 부족 또는 볼륨 바인딩 오류 |
| `Init:0/1` | Kafka Pod 로그, PVC 권한 초기화, Kafka Service와 DNS |
| `CrashLoopBackOff` / `OOMKilled` | `--previous` 로그, 메모리 제한 및 Docker 할당량 |
| Pod Ready인데 주문이 진행되지 않음 | 앱 로그, Kafka/DLT, 결제·재고 서비스 상태; TCP probe는 Saga 완료를 검사하지 않음 |

## 7. 정지와 데이터 삭제

데이터를 유지하며 실행만 멈추려면 앱을 먼저 종료한 뒤 Kafka를 종료합니다.

```bash
kubectl -n kafka-saga-lab scale deployment --all --replicas=0
kubectl -n kafka-saga-lab wait --for=delete pod -l 'app in (order-service,payment-service,inventory-service)' --timeout=120s
kubectl -n kafka-saga-lab scale statefulset/kafka --replicas=0
```

재개는 `kubectl apply -k k8s` 후 위 rollout 확인 명령을 실행합니다.

**아래 명령은 네임스페이스와 모든 PVC를 삭제하여 주문·결제·재고·Kafka 데이터를 함께 초기화합니다.** PV의 실제 디스크 삭제 여부는 StorageClass의 reclaim policy에 따릅니다.

```bash
kubectl delete namespace kafka-saga-lab --wait=true
```

클러스터 자체가 필요 없으면 `kind delete cluster --name kafka-saga-lab`으로 제거할 수 있습니다. kind 노드 안의 로컬 볼륨 데이터도 잃게 됩니다. `kubectl delete -k k8s` 역시 네임스페이스와 PVC를 포함하므로 단순 정지 용도로 사용하지 마세요. StatefulSet만 지울 때의 볼륨 보존과 네임스페이스 전체 삭제는 다릅니다. [StatefulSet 볼륨 보존](https://kubernetes.io/docs/concepts/workloads/controllers/statefulset/)

## 학습 범위

단일 Kafka, 파일 H2, 내부 PLAINTEXT 통신을 사용하는 로컬 실습 구성입니다. 운영 확장에는 외부 DB, Outbox 동시 실행 제어, Kafka 고가용성·인증·암호화, 백업과 모니터링을 별도로 설계해야 합니다. 기존 애플리케이션의 테스트는 `./mvnw verify`로 실행하며, Kubernetes Pod 실행과 스토리지 프로비저닝은 실제 클러스터에서 별도로 확인해야 합니다.
