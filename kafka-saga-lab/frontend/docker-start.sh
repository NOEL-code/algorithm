#!/bin/sh
# 학습: 환경 의존 주소의 런타임 주입
# Docker와 Kubernetes의 DNS 주소가 달라 resolv.conf에서 읽고 Nginx 템플릿에 주입한다. exec로 종료 신호를 전달한다.
set -eu
export DNS_RESOLVER="${DNS_RESOLVER:-$(awk '/^nameserver / { print $2; exit }' /etc/resolv.conf)}"
exec /docker-entrypoint.sh nginx -g 'daemon off;'
