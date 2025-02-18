
struct joy_value{
  int xValue,yValue;
  bool swValue;
}joy1;

#define INT1  0   // Motor 1 (Rear Left)
#define INT2  1// Motor 1 (Rear Left)
#define INT3  2 // Motor 2 (Rear Right)
#define INT4  4  // Motor 2 (Rear Right)
#define INT5  7   // Motor 1 (front Left)
#define INT6  8// Motor 1 (front Left)
#define INT7  12 // Motor 2 (front Right)
#define INT8  13  // Motor 2 (front Right)
#define ENA  3  // Enable pin (PWM for left rear motors)
#define ENB  5  // Enable pin (PWM for right rear motors)
#define ENC  6  // Enable pin (PWM for left front motors)
#define END  9  // Enable pin (PWM for right fornt motors)


const int xAxis = A1;
const int yAxis = A0;
const int swPin = 11;
float xpwm,ypwm;

void setup() {
  pinMode(INT1, OUTPUT);
  pinMode(INT2, OUTPUT);
  pinMode(INT3, OUTPUT);
  pinMode(INT4, OUTPUT);
  pinMode(INT5, OUTPUT);
  pinMode(INT6, OUTPUT);
  pinMode(INT7, OUTPUT);
  pinMode(INT8, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(ENC, OUTPUT);
  pinMode(END, OUTPUT);
  Serial.begin(9600);
}

void loop() {
    joy1.xValue=analogRead(xAxis);
    joy1.yValue=analogRead(yAxis);
    Serial.print("joy1.xValue: ");
    Serial.print(joy1.xValue);
    Serial.print(" | joy1.yValue: ");
    Serial.println(joy1.yValue);
    //int mapped_speed = map(input, 0, 1225, -255, 255); // Map to -255 to 255
    xpwm=scaleValue(joy1.xValue);
    ypwm=scaleValue(joy1.yValue);
    /*Serial.print("Mapped Speed: ");
    Serial.println(mapped_speed);*/

    x_Movement();
    y_Movement();

    // *Rear Motors: Forward / Backward*
    
    }
    


void x_Movement(){
  analogWrite(ENA, abs(ypwm));
  analogWrite(ENB, abs(ypwm));
  int en1 = ypwm>0 ? 1 : (ypwm = 0 ? 0 : 0) ;
  int en2 = ypwm>0 ? 0 : (ypwm = 0 ? 0 : 1) ;

  digitalWrite(INT5,en1);
  digitalWrite(INT6,en2);
  digitalWrite(INT7,en1);
  digitalWrite(INT8,en2);
  /*if(ypwm >0){
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
    }*/

}

void y_Movement(){
  int en1 = xpwm>0 ? 1 : (ypwm = 0 ? 0 : 0) ;
  int en2 = xpwm>0 ? 0 : (ypwm = 0 ? 0 : 1) ;
  /*if(xpwm = 0){
    analogWrite(ENC, abs(ypwm));
    analogWrite(END, abs(ypwm));
    }*/9
  int right_pwm = xpwm>0 ? abs(xpwm) : 0;
  int left_pwm = xpwm<0 ? abs(xpwm) : 0;
     analogWrite(END, right_pwm);
      analogWrite(ENC,left_pwm);
      digitalWrite(INT1,en1);
      digitalWrite(INT2,en2);
      digitalWrite(INT3,en1);
      digitalWrite(INT4,en2);
}

int scaleValue(int input){
  return map(input, 0, 1023, -255, 255); 
}