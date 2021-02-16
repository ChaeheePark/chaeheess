import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
a = np.array([1,5,0])
b = np.array([4,4,0])
c = np.array([4,7,0])

a = a*0.75
b = b*0.81
fig = plt.figure()
ax = fig.add_subplot(1,1,1,projection='3d')
# 3차원 공간 내에 같은 평면에 있는 3개의 벡터를 그린다.
ax.quiver(0,0,0,a[0],a[1],a[2],color='black',arrow_length_ratio=0.1)
ax.text(a[0],a[1],a[2],'a',size=15)
# 벡터 b를 그린다.
ax.quiver(a[0],a[1],a[2],b[0],b[1],b[2],color='black',arrow_length_ratio=0.1)
ax.text(a[0]+b[0],a[1]+b[1],a[2]+b[2],'b',size=15)
# 벡터 a와 b의 외적을 그린다.
ax.quiver(0,0,0,c[0],c[1],c[2],color='blue',arrow_length_ratio=0.1)
ax.text(c[0]+1,c[1],c[2],'c',size=15)
ax.set_xlim(0,8)
ax.set_ylim(0,8)
ax.set_zlim(0,8)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.view_init(elev=20, azim=5)
ax.grid()
ax.set_axisbelow(True)
ax.set_aspect('auto', adjustable='box')
plt.show()
