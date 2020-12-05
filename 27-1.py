import numpy as np

A = np.array([[1,2,3],[4,5,6],[7,8,9]])
x = np.array([0.1,0.2,0.3])
# dot함수를 사용하여 Ax를 계산한다.
print(np.dot(A,x))
# 행렬의 열을 벡터로 만들어 계산한다.
v1 = A[0]
v2 = A[1]
v3 = A[2]
c = np.array([np.dot(v1,x),np.dot(v2,x),np.dot(v3,x)])
print(c)
