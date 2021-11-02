"""
pip install opencv-contrib-python==3.4.2.16
"""
import cv2
import numpy as np
import util

filepath = 'rectangle.png'
img = cv2.imread(filepath)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

sift = cv2.xfeatures2d.SIFT_create()  # SIFT 검출기 생성
kpts = sift.detect(image=gray, mask=None)  # SIFT keypoints 검출

# keypoint 시각화 (https://docs.opencv.org/2.4/modules/features2d/doc/drawing_function_of_keypoints_and_matches.html#drawkeypoints)
res = cv2.drawKeypoints(image=gray,
                        keypoints=kpts,
                        outImage=None)

res_with_rich = cv2.drawKeypoints(image=gray,
                                  keypoints=kpts,
                                  outImage=None,
                                  flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

concatenated = np.hstack((res, res_with_rich))
cv2.imshow('concatenated', concatenated)
cv2.waitKey(0)

cv2.destroyAllWindows()



##############################
# keypoint 결과 출력해보기
##############################
octave_set = set()
layer_set = set()
for i, keypoint in enumerate(kpts):
    octave, layer, scale = util.unpackSIFTOctave(keypoint)
    print('[keypoint #%d] octave: %2d\t layer: %d\t scale: %f' % (i, octave, layer, scale))

    octave_set.add(octave)
    layer_set.add(layer)

print('octave set:', octave_set)
print('layer set:', layer_set)
##############################