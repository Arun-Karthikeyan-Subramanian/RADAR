struct signal{
  int rover_x,rover_y,arm,gripper;
}s1;
Servo lift,gripper;
const int int1 = 22;
const int int2 = 24;
const int int3 = 26;
const int int4 = 28;
const int ena = 2;
const int enb = 3;
const int lift_servo = 4;
const int gripper_servo = 5;
const int x_pin = A0;
const int y_pin = A1;
int lift_angle = 90;
int gripper_angle = 90;
int pin[]={int1,int2,int3,int4,ena,enb,lift_servo,gripper_servo};
void setup() {
  // put your setup code here, to run once
  for(int i=0;i<(sizeof(pin)/sizeof(pin[0]));i++){
    pinMode(pin[i],OUTPUT);
  }
  lift.attach(lift_servo);
  gripper.attach(gripper_servo);
  lift.write(90);
  gripper.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  s1.rover_x=analogRead(x_pin);
  s1.rover_y=analogRead(y_pin);
  movement(s1,int1,int2,int3,int4,ena,enb);
  servo_control(s1, lift_servo, gripper_servo);
}
void movement(signal s1,int int1,int int2,int int3,int int4,int ena,int enb){
  if(abs(s1.rover_x)<=abs(s1.rover_y)){
    bool int1_st = s1.rover_y>=0?HIGH:LOW;
    bool int2_st = s1.rover_y>=0?LOW:HIGH;
    digitalWrite(int1,int1_st);
    digitalWrite(int2,int2_st);
    digitalWrite(int3,int1_st);
    digitalWrite(int4,int2_st);
    analogWrite(ena,abs(s1.rover_y));
    analogWrite(enb,abs(s1.rover_y));
  }

  else{
    bool int2_st = s1.rover_x>=0?HIGH:LOW;
    bool int1_st = s1.rover_x>=0?LOW:HIGH;
    bool int4_st = s1.rover_x<=0?HIGH:LOW;
    bool int3_st = s1.rover_x<=0?LOW:HIGH;
    digitalWrite(int1,int1_st);
    digitalWrite(int2,int2_st);
    digitalWrite(int3,int3_st);
    digitalWrite(int4,int4_st);
    analogWrite(ena,abs(s1.rover_x));
  }
}
void servo_control(signal s1, int lift_servo, int gripper_servo){
  int lift_value = servo_map(s1.arm);
  int gripper_value = servo_map(s1.gripper);
  if (lift_angle <= 180 && lift_angle>=0){
    lift_angle+=lift_value;
  }
  else lift_angle = lift_angle;
  lift_angle = constrain(lift_angle,0,180);
  lift.write(lift_angle);

  if (gripper_angle <= 180 && gripper_angle>=0){
    gripper_angle+=gripper_value;
  }
  else gripper_angle = gripper_angle;
  gripper_angle = constrain(gripper_angle,0,180);
  gripper.write(gripper_angle);
  delay(50);
}
int servo_map(int val) {
   if (val > 0) {
    return map(val, 0, 255, 0, 10);  // Map positive direction
  } else {
    return map(val, -255,0 , -10, 0);  // Map negative direction
  }
}