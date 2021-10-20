import cv2
import numpy as np

# 이미지 읽어오기
img = cv2.imread('shapes.png')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Harris corner 검출 (리턴값: 특징 가능성 C map)
# https://docs.opencv.org/2.4/modules/imgproc/doc/feature_detection.html#cornerharris
conf_map = cv2.cornerHarris(src=gray,     # single-channel을 갖는 8-bit 또는 floating-point 이미지
                            blockSize=3,  # 코너 검출 시 고려할 이웃의 크기
                            ksize=3,      # Sobel 마스크의 크기 (홀수)
                            k=0.04)       # 경험적 상수

# 특징 가능성 C에 따른 해당 영역의 타입 판별
res = np.copy(img)
threshold = 0.01 * conf_map.max()         # 임의의 임계값 flat 한 영역을 판단해 주기 위해
res[conf_map > threshold] = [255, 0, 0]   # C가 일정 임계값보다 크면 corner
res[conf_map <= threshold] = [0, 0, 255]  # C가 0에 가까우면 flat
res[conf_map < 0] = [255, 255, 255]       # C가 음수이면 edge

# 결과 시각화
concatenated = np.hstack((img, res)) #수평방향으로 두 이미지를 쌓음
concatenated = cv2.resize(concatenated, None, fx=2.0, fy=2.0)  # 이미지 확대하기 위함. 중요하지 않음
cv2.imshow('harris corner', concatenated)
cv2.waitKey()

cv2.destroyAllWindows()
