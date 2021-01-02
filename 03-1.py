import cv2
import numpy as np
#이미지를 컬러로 로드하여 넘파이 배열 형태로 img에 대입
img=cv2.imread('C:\\Users\\chaeh\\workspace3\\image.jpg',cv2.IMREAD_COLOR)
#이미지가 저장된 넘파이 배열의 너비와 높이를 구한다.
height,width=img.shape[:2]
#이동 변환 행렬을 생성, x방향으로 100, y방향으로 200픽셀 이동
M=np.array([[1,0,100],[0,1,200],[0,0,1]],dtype=float)
#결과 이미지를 저장할 넘파이 배열을 입력이미지와 같은 크기로 생성
dst=np.zeros(img.shape,dtype=np.uint8)
#이미지의 전체 픽셀을 스캔
for y in range(height-1):
    for x in range(width-1):
        #현재 픽셀 p의 좌표에 이동변환 행렬 M을 곱하여 이동시킨 좌표 p_를 구한다.
        p=np.array([x,y,1])
        p_=np.dot(M,p)
        #이동한 위치인 p_에서 좌표 x_,y_를 정수 형태로 가져온다.
        x_,y_ = p_[:2]
        x_=int(x_)
        y_=int(y_)
        #입력 이미지 크기 내로 픽셀이 이동한 경우에만 결과 이미지를 저장할 넘파이 배열로 픽셀을 복사한다.
        if x_>0 and x_<width and y_ >0 and y_<height:
            dst[y_,x_]=img[y,x]
#입력 이미지와 결과 이미지를 수평으로 연결하여 하나의 이미지로 만든다.
result=cv2.hconcat([img,dst])
cv2.imshow("result",result)
cv2.waitKey(0)
