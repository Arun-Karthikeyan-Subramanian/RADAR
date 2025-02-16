
struct joy_value{
  int xValue,yValue;
  bool swValue;
}joy1;

#define IN1  22   // Motor 1 (Rear Left)
#define IN2  24// Motor 1 (Rear Left)
#define IN3  26 // Motor 2 (Rear Right)
#define IN4  28  // Motor 2 (Rear Right)
#define IN5  30   // Motor 1 (front Left)
#define IN6  32// Motor 1 (front Left)
#define IN7  34 // Motor 2 (front Right)
#define IN8  36  // Motor 2 (front Right)
#define ENA  2  // Enable pin (PWM for left rear motors)
#define ENB  3  // Enable pin (PWM for right rear motors)
#define ENA  4  // Enable pin (PWM for left front motors)
#define ENB  5  // Enable pin (PWM for right fornt motors)


const int xAxis = A0;
const int yAxis = A1;
const int swPin = 6;
float xpwm,ypwm;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(STEER_LEFT, OUTPUT);
  pinMode(STEER_RIGHT, OUTPUT);
  pinMode(STEER_ENA, OUTPUT);

  Serial.begin(9600);
}

void loop() {
    joy1.xValue=analogRead(xAxis);
    joy1.yValue=analogRead(yAxis);
    //int mapped_speed = map(input, 0, 1225, -255, 255); // Map to -255 to 255
    xpwm=scaleValue(joy1.xValue);
    ypwm=scaleValue(joy1.yValue);
    /*Serial.print("Mapped Speed: ");
    Serial.println(mapped_speed);*/
    analogWrite(ENA, abs(ypwm));
    analogWrite(ENB, abs(ypwm));

    // *Rear Motors: Forward / Backward*
    if(xpwm = 0){
    analogWrite(ENC, abs(ypwm));
    analogWrite(END, abs(ypwm));
    }

    else{
      if(xpwm > 0){
        analogWrite(END, abs(xpwm));
        analogWrite(ENC, abs(255-xpwm));
      }
      else {
        analogWrite(END, abs(255-xpwm));
        analogWrite(ENC, abs(xpwm));
      }
    }
    if(ypwm >0){
      digitalWrite(INT1,1);
      digitalWrite(INT2,0);
      digitalWrite(INT3,1);
      digitalWrite(INT4,0);
    }
    else if(ypwm < 0){
      digitalWrite(INT1,0);
      digitalWrite(INT2,1);
      digitalWrite(INT3,0);
      digitalWrite(INT4,1);
    }
    else {
      digitalWrite(INT1,0);
      digitalWrite(INT2,0);
      digitalWrite(INT3,0);
      digitalWrite(INT4,0);
      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
    }
}

int scaleValue(int input){
  return map(input, 0, 1023, -255, 255); 
}
