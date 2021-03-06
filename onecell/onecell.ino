/* Written by Tom McCall 
    July 18th 2017    */

#include <stdio.h>
#include <math.h>

float iInj = 20.0; // Injected current
float tstep = 0.01; // time step
float nSteps = 2000.0; // length of simulation (in ms)

float gBarK = 36.0; // mS/cm^2, maximum potassium conductance
float gBarNa = 120.0; // mS/cm^2, maximum sodium conductance
float gM = 0.3; // mS/cm^2, membrane conductance
float eK = -77.0; // mV, potassium reversal potential
float eNa = 50.0; // mV, sodium reversal potential
float vRest = -54.4; // mV, resting membrane potential

float v = -65.00;
float n = 0.3177;
float m = 0.0529;
float h = 0.5961;
int threshold = 20;
int pin = 13;

void ap() {
  // Potassium current
  float alphan = 0.01 * (v + 55.) / (1 - exp(-(v + 55.) / 10.));
  float betan = 0.125 * exp(-(v + 65) / 80);

  // Sodium current
  float alpham = 0.1 * (v + 40) / (1 - exp(-(v + 40) / 10));
  float betam = 4 * exp(-(v + 65) / 18);
  float alphah = 0.07 * exp(-(v + 65) / 20);
  float betah = 1 / (1 + exp(-(v + 35) / 10));

  // Differential equations
  float dn = (alphan * (1.0 - n)) - (betan*n);
  float dm = alpham * (1.0 - m) - betam * m;
  float dh = alphah * (1.0 - h) - betah * h;
  float dv = -gBarNa * pow(m,3) * h * (v - eNa) - gBarK * pow(n,4) * (v - eK) - gM * (v - vRest) + iInj;

  // Euler integration updates
  n += (dn * tstep);
  m += (dm * tstep);
  h += (dh * tstep);
  v += (dv * tstep);

  /*if (v >= threshold) {
    digitalWrite(pin, HIGH);
    //exit(0); // Uncomment to exit after one spike
  } else {
    digitalWrite(pin, LOW);
  } */

  // analogWrite() only uses values 0-255, so
  // we need to map the min and max voltages to
  // 0 and 255, respectively
  float val = map(v, -75.00, 40, 0, 255);

  // Then write val to the output LED pin
  analogWrite(pin, val);
}

void setup() {
  // put your setup code here, to run once
  pinMode(pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  ap();
}

