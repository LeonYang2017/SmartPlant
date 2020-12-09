/*
亮 1
暗 0
黑线暗
*/

#define IN1_PIN 6
#define IN2_PIN 10
#define IN3_PIN 5
#define IN4_PIN 9



#define FORWARD     1
#define BACK        0
#define STOP        2

#define LEFT        0
#define RIGHT       1
#define MID         2
#define OUT         4//脱轨
#define END         5//到达终点
#define EXCEPTION   6
boolean IsRun=false;
int last_status=OUT;
int foward_or_back = FORWARD;
void initmov(){
  pinMode(IN1_PIN, OUTPUT);
 digitalWrite(IN1_PIN, LOW); // When not sending PWM, we want it low
 pinMode(IN2_PIN, OUTPUT);
 digitalWrite(IN2_PIN, LOW); // When not sending PWM, we want it low
 pinMode(IN3_PIN, OUTPUT);
 digitalWrite(IN3_PIN, LOW); // When not sending PWM, we want it low
 pinMode(IN4_PIN, OUTPUT);
 digitalWrite(IN4_PIN, LOW); // When not sending PWM, we want it low
}

void initxunji(){
 pinMode(A0, INPUT);
 pinMode(A1, INPUT);
 pinMode(A2, INPUT);
}

int xunji(){
  int left, mid, right;
  int  now_status=MID;
 left = digitalRead(A0);
 mid = digitalRead(A1);
 right = digitalRead(A2);
 if(left == 0 and mid == 0 and right == 0){
   return END;
 }else if(left==0 and mid == 0 and right == 1){
   return LEFT;
 }else if(left == 0 and mid == 1 and right == 0){
  return EXCEPTION;
 }else if(left == 0 and mid == 1 and right == 1){
  return LEFT;
 }else if(left == 1 and mid == 0 and right == 0){
  return RIGHT;
 }else if(left == 1 and mid == 0 and right == 1){
  return MID;
 }else if(left == 1 and mid == 1 and right == 0){
  return RIGHT;
 }else if(left == 1 and mid == 1 and right == 1){
  return OUT; 
 }
}

void mov_back(int mov_speed){
 analogWrite(IN1_PIN, mov_speed);
 analogWrite(IN2_PIN, 0);
 analogWrite(IN3_PIN, 0);
 analogWrite(IN4_PIN, mov_speed);
}

void mov_back_right(int mov_speed){
 analogWrite(IN1_PIN, mov_speed);
 analogWrite(IN2_PIN, 0);
 analogWrite(IN3_PIN, 0);
 analogWrite(IN4_PIN, 0);
}

void mov_back_left(int mov_speed){
 analogWrite(IN1_PIN, 0);
 analogWrite(IN2_PIN, 0);
 analogWrite(IN3_PIN, 0);
 analogWrite(IN4_PIN, mov_speed);
}

void mov_forward(int mov_speed){
 analogWrite(IN1_PIN, 0);
 analogWrite(IN2_PIN, mov_speed);
 analogWrite(IN3_PIN, mov_speed);
 analogWrite(IN4_PIN, 0);
}

void mov_forward_right(int mov_speed){
 analogWrite(IN1_PIN, 0);
 analogWrite(IN2_PIN, 90);
 analogWrite(IN3_PIN, mov_speed);
 analogWrite(IN4_PIN, 0);
}

void mov_forward_left(int mov_speed){
 analogWrite(IN1_PIN, 0);
 analogWrite(IN2_PIN, mov_speed);
 analogWrite(IN3_PIN, 90);
 analogWrite(IN4_PIN, 0);
}

void mov_out_right(int mov_speed){
 analogWrite(IN1_PIN, 50);
 analogWrite(IN2_PIN, 0);
 analogWrite(IN3_PIN, mov_speed);
 analogWrite(IN4_PIN, 0);
}

void mov_out_left(int mov_speed){
 analogWrite(IN1_PIN, 0);
 analogWrite(IN2_PIN, mov_speed);
 analogWrite(IN3_PIN, 0);
 analogWrite(IN4_PIN, 50);
}

void mov_stop(){
 analogWrite(IN1_PIN, LOW);
 analogWrite(IN2_PIN, LOW);
 analogWrite(IN3_PIN, LOW);
 analogWrite(IN4_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  initmov();
  initxunji();
  attachInterrupt(0,runsign,RISING);
  attachInterrupt(1,stopsign,RISING);
}
int mov_speed=90;
void loop() {
 if(IsRun){
 goahead(190);
 }else{
 mov_stop(); 
 }
}
void goahead(int forward_speed){
  int getxunji=xunji();
  switch(getxunji){
  case LEFT:
    mov_forward_right(forward_speed);
    last_status=LEFT;
    break;
  case RIGHT:
    mov_forward_left(forward_speed);
    last_status=RIGHT;
    break;
  case MID:
    mov_forward(forward_speed);
    last_status=MID;
    break;
  case EXCEPTION:
    mov_stop();
    last_status=EXCEPTION;
    break;
  case OUT:
    if(last_status == LEFT){
      mov_out_right(forward_speed);
      last_status=LEFT;
    }else if(last_status == RIGHT){
      mov_out_left(forward_speed);
      last_status=RIGHT;
    }else{
      mov_stop();
      last_status=END;
    }
    break;
  case END:
    mov_stop();
    last_status=END;
    break;
  }
}

void goback(int back_speed){
  int getxunji=xunji();
  switch(getxunji){
  case LEFT:
    mov_back_right(back_speed);
    last_status=LEFT;
    break;
  case RIGHT:
    mov_back_left(back_speed);
    last_status=RIGHT;
    break;
  case MID:
    mov_back(back_speed);
    last_status=MID;
    break;
  case EXCEPTION:
    mov_stop();
    last_status=EXCEPTION;
    break;
  case OUT:
    if(last_status == LEFT){
      mov_back_right(back_speed);
      last_status=LEFT;
    }else if(last_status == RIGHT){
      mov_back_left(back_speed);
      last_status=RIGHT;
    }else{
      mov_stop();
      last_status=END;
    }
    break;
  case END:
    mov_stop();
    last_status=END;
    break;
  }
}

void runsign(){
  IsRun=true;
}

void stopsign(){
  IsRun=false;
}
