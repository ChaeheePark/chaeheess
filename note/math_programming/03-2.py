import cv2
import numpy as np
#이미지를 컬러로 로드하여 넘파이 배열 형태로 img에 대입
img=cv2.imread('C:\\Users\\chaeh\\workspace3\\image.jpg',cv2.IMREAD_COLOR)
#이미지가 저장된 넘파이 배열의 너비와 높이를 구한다.
height,width=img.shape[:2]
#이동 변환 행렬을 생성, x방향으로 100, y방향으로 200픽셀 이동
M=np.array([[1,0,100],[0,1,200]],dtype=float)
#warpAffine함수를 사용하여 이동 변환 행렬을 적용
dst=cv2.warpAffine(img, M, (width,height))
result=cv2.hconcat([img,dst])
cv2.imshow("result",result)
cv2.waitKey(0)
