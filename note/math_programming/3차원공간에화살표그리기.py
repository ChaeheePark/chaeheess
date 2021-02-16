import numpy as np
import matplotlib.pyplot as plt

#1차원벡터
A=np.array([4,5,6])
fig=plt.figure()

#서브플롯에서 3차원 좌표계를 사용
ax=fig.add_subplot(1,1,1,projection='3d')

#시작점이 (0,0,0)이고 끝점 좌표가 변수 a의 값인 화살표 그리기
ax.quiver(0,0,0,A[0],A[1],A[2],color='black',arrow_length_ratio=0.1)
ax.text(A[0],A[1],A[2],'a',size=15)

#그리드 생성
ax.set_xlim(0,8)
ax.set_ylim(0,8)
ax.set_zlim(0,8)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.view_init(elev=20,azim=5)
ax.grid()
ax.set_axisbelow(True)
ax.set_aspect('auto',adjustable='box')
plt.show()
