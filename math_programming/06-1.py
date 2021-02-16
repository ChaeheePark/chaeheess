import cv2
import numpy as np
img=cv2.imread('C:\\Users\\chaeh\\workspace3\\image.jpg',cv2.IMREAD_COLOR)
height, width=img.shape[:2]
angle=45
radian=angle*np.pi/180
c=np.cos(radian)
s=np.sin(radian)
center_x=width/2
center_y=height/2
rotation_matrix=np.array([[c,s, (1-c) *center_x-s*center_y],[-s,c,s*center_x+(1-c) *center_y],[0,0,1]])
dst=np.zeros(img.shape, dtype=np.uint8)
for y in range(height-1):
    for x in range(width-1):
        new_p=np.array([x,y,1])
        inv_rotation_matrix=np.linalg.inv(rotation_matrix)
        old_p=np.dot(inv_rotation_matrix,new_p)
        x_,y_=old_p[:2]
        x_=int(x_)
        y_=int(y_)
        if x_> 0 and x_ < width and y_ > 0 and y_ <height:
            dst[y,x]=img[y_,x_]
result=cv2.hconcat([img, dst])
cv2.imshow("result", result)
cv2.waitKey(0)
