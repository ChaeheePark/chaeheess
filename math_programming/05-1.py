import cv2
import numpy as np
#이미지를 컬러로 로드하여 넘파이 배열 형태로 img에 대입
img=cv2.imread('C:\\Users\\chaeh\\workspace3\\image.jpg',cv2.IMREAD_COLOR)
#이미지가 저장된 넘파이 배열의 너비와 높이를 구한다.
height,width=img.shape[:2]
#45도 라디안으로 변환하여 코사인값과 사인값을 구하다.
angle=45
radian=angle*np.pi/180
c=np.cos(radian)
s=np.sin(radian)
#회전 변환행렬을 구성한다.
#opencv의 원점이 왼쪽 아래가 아니라, 왼쪽 위라서 [[c,-s,0],[s,c,0]]가 아니라 [[c,s,0],[-s,c,0]]이다.
rotation_matrix=np.array([[c,s,0],[-s,c,0],[0,0,1]],dtype=float)
dst=np.zeros(img.shape,dtype=np.uint8)
for y in range(height-1):
    for x in range(width-1):
        #backward mapping
        #결과 이미지의 픽셀 new_p로 이동하는 입력 이미지의 픽셀 old_p의 위치를 계산한다.
        new_p=np.array([x,y,1])
        inv_rotation_matrix=np.linalg.inv(rotation_matrix)
        old_p=np.dot(inv_rotation_matrix,new_p)
        #new_p 위치에 계산하여 얻은 old_p 픽셀의 값을 대입한다.
        x_,y_ = old_p[:2]
        x_=int(x_)
        y_=int(y_)
        #입력 이미지의 픽셀을 가져올 수 있는 경우에만 결과 이미지의 현재 위치 픽셀로 사용한다.
        if x_>0 and x_<width and y_ >0 and y_<height:
            dst[y_,x_]=img[y,x]
result=cv2.hconcat([img,dst])
cv2.imshow("result",result)
cv2.waitKey(0)
