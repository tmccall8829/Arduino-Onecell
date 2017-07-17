% Hodgkin-Huxley model using Euler numerical integration
% Author: Chris Fietkiewicz, June 8, 2012
% Exercises:
% 1. What happens when you increase and decrease the following? iInj, gBarNa, gBarK, gM
% 2. Why does each one cause the effect that it does?
% 3. Can you find a value for each of those that will produce 2 spikes?
% 4. What happens if you increase BOTH gBarNa and gBarK?

clear
iInj = 2.5; % Injected current
step = 0.01; % Time step (ms)
nSteps = 2000; % Length of simulation (ms)

% Constants
gBarK = 36; % mS/cm^2, maximum potassium conductance
gBarNa = 120; % mS/cm^2, maximum sodium conductance
gM = 0.3; % mS/cm^2, membrane conductance
eK = -77; % mV, potassium reversal potential
eNa = 50; % mV, sodium reversal potential
vRest = -54.4; % mV, resting membrane potential

% Initial conditions
v = -65;
n = 0.3177;
m = 0.0529;
h = 0.5961;

for j = 1:nSteps
    % Potassium current
    alphan = 0.01 * (v + 55) / (1 - exp(-(v + 55) / 10));
    betan = 0.125 * exp(-(v + 65) / 80);
    
    % Sodium current
    alpham = 0.1 * (v + 40) / (1 - exp(-(v + 40) / 10));
    betam = 4 * exp(-(v + 65) / 18);
    alphah = 0.07 * exp(-(v + 65) / 20);
    betah = 1 / (1 + exp(-(v + 35) / 10));
    
    % Differential equations
    dn = (alphan * ( 1 - n) - betan * n);
    dm = (alpham * ( 1 - m) - betam * m);
    dh = (alphah * ( 1 - h) - betah * h);
    dv = (-gBarNa * m ^ 3 * h * (v - eNa) - gBarK * n ^ 4 * (v - eK) - gM * (v - vRest) + iInj);

    % Euler integration updates
    n = n + dn * step;
    m = m  + dm * step;
    h = h  + dh * step;
    v = v + dv * step;
    
    % Save the current values of variables for plotting later
    vMem(j) = v;
    iLeak(j) = gM * (v - vRest);
    iK(j) = gBarK * n ^ 4 * (v - eK);
    iNa(j) = gBarNa * m ^ 3 * h * (v - eNa);
end

% Plots
nPlots = 2;
subplot(nPlots,1,1)
plot(vMem)
ylabel('Vmem (mV)')
title('Vmem')
subplot(nPlots,1,2)
x = 1:length(iK);
plot(x, iK, 'r', x, iNa, 'b')
ylabel('current')
title('Channel Currents')
legend('K+', 'Na+')
