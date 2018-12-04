/**************************************************************************
定义
**************************************************************************/
int motor_en = 2; //电机使能
int Left_motor_go = 3;  //电机A前进，驱动板A1
int Left_motor_back = 4; //电机A后退，驱动板A2
int Right_motor_go = 5; //电机B前进，驱动板B1
int Right_motor_back = 6; //电机B后退，驱动板B2
int pwm_big = 155;
int pwm_small = 0;

int DO1 = A0; //红外对管DO1
int DO2 = A1; //红外对管DO2
int DO3 = A2; //红外对管DO3
int DO4 = A3; //红外对管DO4
int ledread[4] = {1,1,1,1}; //红外对管扫描结果 无光线返回时，熄灯，输出1

int Trig = A4;  //超声波，Echo回声脚
int Echo = A5;  //超声波，Trig触发脚
float distance = 0.0f; //超声波测得的距离 
float distance_min = 15;
#define ultrasonic 0 //开关超声波逻辑

/**************************************************************************
Arduino 初始化
**************************************************************************/
void setup()
{
  //初始化电机驱动IO为输出方式
  pinMode(motor_en,OUTPUT); // PIN 2 (PWM)
  digitalWrite(motor_en,HIGH);  // 驱动使能
  pinMode(Left_motor_go,OUTPUT); // PIN 3 (PWM)
  pinMode(Left_motor_back,OUTPUT); // PIN 4 (PWM)
  pinMode(Right_motor_go,OUTPUT);// PIN 5 (PWM)
  pinMode(Right_motor_back,OUTPUT);// PIN 6 (PWM)

  //初始化红外对管引脚为输入
  pinMode(DO1, INPUT); //PIN 22 (DIGITAL)
  pinMode(DO2, INPUT); //PIN 24 (DIGITAL)
  pinMode(DO3, INPUT); //PIN 26 (DIGITAL)
  pinMode(DO4, INPUT); //PIN 28 (DIGITAL)

  //初始化超声波
  pinMode(Echo, INPUT); //定义超声波输入脚
  pinMode(Trig, OUTPUT); //定义超声波输出脚
  
  //初始化串口
  Serial.begin(9600);
  Serial.println("I'm Robot Tang!"); 
}

/**************************************************************************
红外对管设置
**************************************************************************/
void ledscan(int time)
{
  ledread[0] = digitalRead(DO1);
  ledread[1] = digitalRead(DO2);
  ledread[2] = digitalRead(DO3);
  ledread[3] = digitalRead(DO4);
  delay(time);   //执行时间，可以调整
}

/**************************************************************************
超声波测距
**************************************************************************/
void Distance_test(int time)   // 量出前方距离 
{
  digitalWrite(Trig, LOW);   // 给触发脚低电平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // 给触发脚高电平10μs，这里至少是10μs
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // 持续给触发脚低电
  float Fdistance = pulseIn(Echo, HIGH);  // 读取高电平时间(单位：微秒)
  distance = Fdistance/58;       //单位厘米
  //Serial.println(distance);         //显示距离
  delay(time);   //执行时间，可以调整
}  

/**************************************************************************
电机驱动设置
**************************************************************************/
void run(int time)     // 前进
{
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,pwm_big);//PWM比例0~255调速，左右轮差异略增减
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,pwm_big);//PWM比例0~255调速，左右轮差异略增减
  delay(time);   //执行时间，可以调整
}

void brake(int time)  //刹车，停车
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  delay(time);//执行时间，可以调整
}

void left(int time)         //左转(左轮不动，右轮前进)
{
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,pwm_big);
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,pwm_small);
  delay(time);  //执行时间，可以调整
}

void right(int time)        //右转(右轮不动，左轮前进)
{
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,pwm_small);
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,pwm_big);
  delay(time);  //执行时间，可以调整
}

void back(int time)        //后退
{
  digitalWrite(Right_motor_back,pwm_big);
  analogWrite(Right_motor_go,LOW);
  digitalWrite(Left_motor_back,pwm_big);
  analogWrite(Left_motor_go,LOW);
  delay(time);  //执行时间，可以调整
}

/**************************************************************************
主循环
**************************************************************************/
void loop() {
    while (Serial.available() > 0) {
    char cmd = Serial.read();
    Serial.println(cmd);        

    if(cmd == 'w'){
      run(1000); 
      brake(100);
    }
    else if(cmd == 's'){
      back(1000);
      brake(100);
      }
    else if(cmd == 'a'){
      left(1500);
      brake(100);
      }
    else if(cmd == 'd'){
      right(1500);
      brake(100);
      }
    else if(cmd == 'q'){
      ledscan(10); //红外对管扫描
      if ((ledread[0] == 0) && (ledread[1] == 0) && (ledread[2] == 0) && (ledread[3] == 0))
        run(100);
      else if ( ((ledread[0] == 0) && (ledread[1] == 0)) && ((ledread[2] == 1) || (ledread[3] == 1)) )
        left(100);
      else if ( ((ledread[0] == 1) || (ledread[1] == 1)) && ((ledread[2] == 0) && (ledread[3] == 0)))
        right(1);
      else if ((ledread[0] == 0) && (ledread[1] == 1) && (ledread[2] == 1) && (ledread[3] == 0))
        run(100);
      else
        brake(100);
    }
    
    else brake(100);
     Serial.println(ledread[0]) ;
      }
}
