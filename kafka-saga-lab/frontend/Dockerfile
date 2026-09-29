# 학습: 빌드 도구와 실행 환경의 분리
# Node 단계에서 번들만 만들고 Nginx 이미지에는 정적 산출물을 복사한다. npm ci는 lockfile 계약을 사용한다.
FROM node:22-alpine AS build
WORKDIR /app
COPY package*.json ./
RUN npm ci
COPY . .
RUN npm run build

FROM nginx:stable-alpine
COPY nginx.conf /etc/nginx/templates/default.conf.template
COPY docker-start.sh /usr/local/bin/start-frontend
RUN chmod +x /usr/local/bin/start-frontend
COPY --from=build /app/dist /usr/share/nginx/html
EXPOSE 80
ENTRYPOINT ["/usr/local/bin/start-frontend"]
