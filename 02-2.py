import numpy as np
a = np.array([3,3])
b = np.array([3,0])
# 첫 번째 방법
print(np.dot(a,b))
# 두 번째 방법
norm_a = np.linalg.norm(a)
norm_b = np.linalg.norm(b)
print(norm_a * norm_b * np.cos(45*np.pi/180))
