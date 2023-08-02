import cv2
import torch
import time
import serial
from models.experimental import attempt_load
from utils.general import non_max_suppression
from utils.general import xyxy2xywh, xywh2xyxy
from utils.torch_utils import select_device
from csv import writer
from datetime import datetime
coordinate = list()
data_input = list()
time_count = 0
ser = serial.Serial('COM8', 9600,timeout=1) 


# 모델 경로와 임계값, 비율 등 하이퍼파라미터를 설정합니다.
weights_path = 'best.pt'
conf_thres = 0.7
iou_thres = 0.5
device = select_device('cpu')

# YOLOv5 모델을 로드합니다.
model = attempt_load(weights_path, device=device)
model.eval()

# 웹캠으로부터 영상을 받아오는 객체를 생성합니다.
cap = cv2.VideoCapture(1)

while True:
    # 웹캠으로부터 영상 프레임을 가져옵니다.
    ret, frame = cap.read()
    frame = cv2.resize(frame, dsize=(640,480),interpolation=cv2.INTER_LINEAR)

    if not ret:
        break

    # YOLOv5 모델에 입력할 이미지를 전처리합니다.
    img = frame.copy()  # 이미지 데이터를 복사합니다.
    img = img.transpose(2, 0, 1)  # 이미지 데이터의 메모리 레이아웃을 변경합니다.
    img = torch.from_numpy(img).float().div(255.0).unsqueeze(0)

    # 모델을 이용하여 객체를 탐지합니다.
    detections = model(img)[0]
    detections = non_max_suppression(detections, conf_thres, iou_thres)
    # 탐지된 객체를 화면에 출력합니다.
    # print(detections)
    if detections is not None:
        detections = detections[0]
        detections = detections[:,:6]

        for xyxy in detections:
            x1, y1, x2, y2,conf,cls = [int(i) for i in xyxy]
            conf = float(xyxy[4])
            label = f"{model.names[int(cls)]} {conf:.2f}"
            cv2.rectangle(frame, (x1, y1), (x2, y2), (255, 0, 0), 2)
            data_input.append(int(cls))
            if len(data_input) == 5:
                if data_input[0] == data_input[1] == data_input[2] == data_input[3] == data_input[4]:
                    if data_input[0] == 1:
                        ser.write(b'1') 
                        data_input = list()
                        print("1번 들어옴!")     
                        time.sleep(3)         
                    elif data_input[0] == 2:
                        ser.write(b'2') 
                        data_input = list()    
                        print("2번 들어옴!")   
                        time.sleep(3)                         
                    elif data_input[0] == 3:
                        ser.write(b'3') 
                        data_input = list()
                        print("3번 들어옴!")       
                        time.sleep(3)           
                else:
                    data_input = list()
                    print("날림")    
                    time.sleep(3)          
            cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 1)

    # data_input에 데이터가 존재할 때 프레임 3번동안 detect된 데이터가 없으면 data_input을 초기화한다.
    detections = list(detections)
    if len(data_input)>0:
        if len(detections)==0:
            time_count += 1
            if time_count == 3:
                data_input = list()
                time_count = 0

# 화면에 출력된 영상을 보여줍니다.
         
    cv2.imshow('frame', frame)

    # 'q' 키를 누르면 종료합니다.
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 사용한 자원을 해제합니다.
cap.release()
cv2.destroyAllWindows()


      
    
    