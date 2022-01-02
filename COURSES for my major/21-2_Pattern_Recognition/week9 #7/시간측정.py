import cv2
import time

sift = cv2.xfeatures2d.SIFT_create() #SIFT 검출기 생성
surf = cv2.xfeatures2d.SURF_create() #SURT 검출기 생성

for scale_factor in [0.5,1.0,2.0,10]:

    img = cv2.imread('rectangle.png')
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) #gray image로 변환
    gray = cv2.resize(gray, None, fx=scale_factor, fy=scale_factor) # resize
    #fx,fy는 비율로 변환

    print(">> (%d, %d)" % (gray.shape[0], gray.shape[1]))

    t1 = time.time()
    kpts = sift.detect(image=gray, mask=None)
    print(kpts)
    t2 = time.time()
    print("SIFT :", t2-t1, "sec")

    t1 = time.time()
    kpts = surf.detect(image=gray, mask=None)
    print(kpts)
    t2 = time.time()
    print("SURF :", t2-t1, "sec\n")
