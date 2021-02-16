import numpy as np
import matplotlib.pyplot as plt

a=np.array([4,1])
b=np.array([-2,3])
sum=a+b

fig=plt.figure()
ax=fig.add_subplot(1,1,1)
#벡터a를 원점에서 시작하도록 그린다.
ax.quiver(0,0,a[0],a[1],angles='xy',scale_units='xy',scale=1,color='blue')
ax.text(a[0],a[1],'a',size=15)

#벡터b를 벡터a의 끝점에서 시작하도록 그린다.
ax.quiver(a[0],a[1],b[0],b[1],angles='xy',scale_units='xy',scale=1,color='green')
ax.text(a[0]+b[0],a[1]+b[1],'b',size=15)

#벡터b를 벡터a의 끝점에서 시작하도록 그린다.
ax.quiver(0,0,sum[0],sum[1],angles='xy',scale_units='xy',scale=1,color='red')
ax.text(sum[0]*0.5+0.3,b[1],'a+b',size=15,color='blue')

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
