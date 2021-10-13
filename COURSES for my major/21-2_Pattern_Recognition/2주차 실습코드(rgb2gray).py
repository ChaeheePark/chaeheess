import cv2
import matplotlib.pyplot as plt

#rgb to gray
img=cv2.imread('week2 #1/lenna.png')
rgb_img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
gray= cv2.cvtColor(rgb_img, cv2.COLOR_BGR2GRAY)

plt.subplot(1, 2, 1)
plt.imshow(gray,cmap='gray')

#histogram
plt.subplot(1, 2, 2)
plt.hist(gray.ravel(), 256, [0,256])

plt.show()