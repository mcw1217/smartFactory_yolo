detect 코드
python detect.py --weights ./runs/train/yolov5_smartfactory23/weights/best.pt --img 640 --conf 0.7 --source ./mydataset/data3.mp4 

source만 바꿔서 detect
만약 학습한 모델이 변경될 경우
--weights 부분의 경로 변경


train 코드
python train.py --img 640 --batch 100 --epochs 10000 --data ./data.yaml --cfg ./models/yolov5s.yaml --weights yolov5s.pt --name yolov5_smartfactory2

batch 100까지는 메모리 허용됨, --name이 학습된 모델 저장되는 폴더 이름

- 학습: roboflow를 사용하여 데이터셋 구축
- 데이터셋: 배터리 사진 51개 + 증가된 사진 70개

[!] 가까이있을땐 confidence가 높지만, 거리가 멀 경우 다른 물체의 confidence가 높아져 오작동하는 경우가 많음 / 학습 데이터의 부족 

roboflow로 만든 데이터셋 yolov5에 넣어서 학습 시키는 법
- roboflow에 있는 파일인 train, test, valid, data.yaml을 넣어준 후 train 시키면됨 / 기존에있는 파일은 삭제 해준다.