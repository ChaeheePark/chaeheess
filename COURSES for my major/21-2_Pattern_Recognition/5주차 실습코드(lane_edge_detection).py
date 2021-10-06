import cv2
def pipeline(img):
# img = cv2.imread('./test_images/solidWhiteRight.jpg')

    gray_img = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

    blurred_img=cv2.GaussianBlur(gray_img,(3,3),0.0)

    edge_img=cv2.Canny(blurred_img,70,140)
#
# cv2.imshow('edge',edge_img)
# cv2.waitKey()
    return edge_img

# 동영상 테스트
cap = cv2.VideoCapture('./test_videos/solidWhiteRight.mp4')

while True:
    ok, frame = cap.read()
    if not ok:
        break

    edge_img=pipeline(frame)
    cv2.imshow('edge',edge_img)
    cv2.waitKey(30)

    key = cv2.waitKey(1)  # -1
    if key == ord('x'):
        break
cap.release()
