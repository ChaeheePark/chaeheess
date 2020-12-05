import numpy as np
I = np.eye(3)
A = np.arange(9).reshape(3,3)
print(A)
# AI = IA = A 임을 확인
print((np.dot(A,I) == A).all())
print((np.dot(I,A) == A).all())
