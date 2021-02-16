import numpy as np
import matplotlib.pyplot as plt

#벡터 A와 B를 생성합니다.
A=np.array([1,-2])
B=np.array([4,3])

#1개의 행과 2개의 열을 가지는 서브플롯을 생성
f,ax=plt.subplots(1,2)
ax[0].title.set_text('A')
ax[1].title.set_text('B')

#각 서브플롯에 시작하여 (0,0)인 벡터를 그린다.
ax[0].quiver(0,0,A[0],A[1],angles='xy',scale_units='xy',scale=1)
ax[1].quiver(0,0,B[0],B[1],angles='xy',scale_units='xy',scale=1)

#서브플롯별로 그리드를 생성
start_x=-1
end_x=5
start_y=-3
end_y=4
for i in range(2):
    ax[i].axis([start_x,end_x,start_y,end_y])
    ax[i].set_xticks(range(start_x,end_x))
    ax[i].set_yticks(range(start_y,end_y))
    ax[i].grid(True)
    ax[i].set_aspect('equal',adjustable='box')
plt.show()
