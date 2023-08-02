import cv2 as cv
import datetime

cap = cv.VideoCapture(1)

while True:
    ret, img= cap.read()
    time = datetime.datetime.now().strftime("%Y-%m-%d %H-%M-%S")
    
    if not ret:
        break
    
    cv.imshow('img',img)
    key = cv.waitKey(1)
    if key == ord('q'):
        break
    elif key == ord('c'):
        cv.imwrite(f'imgFile/{time}-.jpg',img)
        print("캡처가 완료되었습니다!")
        
cv.release()
cv.destroyAllWindows()
