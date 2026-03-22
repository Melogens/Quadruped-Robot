#include <Servo.h>
#define numLegs 4
#define DOF 2

Servo servos[numLegs][DOF];

// replacable with trained weights
float w1[4][6] = {
  {0.5, -0.2, 0.3, 0.1, -0.4, 0.2},
  {-0.3, 0.6, -0.1, 0.2, 0.5, -0.2},
  {0.2, 0.1, -0.5, 0.4, -0.3, 0.6},
  {-0.4, 0.3, 0.2, -0.1, 0.6, 0.1}
};

float w2[8][4] = {
  {0.3, -0.2, 0.5, 0.1},
  {-0.1, 0.4, -0.3, 0.6},
  {0.2, 0.3, -0.4, 0.5},
  {-0.5, 0.2, 0.1, -0.3},
  {0.6, -0.1, 0.2, 0.4},
  {-0.2, 0.5, -0.6, 0.3},
  {0.1, -0.4, 0.3, 0.2},
  {0.4, 0.1, -0.2, 0.5}
};

float relu(float x) {
  return x > 0 ? x : 0;
}

// Forward pass
void forward(float input[6], float output[8]) {
  float hidden[4];

  for (int i = 0; i < 4; ++i) {
    hidden[i] = 0;
    for (int j = 0; j < 6; j++) {
      hidden[i] += w1[i][j] * input[j];
    }
    hidden[i] = relu(hidden[i]);
  }

  for (int i = 0; i < 8; ++i) {
    output[i] = 0;
    for (int j = 0; j < 4; j++) {
      output[i] += w2[i][j] * hidden[j];
    }
  }
}

void setup() {
  int pins[numLegs][DOF] = {{2,3},{4,5},{6,7},{8,9}};

  for (int i = 0; i < numLegs; ++i) {
    for (int j = 0; j < DOF; ++j) {
      servos[i][j].attach(pins[i][j]);
    }
  }
}

void loop() {
  float input[6] = {
    millis()/1000.0,
    sin(millis()*0.001),
    cos(millis()*0.001),
    0,0,0
  };

  float output[8];
  forward(input, output);

  for (int i = 0; i < numLegs; ++i) {
    int hip = constrain(90 + output[i*2]*30, 0, 180);
    int knee = constrain(90 + output[i*2+1]*30, 0, 180);

    servos[i][0].write(hip);
    servos[i][1].write(knee);
  }

  delay(20);
}