name2 = [
    "김민경", "송우정", "홍현욱", "오민석", "주현민", "최준혁", "장원혁", "이재준", "이경은", "박상원", 
    "구도영", "하주은", "연수인", "장시현", "김성아", "박지성", "정경욱", "신규환", "손동진", "이선주", 
    "박상만", "김민진", "최교림", "백성빈", "조예나", "권기태", "윤진", "엄주현", "이효창", "정희경", 
    "윤지환", "조성진", "서용준", "진아영", "이지우", "양병현", "고라희", "우승엽", "이상묵", "신연성", 
    "배혜진", "여다겸", "이동현", "이송희", "박예인", "홍예원", "허유정", "오채홍", "김도현", "장효진", 
    "김승희", "임선민", "표준성", "이재원", "송준우", "이예진", "이준민", "박새연", "김현욱", "조승준", 
    "김다빈", "강현성", "정예원", "김지현", "신윤수", "정우성", "우성훈", "정유경", "유현서", "구태오", 
    "박정훈", "김유진"
]


name1 = [
    "홍현욱", "주현민", "송우정", "오민석", "장원혁", "이재준", "이경은", "박상원", "연수인", "구도영",
    "하주은", "장시현", "정경욱", "설소연", "손동진", "박상만", "신규환", "김민진", "박지성", "최교림",
    "조예나", "백성빈", "윤지환", "권기태", "장시현", "설소연", "정경욱", "손동진", "윤진", "윤진", "이선주",
    "엄주현", "윤지환", "이효창", "정희경", "백성빈", "박지성", "조성진", "김성아", "서용준", "진아영",
    "이지우", "양병현", "김민경", "신연성", "우승엽", "배혜진", "고라희", "여다겸", "이상묵", "허유정",
    "이동현", "이송희", "박예인", "MT기획단 환불_권기태", "조예나", "홍예원", "허유정", "오채홍", "김도현",
    "심규원", "오나경", "장효진", "김승희", "임선민", "표준성", "최준혁", "이재원", "송준우", "이예진",
    "이준민", "프렌즈 체크카드 캐시백", "이태연", "박새연", "김현욱", "김다빈", "강현성", "정예원",
    "김지현", "MT환불_심규원", "MT환불_오나경", "정우성", "정우성_착오송금", "신윤수", "박정호",
    "착오송금 반환_박정호", "김은교", "정우성", "우성훈", "신규환", "최교림", "조승준", "정유경",
    "입출금통장 이자", "설소연", "쿠팡(쿠페이)", "쿠팡(쿠페이)", "쿠팡(쿠페이)", "김은교", "구태오",
    "유현서"
]

# 이름 1에는 있지만 이름 2에는 없는 이름을 찾는 함수
def find_missing_names(name1, name2):
    missing_names = []
    for name in name1:
        if name not in name2:
            missing_names.append(name)
    return missing_names

missing_names = find_missing_names(name1, name2)
print("이름 1에는 있지만 이름 2에는 없는 이름:")
print(missing_names)
