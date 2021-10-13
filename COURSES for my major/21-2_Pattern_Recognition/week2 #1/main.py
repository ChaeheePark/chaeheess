import cv2
import matplotlib.pyplot as plt

# cv2.calcHist(images,channels,mask,histSize,ranges,[,hist[,accumalte]])
## images  분석할 이미지 리스트/ channels 분석할 채널, 1channel이면 [0] , 3channel이면 [0].[1], [0,2] 등의 형태
## mask 분석할 이미지 영역 마스크(None이면 전체) / histSize bin의 개수(channel에 따라서 다르게) / ranges 히스토그램 분석 범위 [0,256] (channel에 따라 다르게)

#rgb to gray
img=cv2.imread('lenna.png')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
hist= cv2.calcHist(gray, [0], None, [256],[0,256])
plt.plot(hist)
plt.show()