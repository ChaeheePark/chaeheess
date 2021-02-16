import numpy as np
import matplotlib.pyplot as plt

a=np.array([4,3])
#Figure객체를 만든 후
fig=plt.figure()
#1x1개의 서브 플롯을 추가, 첫 번째 서브플롯을 사용
ax=fig.add_subplot(1,1,1)

#변수 a의 값을 좌표로 사용하여 점을 그린다.
#a[0]이 x좌표, a[1]이 y좌표, s는 점의 크기
ax.scatter(a[0],a[1],s=30)
#점 옆에 문자 a를 표시, size는 문자의 크기
ax.text(a[0]+0.2,a[1]+0.2,'a',size=15)
#x축, y축 눈금 범위를 -5~5 로 지정
ax.set_xticks(range(-5,6))
ax.set_yticks(range(-5,6))
#그리드를 좌표축 아래에 표시
ax.grid()
ax.set_axisbelow(True)
#x축과 y축 눈금의 비율을 1:1로 함
#창의 크기를 변경해도 비율이 유지됨
ax.set_aspect('equal',adjustable='box')
#다음 코드를 사용하여 원점에 좌표축이 보이게함
#왼쪽 y축과 아래 x축을 (0,0)으로 이동
ax.spines['left'].set_position('zero')
ax.spines['bottom'].set_position('zero')
#위쪽과 오른쪽 축을 제거
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
plt.show()
