// PINs CONFIG
#define M1_1 5
#define M1_0 4
#define M0_0 9
#define M0_1 10
#define CH1 2
#define CH2 3
#define CH5 6
#define S1 7
#define S2 8
// OTHER CONFIG
#define AUTOMUL 1

void setup() {
  // SERIAL
  Serial.begin(115200);
  // PINMODEs
  pinMode(M0_0, OUTPUT);
  pinMode(M0_1, OUTPUT);
  pinMode(M1_0, OUTPUT);
  pinMode(M1_1, OUTPUT);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH5, INPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
}

// gets flysky pwm signal
int getCh(int pin) {
  int duration = pulseIn(pin, HIGH);
  int r = map(duration, 1000, 2000, -255, 255);
  if (r < -300 or r > 300) {
    return 0;
  }
  if (r > -20 and r < 20) {
    return 0;
  }
  return r;
}
bool sl = false;
bool sr = false;
int last_dir = 1;
void loop() {
  // COMPUTE X Y CONTROLS
  int x = 0;
  int y = 0;
  int a = getCh(CH5);
  if (a > 100) {
    sl = digitalRead(S1);
    sr = digitalRead(S2);
    y = 255;
    if (sl and !sr) {
      last_dir = 1;
    } else if (!sl and sr) {
      last_dir = -1;
    }
    x = 255 * last_dir * AUTOMUL;
  } else {
    x = getCh(CH1);
    y = getCh(CH2);
  }

  // MOTOR OUTPUT
  int d0 = y - x;
  int d1 = y + x;
  int m0 = constrain(d0, -255, 255);
  int m1 = constrain(d1, -255, 255);
  if (m0 < 0) {
    analogWrite(M0_0, -m0);
    analogWrite(M0_1, 0);
  } else {
    analogWrite(M0_0, 0);
    analogWrite(M0_1, m0);
  }
  if (m1 < 0) {
    analogWrite(M1_0, -m1);
    analogWrite(M1_1, 0);
  } else {
    analogWrite(M1_0, 0);
    analogWrite(M1_1, m1);
  }
  delay(15);
}
