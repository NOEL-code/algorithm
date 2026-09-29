# 학습: Java 멀티 스테이지 빌드
# Maven/JDK 빌드 단계와 JRE 실행 단계를 분리한다. SERVICE 인자로 같은 빌드에서 서비스별 산출물을 선택한다.
FROM maven:3.9.11-eclipse-temurin-21 AS build
WORKDIR /workspace
COPY . .
RUN mvn -B -DskipTests package

FROM eclipse-temurin:21-jre
ARG SERVICE
WORKDIR /app
COPY --from=build /workspace/${SERVICE}/target/${SERVICE}-1.0.0-exec.jar app.jar
ENTRYPOINT ["java", "-jar", "app.jar"]
