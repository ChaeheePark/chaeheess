import numpy as np
import matplotlib.pyplot as plt

i=np.array([1,0])
j=np.array([0,1])

#단위 벡터로 벡터[4,3]을 나타낼 수 있다.
a=4*i+3*j

fig=plt.figure()
ax=fig.add_subplot(1,1,1)

ax.quiver(0,0,a[0],a[1],angles='xy',scale_units='xy',scale=1)
ax.text(a[0],a[1],'4i+3j',size=15)

ax.set_xticks(range(0,7))
ax.set_yticks(range(0,7))
ax.grid()
ax.set_axisbelow(True)
ax.set_aspect('equal',adjustable='box')

ax.spines['left'].set_position('zero')
ax.spines['bottom'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
plt.show()
