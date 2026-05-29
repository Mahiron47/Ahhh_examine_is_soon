function Task01
% draw energy levels of electron in potential well
% potential well is infinitely deep well

clc   %clears all  from the Command Window
clear %removes all variables from the workspace (memory) 


%-------------------------------------------------------------
% PARAMETERS OF TASK

m = 9.1e-31;  %mass of electron (kg)
e = 1.6e-19;  %elementary charge (C)
h = 1.05e-34; %Plank's constant (J*s)
a = 2e-10;    %width (m)

n = [1, 2, 3];   %quantum number 

k = pi .* n ./ a; %wave number (1/m)

E = (h^2 .* k.^2) ./ (2 * m); % energy of electron (J)

fprintf("E1 = %.2g J\n", E(1));
fprintf("E2 = %.2g J\n", E(2));
fprintf("E3 = %.2g J\n", E(3));

E = E / e;    % energy of electron (eV)
a = a * 1e10; % width in Angstroms

fprintf("E1 = %.2f eV\n", E(1));
fprintf("E2 = %.2f eV\n", E(2));
fprintf("E3 = %.2f eV\n", E(3));

fig = figure();

ax = axes(fig);
hold(ax, 'on');

grid(ax, 'on');
ax.XMinorGrid = 'on';
ax.YMinorGrid = 'on';

ax.GridAlpha = 0.55;
ax.MinorGridAlpha = 0.25;

ax.LineWidth = 0.9;
ax.Box = 'on';

ax.FontName = 'Times New Roman';
ax.FontSize = 12;

ArrowU = quiver(0, 0, 0, 100, 0, 'Color', 'black', 'LineWidth', 1.5, 'MaxHeadSize', 0.01, 'AutoScale', 'off');
ArrowX = quiver(-a * 0.5, 0, a * 2, 0, 'Color', 'black', 'LineWidth', 1.5, 'MaxHeadSize', 0.01, 'AutoScale', 'off');

Utext = text(-a * 0.2, 100 * 0.95, '$U(x)$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'middle', 'HorizontalAlignment', 'left', 'Interpreter', 'latex');
Xtext = text(a * 1.5 * 0.95, -10, '$x$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'center', 'Interpreter', 'latex');

E1 = line([0, a], [E(1), E(1)], 'Color', 'red', 'LineWidth', 2, 'DisplayName', 'E_{1}');
Etext1 = text(a * 0.5, E(1) + 0.1, '$E_{1}$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'bottom', 'Interpreter', 'latex');
E2 = line([0, a], [E(2), E(2)], 'Color', 'red', 'LineWidth', 2, 'DisplayName', 'E_{2}');
Etext2 = text(a * 0.5, E(2) + 0.1, '$E_{2}$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'bottom', 'Interpreter', 'latex');
E3 = line([0, a], [E(3), E(3)], 'Color', 'red', 'LineWidth', 2, 'DisplayName', 'E_{3}');
Etext3 = text(a * 0.5, E(3) + 0.1, '$E_{3}$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'bottom', 'Interpreter', 'latex');

P1 = line([0, 0, a, a], [100, 0, 0, 100], 'Color', 'blue', 'LineWidth', 3, 'DisplayName', 'U(x)');

P1text = text(a * 0.55, -5, '$a$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'middle', 'HorizontalAlignment', 'center', 'Interpreter', 'latex');

xlabel('$x\ (\AA)$', 'Interpreter', 'latex')
ylabel('$U(x), E\ ($eV$)$', 'Interpreter', 'latex')

title('$U(x), E$', 'Interpreter', 'latex')



end