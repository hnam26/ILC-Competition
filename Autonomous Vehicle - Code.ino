#include <Servo.h>
#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)
#define trig  5           // Chân trig (HC-SR04)              GPIO5(D1)
#define echo  4           // Chân echo  (HC-SR04)             GPIO4(D2)
#define servo_pin 16      // Chân tín hiệu của servo          GPIO16(D0)
int speedCar = 500;       // điều hiển giá trị đốc độ 400 - 1023.
int distance_limit = 17;  // khoảng cách từ xe đến vật cản - do quán tính di chuyển của xe và thời gian phát hiện vật thể nên ta phải điều chỉnh giá trị này cao hơn thực tế
Servo myservo;
void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  myservo.attach(servo_pin);
  Serial.begin(9600);
}

void loop() {
  avoidObject(distance_limit);
}
// trả về khoảng cách tại gốc quay của servo (90-phía trước, 180-quay bên trái, 0-quay bên phải)
int ultrasonics(int angle) {
  myservo.write(angle);
  //  delay(1000);
  unsigned long duration;
  int distance;
  digitalWrite(trig, 0);
  delayMicroseconds(2);
  digitalWrite(trig, 1);
  delayMicroseconds(5);
  digitalWrite(trig, 0);

  duration = pulseIn(echo, HIGH);
  distance = int(duration / 2 / 27); //cm (Công thức tính khoảng cách từ giá trị nhận được - điều chỉnh 27 hoặc 29.412)
  delay(50);
  return distance;
}
/* đầu tiên cho xe duy chuyển về phía trước
  đọc khoảng cách ở phía trước, nếu phát hiện vật thể thì lùi lại 1 chút rồi dừng, 
  vervo quay sang trái và phải để đọc khoảng cách, 
  nếu khoảng cách bên nào lớn hơn thì quay sang hướng đó rồi tiếp tục duy chuyển
 */
int avoidObject(int allow_distance) {
  goAhead(speedCar);
  int distance_f = ultrasonics(90);
  if (distance_f > allow_distance) {
    goAhead(speedCar);
  }
  if (distance_f <= allow_distance) {
    goBack(speedCar);    delay(400);
    stopCar();
    int distance_l = ultrasonics(180);    delay(1000);
    int distance_r = ultrasonics(0);    delay(1000);
    //distance_f = ultrasonics_cm(90);    delay(1000);

    int distance_max = max_val2(distance_l, distance_r);
    if (distance_max == distance_l) {
      goLeft(speedCar);
      delay(400);
    }
    if (distance_max == distance_r) {
      goRight(speedCar);
      delay(400);
    }
  }
}

int max_val2(int a, int b) {
  int mx = a;
  if (b >= mx) return b;
  else return mx;
}
int max_val3(int a, int b, int c) {
  int mx = a;
  if (b >= mx) mx = b;
  if (c >= mx) mx = c;
  return mx;
}
// tiến
void goAhead(int speedCar) {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}
// lùi
void goBack(int speedCar) {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}
// quay sang trái
void goLeft(int speedCar) {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}
// quay sang phải
void goRight(int speedCar) {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}
// dừng 
void stopCar() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, 0);
}