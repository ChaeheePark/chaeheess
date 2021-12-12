import cv2

# 이미지 불러오기
img_query = cv2.imread('box.png')           # query image
img_train = cv2.imread('box_in_scene.png')  # train image
gray_query = cv2.cvtColor(img_query, cv2.COLOR_BGR2GRAY)
gray_train = cv2.cvtColor(img_train, cv2.COLOR_BGR2GRAY)

# SIFT
sift = cv2.xfeatures2d.SIFT_create()  # SIFT 검출기 생성

# keypoint 검출 및 descriptor 생성
kp_query = sift.detect(image=gray_query, mask=None)
kp_query, desc_query = sift.compute(image=gray_query, keypoints=kp_query)
# kp_query, desc_query = sift.detectAndCompute(image=gray_query, mask=None)
kp_train, desc_train = sift.detectAndCompute(gray_train, None)

# feature matching
bf = cv2.BFMatcher(normType=cv2.NORM_L1, crossCheck=False)  # BFMatcher 객체 생성
matches = bf.match(queryDescriptors=desc_query, trainDescriptors=desc_train)  # descriptor 간 매칭 수행

matches = sorted(matches, key=lambda x: x.distance)  # distance를 기준으로 오름차순 정렬

# distance가 가장 작은 10개의 매칭쌍만 그리기 (Minkowski distance)
## https://docs.opencv.org/2.4/modules/features2d/doc/drawing_function_of_keypoints_and_matches.html#drawmatches
res = cv2.drawMatches(img1=img_query,
                      keypoints1=kp_query,
                      img2=img_train,
                      keypoints2=kp_train,
                      matches1to2=matches[:10],
                      outImg=None,
                      flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

cv2.imshow('result', res)
cv2.imwrite('result.png', res)
cv2.waitKey(0)

cv2.destroyAllWindows()
