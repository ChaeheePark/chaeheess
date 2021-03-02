import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle, Arrow
a = np.array([2,4])
b = np.array([5,2])

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.add_patch(Rectangle((0,0),6,6,alpha=1,facecolor='blue',zorder=1))
# 벡터 a를 그린다.
ax.quiver(0,0,a[0],a[1],angles='xy',scale_units='xy',scale=1,zorder=2,color='white')
ax.text(a[0],a[1],'a',size=15,color='white')
# 벡터 b를 그린다.
ax.quiver(0,0,b[0],b[1],angles='xy',scale_units='xy',scale=1,zorder=2,color='white')
ax.text(b[0],b[1],'b',size=15,color='white')
ax.quiver(0,0,a[0]+b[0],a[1]+b[1],angles='xy',scale_units='xy',scale=1,zorder=2,color='red')
ax.text(a[0]+b[0],a[1]+b[1],'a+b',size=15,color='red')

ax.set_xticks(range(-1,10))
ax.set_yticks(range(-1,10))
ax.grid()
ax.set_axisbelow(True)
ax.set_aspect('auto', adjustable='box')
ax.spines['left'].set_position('zero')
ax.spines['bottom'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
plt.show()
