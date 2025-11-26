from time import sleep
import requests

Ewha_school_id = 425


# 티어 계산기
def whatIsTier(num):
  t_color = (num - 1) // 5

  if t_color == 0 : t_color = "🥉 "
  elif t_color == 1 : t_color = "🥈 "
  elif t_color == 2 : t_color = "🥇 "
  elif t_color == 3 : t_color = "💠 "
  elif t_color == 4 : t_color = "💎 "
  elif t_color == 5 : t_color = "❤️‍🔥 "
  elif t_color == 6 : t_color = "👑 "
  else : t_color = "🆇 "

  t_number = 5 - ((num - 1) % 5)

  return t_color + str(t_number)



# 학교 사용자들을 크롤링하는 함수
def crawl_school():
  school_id = 425

  for page in range(1, 200):
    sleep(1.0) # API 요청 사이의 딜레이 주기
    path = f"https://solved.ac/api/v3/ranking/in_organization?organizationId={school_id}&page={page}"
    print('\n' + path + '\n') # 현재 크롤링 중인 페이지 URL 출력
    response = requests.get(path)
    if response.status_code != 200:
      print(response.status_code)
      break

    data = response.json()
    if len(data['items']) == 0:
      break

    for user in data['items']:
      username = user['handle']
      tier = user['tier']  # 사용자의 티어 정보
      real_tier = whatIsTier(tier)
      solved_count = user['solvedCount']  # 사용자가 푼 문제 수

      #사용자 이름(핸들), 티어, 푼 문제 수 
      print(f"{real_tier} \t {username} \t {solved_count}개 solved")

crawl_school()