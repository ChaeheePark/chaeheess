import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle, Arrow
a = np.array([4,1])
b = np.array([-8,-2])

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
# a와 b에 의해 만들어지는 생성(span)을 그림.
line_x = []
line_y = []

for c1 in np.linspace(-0.9,0.9,25):
    for c2 in np.linspace(-0.9,0.9,25):
        c1_a1=a*c1
        c2_a2=b*c2
        sum=c1_a1+c2_a2
        line_x.append(sum[0])
        line_y.append(sum[1])

ax.plot(line_x,line_y,color='blue',zorder=0)

ax.quiver(0,0,a[0],a[1],angles='xy',scale_units='xy',scale=1,zorder=2)
ax.text(a[0],a[1],'a',size=15)
# 벡터 b를 그린다.
ax.quiver(0,0,b[0],b[1],angles='xy',scale_units='xy',scale=1,zorder=2)
ax.text(b[0],b[1]+0.5,'b',size=15)
ax.quiver(0,0,a[0]+b[0],a[1]+b[1],angles='xy',scale_units='xy',scale=1,zorder=2,color='red')
ax.text(a[0]+b[0],a[1]+b[1]+1,'a+b',size=15,color='red')

ax.set_xticks(range(-11,12))
ax.set_yticks(range(-5,5))
ax.grid()
ax.set_axisbelow(True)
ax.set_aspect('auto', adjustable='box')
ax.spines['left'].set_position('zero')
ax.spines['bottom'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
plt.show()
