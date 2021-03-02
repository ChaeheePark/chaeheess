f=open("yesterday.txt",'r') #파일오픈
yesterday_lyric=f.readlines() #한 줄씩 list형태로 저장
f.close() #파일닫기

contents="" #문자열을 담을수 있는 변수 지정
for line in yesterday_lyric: #한줄씩 line 변수에 넣어 읽기
    contents=contents+line.strip()+"\n" #contents 변수안에 문자열로 한줄씩 모든 가사 넣음

n_of_yesterday=contents.upper().count("YESTERDAY") #contents 변수안의 값을 모두 대문자로 바꿔 "YESTERDAY"가 몇개인지 찾음
print("Number of a Word 'Yesterday'",n_of_yesterday)
