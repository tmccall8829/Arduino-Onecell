#include <stdio.h>
#include <math.h>

int main() {
  float iInj = 2.5; // Injected current
  float step = 0.01; // time step
  float nSteps = 2000; // length of simulation (in ms)

  int gBarK = 36; // mS/cm^2, maximum potassium conductance
  int gBarNa = 120; // mS/cm^2, maximum sodium conductance
  float gM = 0.3; // mS/cm^2, membrane conductance
  int eK = -77; // mV, potassium reversal potential
  int eNa = 50; // mV, sodium reversal potential
  float vRest = -54.4; // mV, resting membrane potential

  // Initial conditions
  float v = -65;
  float n = 0.3177;
  float m = 0.0529;
  float h = 0.5961;

  float i = 0;
  // Calculations
  for (i = 0; i < nSteps; i += step) {
    double alphan = 0.01 * (v + 55.) / (1 - exp(-(v + 55.) / 10.));
    double betan = 0.125 * exp(-(v + 65) / 80);

    // Sodium current
    double alpham = 0.1 * (v + 40) / (1 - exp(-(v + 40) / 10));
    double betam = 4 * exp(-(v + 65) / 18);
    double alphah = 0.07 * exp(-(v + 65) / 20);
    double betah = 1 / (1 + exp(-(v + 35) / 10));

    // Differential equations
    double dn = (alphan * (1 - n) - betan * n);
    double dm = (alpham * (1 - m) - betam * m);
    double dh = (alphah * (1 - h) - betah * h);
    double dv = (-gBarNa * pow(m,3) * h * (v - eNa) - gBarK * pow(n,4) * (v - eK) - gM * (v - vRest) + iInj);

    // Euler integration updates
    double n = n + dn * step;
    double m = m + dm * step;
    double h = h + dh * step;
    double v = v + dv * step;

    printf("n: %f\t m: %f\t h: %f\t v: %f\n", n, m, h, v);
  }
}
