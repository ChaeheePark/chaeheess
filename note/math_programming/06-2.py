import cv2
import numpy as np
#이미지를 컬러로 로드하여 넘파이 배열 형태로 img에 대입
img=cv2.imread('C:\\Users\\chaeh\\workspace3\\image.jpg',cv2.IMREAD_COLOR)
#이미지가 저장된 넘파이 배열의 너비와 높이를 구한다.
height,width=img.shape[:2]
#45도 라디안으로 변환하여 코사인값과 사인값을 구하다.
angle=45
center=(width/2,height/2)
#getRotationMatrix2D함수를 사용하여 angle만큼 회전하는 회전 변환 행렬 생성
rotation_matrix=cv2.getRotationMatrix2D(center,angle,1)
#warpAffine함수를 사용하여 이동 변환 행렬을 적용
dst=cv2.warpAffine(img, rotation_matrix, (width,height))
result=cv2.hconcat([img,dst])
cv2.imshow("result",result)
cv2.waitKey(0)
