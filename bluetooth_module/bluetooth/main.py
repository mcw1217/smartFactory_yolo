import serial 

ser = serial.Serial('COM8', 9600,timeout=1) 
# 아두이노 블루투스 연결 한 후 COM8 자리에 연결된 아두이노의 송신 포트를 적음

def retrieveData():
  ser.write(b'1')
  print("1번 객체")
def retrieveData2():
  ser.write(b'2')
  print("2번 객체")
def retrieveData3():
  ser.write(b'3')
  print("3번 객체")

while True:
  
  uInput = input("Retrieve data: ")
  if uInput == '1':
    print(retrieveData())
  elif uInput == '2':
    print(retrieveData2())
  elif uInput == '3':
    print(retrieveData3())
  else:
    ser.write(b'0')