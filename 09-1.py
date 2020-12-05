import numpy as np
A = np.array([[1,2], [3,4]])
invA = np.linalg.inv(A)
print(invA)
# (A역행렬)A = A(A역행렬) = I 임을 확인
# 파이썬에서 결과값이 근사값으로 출력된다.
# np.round 함수를 사용
print(np.dot(A, invA))
print(np.round(np.dot(A, invA)))
print(np.round(np.dot(invA, A)))
