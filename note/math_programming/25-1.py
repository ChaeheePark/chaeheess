import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle, Arrow
a = np.array([1,2])
b = np.array([3,6])

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
# a와 b에 의해 만들어지는 생성(span)을 그림.
for c1 in np.linspace(-1.5,1.5,25):
    for c2 in np.linspace(-1.5,1.5,25):
        c1_a=a*c1
        c2_b=b*c2
        sum=c1_a+c2_b
        plt.scatter(sum[0],sum[1],color="blue",zorder=1,s=1)

ax.quiver(0,0,a[0],a[1],angles='xy',scale_units='xy',scale=1,color="black",zorder=2)
ax.text(a[0]+0.5,a[1],'a',size=15,color="black",zorder=2)

ax.quiver(0,0,b[0],b[1],angles='xy',scale_units='xy',scale=1,color="black",zorder=2)
ax.text(b[0],b[1]+0.5,'b',size=15,color="black",zorder=2)
ax.axis([-7,7,-7,7])
ax.set_xticks(range(-7,7))
ax.set_yticks(range(-7,7))
ax.grid()
ax.set_axisbelow(True)
ax.set_aspect('auto', adjustable='box')
ax.spines['left'].set_position('zero')
ax.spines['bottom'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
plt.show()
