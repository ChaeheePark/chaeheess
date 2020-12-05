import numpy as np
A = np.array([[1,2,3], [4,5,6]])
B = np.array([[4,1], [5,2], [1,2]])
# 행렬 A를 구성하는 행 벡터 출력
print(A)
print(B)
print(A.dot(B))
print(np.dot(A,B))
