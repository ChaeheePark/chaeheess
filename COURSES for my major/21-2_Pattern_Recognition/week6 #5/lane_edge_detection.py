import cv2
import pipeline

# 동영상 테스트
cap = cv2.VideoCapture('./test_videos/solidWhiteRight.mp4')

while True:
    ok, frame = cap.read()
    if not ok:
        break

    result = pipeline.run(frame)

    cv2.imshow('result', result)
    key = cv2.waitKey(1)  # -1
    if key == ord('x'):
        break

cap.release()
cv2.destroyAllWindows()

