const express = require('express');
const request = require('request');
const cheerio = require('cheerio');

const app = express();
const PORT = 3000;

// 대상 원본 서버 (여기서 HTML을 받아옴)
const TARGET = 'https://example.com';

// 무작위 더미 요소 생성 함수
function injectDummyElements($) {
  const dummyScripts = [
    '<script>console.log("dummy script A")</script>',
    '<div style="display:none;"><img src="https://placekitten.com/1/1" /></div>',
    '<script>var _ = Math.random();</script>'
  ];
  const rand = Math.floor(Math.random() * dummyScripts.length);
  $('body').append(dummyScripts[rand]); // <body> 태그 끝에 삽입
}

// 리소스 로딩 순서 무작위화
function shuffleHeadResources($) {
  const elements = $('head script, head link, head style').toArray();
  const shuffled = elements.sort(() => 0.5 - Math.random());
  $('head').empty(); // 기존 제거
  shuffled.forEach(el => $('head').append(el)); // 무작위 순서로 재삽입
}

// 응답 지연 함수
function delay(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

app.get('/', async (req, res) => {
  request(URL, async (error, response, body) => {
    if (!error && response.statusCode === 200) {
      const $ = cheerio.load(body);

      // 정규화 기법 적용
      injectDummyElements($);
      shuffleHeadResources($);

      // 응답 지연: 100~500ms 사이 랜덤
      const randomDelay = 100 + Math.random() * 400;
      await delay(randomDelay);

      res.send($.html());
    } else {
      res.status(500).send('Failed to fetch target site');
    }
  });
});

app.listen(PORT, () => {
  console.log(`Proxy server running on http://localhost:${PORT}`);
});