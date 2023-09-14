
const int sw = 18;
const int dt = 19;
const int clk = 21;

int last_clock = 0;
int state = 0;

void setup() {
  pinMode(sw, INPUT);
  pinMode(dt, INPUT);
  pinMode(clk, INPUT);

  Serial.begin(115200);
}

void loop() {

  int clk_signal = digitalRead(clk);
  int key_signal = digitalRead(sw);
  int dt_signal = digitalRead(dt);
  if (clk_signal != last_clock) {
    if (dt_signal == HIGH) {
      state++;
    } else {
      state--;
    }
  } else {
    Serial.println("lala");
  }

  last_clock = clk_signal;

  Serial.print("clk_signal: ");
  Serial.print(clk_signal);
  Serial.print(", key_signal: ");
  Serial.print(key_signal);
  Serial.print(", dt_signal: ");
  Serial.print(dt_signal);
  Serial.print(", Count: ");
  Serial.print(state);
  Serial.print(", Key: ");
  Serial.println(key_signal);

  delay(10);
}
