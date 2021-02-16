import numpy as np
import matplotlib.pyplot as plt
a = np.array([2,1])
b = np.array([-3,2])
c = np.array([-6,4])
fig = plt.figure()
ax = fig.add_subplot(1,1,1)
# 벡터 a를 그린다.
ax.quiver(0,0,a[0],a[1],angles='xy',scale_units='xy',scale=1)
ax.text(a[0],a[1],'a',size=15)
# 벡터 b를 그린다.
ax.quiver(0,0,b[0],b[1],angles='xy',scale_units='xy',scale=1,zorder=2)
ax.text(b[0],b[1],'b',size=15)
ax.quiver(0,0,c[0],c[1],angles='xy',scale_units='xy',scale=1,zorder=1,color='blue')
ax.text(c[0],c[1],'c',size=15)

ax.set_xticks(range(-8,4))
ax.set_yticks(range(-2,6))
ax.grid()
ax.set_axisbelow(True)
ax.set_aspect('auto', adjustable='box')
ax.spines['left'].set_position('zero')
ax.spines['bottom'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
plt.show()
