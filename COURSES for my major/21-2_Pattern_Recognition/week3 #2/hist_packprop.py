import cv2
import numpy as np

#모델영상의 2차원 H-S 히스토그램 계산
img_m=cv2.imread('model.jpg')
hsv_m=cv2.cvtColor(img_m,cv2.COLOR_BGR2HSV)
hist_m=cv2.calcHist([hsv_m],[0,1],None,[180,256],[0,180,0,256])

#입력영상의 2차원 H-S 히스토그램 계산
img_i=cv2.imread('hand.jpg')
hsv_i=cv2.cvtColor(img_i,cv2.COLOR_BGR2HSV)
hist_i=cv2.calcHist([hsv_i],[0,1],None,[180,256],[0,180,0,256])

#히스토그램 정규화
hist_m=hist_m/img_m.size
hist_i=hist_i/img_i.size
print("maximum of hist_m: %f"%hist_m.max())
print("maximum of hist_i: %f"%hist_i.max())

#비율 히스토그램 계산
hist_r=hist_m/(hist_i+1e-7)
hist_r=np.minimum(hist_r,1.0)
print(hist_r)
print("range of hist_r:[%.1f, %1f]"%(hist_r.min(),hist_r.max()))

#히스토그램 역투영 수행
height, width=img_i.shape[0],img_i.shape[1]
result=np.zeros_like(img_i,dtype='float32')

h,s,v=cv2.split(hsv_i)

for i in range(img_i.shape[0]):
    for j in range(img_i.shape[1]):
        h_value=h[i,j]
        s_value=s[i,j]
        confidence=hist_r[h_value,s_value]
        result[i,j]=confidence
##이진화 수행
ret, thresholded=cv2.threshold(result, 0.02, 255, cv2.THRESH_BINARY)
cv2.imwrite('result.jpg', thresholded)


#모폴로지 연산 적용
kernel=np.ones((13,13))
improved = cv2.morphologyEx(thresholded, cv2.MORPH_CLOSE, kernel)
cv2.imwrite('morphology.jpg',improved)