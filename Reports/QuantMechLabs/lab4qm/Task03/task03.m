function Task03
% find probability of electron in potential well
% potential well is infinitely deep well

clc   %clears all  from the Command Window
clear %removes all variables from the workspace (memory) 


%-------------------------------------------------------------
% PARAMETERS OF TASK

m = 9.1e-31;                %mass of electron (kg)
h = 1.05e-34;               %Plank's constant (J*s)
a = 2.0;                    %width of potential well (A, Angstroem)

x1 = 0:0.01:a/3; %x in Angstroms
x2 = a/3:0.01:2*a/3;
x3 = 2*a/3:0.01:a;

n = [1, 2, 3];   %quantum number

I1 = zeros(size(n));
I2 = zeros(size(n));
I3 = zeros(size(n));

Density = @(n, x) (sqrt(2/a) * sin(n * pi * x / a)).^2;

for i = 1:length(n)
	I1(i) = trapz(x1, Density(n(i), x1));
	fprintf("Probability from 0 to a/3 for n = %d: %.4f\n", n(i), I1(i));
	I2(i) = trapz(x2, Density(n(i), x2));
	fprintf("Probability from a/3 to 2*a/3 for n = %d: %.4f\n", n(i), I2(i));
	I3(i) = trapz(x3, Density(n(i), x3));
	fprintf("Probability from 2*a/3 to a for n = %d: %.4f\n", n(i), I3(i));
end

fprintf("Sum of probabilities for n = 1: %.4f\n", I1(1) + I2(1) + I3(1));
fprintf("Sum of probabilities for n = 2: %.4f\n", I1(2) + I2(2) + I3(2));
fprintf("Sum of probabilities for n = 3: %.4f\n", I1(3) + I2(3) + I3(3));

end