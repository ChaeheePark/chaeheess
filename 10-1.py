import cv2
import numpy as np
img=cv2.imread('C:\\Users\\chaeh\\workspace3\\image.jpg',cv2.IMREAD_COLOR)
height,width=img.shape[:2]
scale_factor=0.5
scaling_matrix=np.array([[scale_factor,0,0],[0,scale_factor,0],[0,0,1]])
translation_matrix=np.array([[1,0,width/4],[0,1,height/4],[0,0,1]])
angle=45
radian=angle*np.pi/180
c=np.cos(radian)
s=np.sin(radian)
center_x=width/2
center_y=height/2
rotation_matrix=np.array([[c,s, (1-c) *center_x-s*center_y],[-s,c,s*center_x+(1-c) *center_y],[0,0,1]])
#정해진 순서대로 변환 행렬을 곱하여 하나의 행렬을 생성
T=np.eye(3)
T=np.dot(scaling_matrix,T)
T=np.dot(translation_matrix,T)
T=np.dot(rotation_matrix,T)
dst=np.zeros((height,width,img.shape[2]), dtype=np.uint8)
for y in range(height):
    for x in range(width):
        #미리 구해 놓은 변환 행렬을 행렬 곱 한번으로 적용
        #여기에서도 backward mapping을 사용
        new_p=np.array([x,y,1])
        inv_scaling_matrix=np.linalg.inv(T)
        old_p=np.dot(inv_scaling_matrix,new_p)
        x_,y_ = old_p[:2]
        x_=int(x_)
        y_=int(y_)
        if x_>0 and x_<width and y_ >0 and y_<height:
                dst.itemset((x,y,0),img.item(y_,x_,0)) #blue channel
                dst.itemset((x,y,1),img.item(y_,x_,1)) #green channel
                dst.itemset((x,y,2),img.item(y_,x_,2)) #red channel
cv2.imshow("result",dst)
cv2.waitKey(0)
