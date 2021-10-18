import cv2
import numpy as np
import matplotlib.pyplot as plt


'''
원본 이미지
'''
img = cv2.imread('hand.jpg')
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
height = img.shape[0]
width = img.shape[1]
print('height:', height)
print('width:', width, end='\n\n')

plt.figure(figsize=(17, 4))

plt.subplot(1, 4, 1)
plt.imshow(img)
plt.axis('off')
plt.title('original')


'''
1) 이동 변환 (translation)
'''
tran_M = np.float32([[1, 0, 50],
                     [0, 1, 100]])
translated = cv2.warpAffine(img, tran_M, (width, height)) # default interpolation => bilinear
print('>> Translation matrix')
print(tran_M, end='\n\n')

plt.subplot(1, 4, 2)
plt.imshow(translated)
plt.axis('off')
plt.title('translation')


'''
2) 크기 변환 (scaling)
'''
scale_M = cv2.getRotationMatrix2D((0, 0), 0, 1.5) #1.5배 키움
scaled = cv2.warpAffine(img, scale_M, (width, height))
print('>> Scaling matrix')
print(scale_M, end='\n\n')

plt.subplot(1, 4, 3)
plt.imshow(scaled)
plt.axis('off')
plt.title('scaling')


'''
3) 회전 변환 (rotation)
'''
# OpenCV를 이용한 회전 변환
center = (width / 2, height / 2)
rot_M = cv2.getRotationMatrix2D(center, 90, 1.0)  # 회전 방향이 반시계방향(CCW; Counter Clock-Wise)
rotated = cv2.warpAffine(img, rot_M, (width, height))
print('>> OpenCV Rotation matrix')
print(rot_M, end='\n\n')

plt.subplot(1, 4, 4)
plt.imshow(rotated)
plt.axis('off')
plt.title('rotation')

# figure 출력
plt.tight_layout()
plt.show()
