#define L_MOTOR_POSITIVE 0
#define L_MOTOR_NEGATIVE 0
#define L_MOTOR_ENABLE 0
#define R_MOTOR_POSITIVE 0
#define R_MOTOR_NEGATIVE 0
#define R_MOTOR_ENABLE 0
#define MAX_SPEED 0

/*
 * Movements:
 *     W := forward
 *     S := stop
 *     D := right
 *     A := left
 *     R := extreme right
 *     L := extreme left
 *     X := reverse
 */

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

int incomingByte = Serial.read();

void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
    if (incomingByte == 'A') {
      digitalWrite(L_MOTOR_POSITIVE, HIGH);
      digitalWrite(L_MOTOR_NEGATIVE, LOW);

      digitalWrite(R_MOTOR_POSITIVE, LOW);
      digitalWrite(R_MOTOR_NEGATIVE, LOW);

      analogWrite(L_MOTOR_ENABLE, MAX_SPEED);
      analogWrite(R_MOTOR_ENABLE, 0);
      delay(500);
    }
    else if (incomingByte == 'D') {
      digitalWrite(L_MOTOR_POSITIVE, LOW);
      digitalWrite(L_MOTOR_NEGATIVE, LOW);

      digitalWrite(R_MOTOR_POSITIVE, HIGH);
      digitalWrite(R_MOTOR_NEGATIVE, LOW);

      analogWrite(L_MOTOR_ENABLE, 0);
      analogWrite(R_MOTOR_ENABLE, MAX_SPEED);
      delay(500);
    }
    else if (incomingByte == 'W') {
      digitalWrite(L_MOTOR_POSITIVE, HIGH);
      digitalWrite(L_MOTOR_NEGATIVE, LOW);

      digitalWrite(R_MOTOR_POSITIVE, HIGH);
      digitalWrite(R_MOTOR_NEGATIVE, LOW);

      analogWrite(L_MOTOR_ENABLE, MAX_SPEED);
      analogWrite(R_MOTOR_ENABLE, MAX_SPEED);
      delay(500);
    }
    else if (incomingByte == 'S') {
      digitalWrite(L_MOTOR_POSITIVE, LOW);
      digitalWrite(L_MOTOR_NEGATIVE, LOW);

      digitalWrite(R_MOTOR_POSITIVE, LOW);
      digitalWrite(R_MOTOR_POSITIVE, LOW);

      analogWrite(L_MOTOR_ENABLE, 0);
      analogWrite(R_MOTOR_ENABLE, 0);
      delay(500);
    }
    else if (incomingByte == 'R') {
      digitalWrite(R_MOTOR_POSITIVE, HIGH);
      digitalWrite(R_MOTOR_NEGATIVE, LOW);

      digitalWrite(L_MOTOR_POSITIVE, LOW);
      digitalWrite(L_MOTOR_NEGATIVE, HIGH);

      analogWrite(L_MOTOR_ENABLE, MAX_SPEED);
      analogWrite(R_MOTOR_ENABLE, MAX_SPEED);
      delay(500);
    }
    else if (incomingByte == 'L') {
      digitalWrite(R_MOTOR_POSITIVE, LOW);
      digitalWrite(R_MOTOR_NEGATIVE, HIGH);

      digitalWrite(L_MOTOR_POSITIVE, HIGH);
      digitalWrite(L_MOTOR_NEGATIVE, LOW);

      analogWrite(L_MOTOR_ENABLE, MAX_SPEED);
      analogWrite(R_MOTOR_ENABLE, MAX_SPEED);
      delay(500);
    }
    else if (incomingByte == 'X') {
      digitalWrite(R_MOTOR_POSITIVE, LOW);
      digitalWrite(R_MOTOR_NEGATIVE, HIGH);

      digitalWrite(L_MOTOR_POSITIVE, LOW);
      digitalWrite(L_MOTOR_NEGATIVE, HIGH);

      analogWrite(L_MOTOR_POSITIVE, MAX_SPEED);
      analogWrite(R_MOTOR_NEGATIVE, MAX_SPEED);
    }
  }
}



