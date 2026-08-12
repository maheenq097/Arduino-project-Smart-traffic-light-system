/*
  Smart Traffic Light System with Pedestrian Crossing
  Board: Arduino Uno
  ---------------------------------------------------
  Vehicle lights: pins 8 (Red), 9 (Yellow), 10 (Green)
  Pedestrian lights: pins 11 (Red/Don't Walk), 12 (Green/Walk)
  Pushbutton: pin 2 (interrupt pin, uses internal pull-up)
  Buzzer (optional): pin 7


  - Vehicles get GREEN by default, pedestrians see RED.
  - Pedestrian presses button -> request is captured via interrupt.
  - System finishes a safe transition (Green -> Yellow -> Red)
    before letting pedestrians cross.
  - Pedestrian GREEN blinks near the end as a warning, like real signals.
  - Cycle resets back to vehicle GREEN.
*/

// Vehicle traffic light pins
const int vehicleRed    = 8;
const int vehicleYellow = 9;
const int vehicleGreen  = 10;

// Pedestrian light pins
const int pedRed   = 11;
const int pedGreen = 12;

// Pedestrian pushbutton pin
const int buttonPin = 2;

// Optional buzzer for pedestrian audio cue
const int buzzerPin = 7;

volatile bool pedestrianRequest = false;
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounceDelay = 200; // ms

// Timing (all in milliseconds) - tweak these to taste
const unsigned long vehicleGreenTime    = 3000; // default green duration
const unsigned long yellowTime          = 4000;
const unsigned long pedestrianCrossTime = 6000;
const unsigned long pedestrianBlinkTime = 3000;
const unsigned long blinkInterval       = 300;

void setup() {
  pinMode(vehicleRed, OUTPUT);
  pinMode(vehicleYellow, OUTPUT);
  pinMode(vehicleGreen, OUTPUT);
  pinMode(pedRed, OUTPUT);
  pinMode(pedGreen, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);

  // Default state: vehicles green, pedestrians red
  digitalWrite(vehicleGreen, HIGH);
  digitalWrite(pedRed, HIGH);

  Serial.begin(9600);
  Serial.println("Smart Traffic Light System Ready");
}

void buttonISR() {
  unsigned long now = millis();
  if (now - lastInterruptTime > debounceDelay) {
    pedestrianRequest = true;
    lastInterruptTime = now;
  }
}

void loop() {
  // Hold vehicle green for its normal duration, but keep checking
  // for a pedestrian request the whole time.
  unsigned long phaseStart = millis();
  while (millis() - phaseStart < vehicleGreenTime) {
    if (pedestrianRequest) break;
  }

  if (pedestrianRequest) {
    Serial.println("Pedestrian request received");
    runPedestrianCycle();
    pedestrianRequest = false;
  }
  // If no request came in, loop repeats and vehicles simply stay green.
}

void runPedestrianCycle() {
  // Step 1: Vehicle Green -> Yellow (warn drivers)
  digitalWrite(vehicleGreen, LOW);
  digitalWrite(vehicleYellow, HIGH);
  delay(yellowTime);

  // Step 2: Vehicle Yellow -> Red, Pedestrian Red -> Green
  digitalWrite(vehicleYellow, LOW);
  digitalWrite(vehicleRed, HIGH);
  digitalWrite(pedRed, LOW);
  digitalWrite(pedGreen, HIGH);
  tone(buzzerPin, 1000, 200); // short beep = safe to walk

  delay(pedestrianCrossTime);

  // Step 3: Blink pedestrian green as a warning that time is running out
  unsigned long blinkStart = millis();
  bool state = true;
  while (millis() - blinkStart < pedestrianBlinkTime) {
    digitalWrite(pedGreen, state);
    state = !state;
    delay(blinkInterval);
  }
  digitalWrite(pedGreen, LOW);
  digitalWrite(pedRed, HIGH);

  // Step 4: Back to vehicle green
  digitalWrite(vehicleRed, LOW);
  digitalWrite(vehicleGreen, HIGH);
}
