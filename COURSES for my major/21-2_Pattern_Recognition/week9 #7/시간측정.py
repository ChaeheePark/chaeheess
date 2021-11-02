import cv2
import time

filepath = 'rectangle.png'
img = cv2.imread(filepath)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

sift = cv2.xfeatures2d.SIFT_create()
surf = cv2.xfeatures2d.SURF_create()

for i in range(4):

    li=[0.3,0.7,1.2,2.4]
    resize_img = cv2.resize(gray, None, fx=li[i], fy=li[i])

    print(">> (%d, %d)" % (resize_img.shape[0], resize_img.shape[1]))

    t1 = time.time()
    kpts = sift.detect(image=gray, mask=None)
    t2 = time.time()
    print("SIFT :", t2-t1, "sec")

    t1 = time.time()
    kpts = surf.detect(image=gray, mask=None)
    t2 = time.time()
    print("SURF :", t2-t1, "sec\n")
