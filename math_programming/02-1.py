import numpy as np
a = np.array([1,2])
# 자기 자신에 대한 내적의 제곱근
print(np.sqrt(np.dot(a,a)))
# 벡터의 크기
print(np.linalg.norm(a))
