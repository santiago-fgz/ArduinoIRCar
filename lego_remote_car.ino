#include <Servo.h>
#include <IRremote.h>

/*
 * ==========================================================================================
 *ARDUINO LEGO REMOTE CAR
 *Santiago F.G Zamora
 *==========================================================================================
*/

/* Servomotor */
Servo myservo;
int servo = 9; /* pin */
int str8_pos = 90; /* posición central del servomotor */
int min_pos = str8_pos-25;
int max_pos = str8_pos+25;
int pos = str8_pos;

/* L298N (motor DC)*/
int enA = 5; // pin, entrada de 0 a 255
int in1 = 4; // pin 
int in2 = 3; // pin

int max_vel = 65;
int min_vel = 45;

int vel = 0;

/* HW490 (IR sensor) */
int ir_recv = 7;
IRrecv irrecv(ir_recv);
decode_results results;

/*
 * ==========================================================================================
 *  SETUP
 *==========================================================================================
*/
void setup() {
  Serial.begin(9600);
  /* L298N */
  pinMode(enA, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  /* Servo */
  myservo.attach(servo);
  myservo.write(str8_pos);
  /* IR Reciever */
  irrecv.enableIRIn();
  /*
   * 
  fwd : 40100525
  right : 40100F2F
  left : 40100727
  back : 40100D2D
  stop : 40100929
  speed up : 40100040
  speed down : 40100848
  *
  */
}

/*
 * ==========================================================================================
 *  FUNCTIONS
 *==========================================================================================
*/

/*
 * ==========================================================================================
 *  LOOP
 *==========================================================================================
*/

void loop() {
  if (irrecv.decode(&results)){ // Returns 0 if no data ready, 1 if data ready.    
    
    if (results.value == 0x40100929){ // STOP
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      vel = 0;
      analogWrite(enA,vel);
      pos = str8_pos;
      myservo.write(pos);
      Serial.println(" ");
      Serial.println("ALTO");
    }
    if (results.value == 0x40100525){ // FWD
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      vel = min_vel;
      analogWrite(enA,vel);
      Serial.println(" ");
      Serial.println("ADELANTE");
    }
    if (results.value == 0x40100D2D){ // BACK
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      vel = min_vel;
      analogWrite(enA,vel);
      Serial.println(" ");
      Serial.println("ATRAS");
    }
    if (results.value == 0x40100F2F){ // RIGHT
      if (pos < max_pos){
        pos = pos + 5;
      }
      myservo.write(pos);
      Serial.println(" ");
      Serial.println("DERECHA");
      Serial.println(pos);
    }
    if (results.value == 0x40100727){ // LEFT
      if (pos > min_pos){
        pos = pos - 5;
      }
      myservo.write(pos);
      Serial.println(" ");
      Serial.println("IZQUIERDA");
      Serial.println(pos);
    }
    if (results.value == 0x40100040){ // SPEED UP
      if (vel < max_vel){
        vel = vel + 1;
      }
      analogWrite(enA,vel);
      Serial.println(" ");
      Serial.println("MAS VEL");
      Serial.println(vel);
    }
    if (results.value == 0x40100848){ // SPEED DOWN
      if (vel > min_vel){
        vel = vel - 1;
      }
      analogWrite(enA,vel);
      Serial.println(" ");
      Serial.println("MENOS VEL");
      Serial.println(vel);
    }
    
    irrecv.resume(); // Restart the ISR state machine and Receive the next value
  }
}
