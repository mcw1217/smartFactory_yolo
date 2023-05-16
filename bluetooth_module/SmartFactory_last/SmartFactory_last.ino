


#include <SoftwareSerial.h>

/**
   스마트 팩토리 컨베이어벨트 최종 예제

   DC모터 : 레일을 움직임
   서보모터 : 제품을 색상에 따라 분류
   적외선 IR센서 : 물품이 적재됨을 감지.
   컬러센서 : 색상을 아두이노에 전달. 해당 값에 따라 서보모터의 각도가 결정
   RGB : 측정된 제품의 색상을 표시

   동작 순서
   1. 컨베이어 벨트 레일 작동
   2. 적외선 센서로부터 레일에 제품이 적재됨을 감지하면 일시 정지
   3. 레일이 컬러 센서까지 움직이기 시작
   4. 컬러센서에서 정지 후 해당 제품의 색상을 분석
   5. 색상 분석에 따른 결론에 따라 R, G, B가 결정되고, 해당되는 곳으로 서보모터가 동작
   6. 레일이 다시 움직이며 서보모터가 가리키는 방향으로 제품 분류
*/

/* 라이브러리 불러오기 */
#include <Servo.h>              // 서보모터 라이브러리 불러오기
#include <SoftwareSerial.h>     // 블루투스에 이용되는 라이브러리
#include <Wire.h>               // I2C 통신에 사용되는 라이브러리

/* 상수 선언 : 핀 번호, 속도제어, 서보모터의 각도*/
#define PIN_DC_DIRECTION 13  // DC모터(레일) 방향을 정하는 핀(현재 B모터 사용)
#define PIN_DC_SPEED 11      // DC모터(레일) 속도를 정하는 핀(현재 B모터 사용)
#define PIN_SERVO 9          // 서보모터 연결 핀
#define PIN_LED 5            // LED 연결 핀
#define PIN_IR A0            // 적외선 IR센서 연결 핀

#define POS_RED 55   // 빨간 색 제품을 분류할 서보모터의 각도
#define POS_GREEN 85  // 초록 색 제품을 분류할 서보모터의 각도
#define POS_BLUE 2    // 파란 색 제품을 분류할 서보모터의 각도
#define NUM_PIXELS 3  // 네오픽셀의 픽셀 수: 3

/* 변수 선언 : HW객체, 측정값, 기타 변수, ...*/
Servo servo;
SoftwareSerial mySerial(2,3);
int railSpeed = 80;               // 레일 기본 속도, 초기값은 160
bool isRailMoving = true;          // 레일 가동/정지 변수 유지 -> 블루투스로 레일 상태 조정
String myString = "";
int count = 1;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); 
  mySerial.setTimeout(10000);
  pinMode(PIN_DC_SPEED, OUTPUT);
  pinMode(PIN_DC_DIRECTION, OUTPUT);     
  servo.attach(PIN_SERVO);               
  servo.write(2);                        
  delay(500);                            
  digitalWrite(PIN_DC_DIRECTION, HIGH);  
  digitalWrite(PIN_DC_SPEED, LOW);  
  servo.detach();       
  pinMode(PIN_IR, INPUT);  
}

void loop() {

  if (digitalRead(PIN_IR) == HIGH) {  
    return;                         
  }
  digitalWrite(PIN_DC_DIRECTION, LOW);
  digitalWrite(PIN_DC_SPEED, LOW);                                 
  delay(500);
  digitalWrite(PIN_DC_DIRECTION, HIGH);
  digitalWrite(PIN_DC_SPEED, LOW); 
  delay(800);
  digitalWrite(PIN_DC_DIRECTION, LOW);
  digitalWrite(PIN_DC_SPEED, LOW);
  delay(2000);
 

  while(!mySerial.available()) // 10초간 객체 탐지 결과를 받을 수 있는 상태 -> 10초 후 객체 탐지 실패 시 레일이 뒤로감 / 객체 탐지 하면 바로 escape
  {
     if(count == 10) 
     {
        break;
     }
     delay(1000);
     count++;
     
    continue;
  }  
  count = 1;
  char myChar = (char)mySerial.read();
  mySerial.print(myChar);
  myString += myChar;
  delay(5);

  if(!myString.equals("")){
    if(myString.equals("1")){
      servo.attach(PIN_SERVO);
      servo.write(POS_RED);
      delay(200); // delay가 안 붙으면 모터가 작동하지 않음 
    }else if(myString.equals("2")){
      servo.attach(PIN_SERVO);  
      servo.write(POS_GREEN);
      delay(200);      
    }else if(myString.equals("3")){
      servo.attach(PIN_SERVO);  
      servo.write(POS_BLUE);
      delay(200);
    }else{
      digitalWrite(PIN_DC_DIRECTION, LOW); // 객체 탐지 실패 시 레일을 뒤로가게함
      digitalWrite(PIN_DC_SPEED, HIGH);
      delay(1200);  // 레일이 뒤로 가는 시간
      digitalWrite(PIN_DC_DIRECTION, HIGH); // 뒤로 간 후 다시 앞으로 정상작동하게 함
      digitalWrite(PIN_DC_SPEED, LOW);         
    }
    digitalWrite(PIN_DC_DIRECTION, HIGH); // 객체 탐지가 성공 시 앞으로 가게 함
    digitalWrite(PIN_DC_SPEED, LOW);     
    delay(200);                                                       
    servo.detach(); 
    myString="";
    }                                           
  }
