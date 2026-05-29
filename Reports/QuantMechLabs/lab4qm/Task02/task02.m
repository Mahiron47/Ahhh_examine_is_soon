function Task02
% draw wave functions of electron in potential well
% potential well is infinitely deep well

clc   %clears all  from the Command Window
clear %removes all variables from the workspace (memory) 


%-------------------------------------------------------------
% PARAMETERS OF TASK

m = 9.1e-31;                %mass of electron (kg)
h = 1.05e-34;               %Plank's constant (J*s)
a = 2;                  %width of potential well (Angstroms)

x = 0:0.01:a; %x in Angstroms

n = [1, 2, 3];   %quantum number

psi = @(n, x) sqrt(2 / a) * sin(n * pi * x / a); %wave function

psi1 = psi(n(1), x);
psi2 = psi(n(2), x);
psi3 = psi(n(3), x);

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

ArrowPsi = quiver(0, 0, 0, 1.5, 0, 'Color', 'black', 'LineWidth', 1.5, 'MaxHeadSize', 0.3, 'AutoScale', 'off');
ArrowX = quiver(-a * 0.5, 0, a * 2, 0, 'Color', 'black', 'LineWidth', 1.5, 'MaxHeadSize', 0.1, 'AutoScale', 'off');

Utext = text(-a * 0.2, 1.5 * 0.9, '$\Psi(x)$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'middle', 'HorizontalAlignment', 'left', 'Interpreter', 'latex');
Xtext = text(a * 1.5 * 0.95, -0.2, '$x$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'center', 'Interpreter', 'latex');

Psi1 = plot(x, psi1, 'Color', 'red', 'LineWidth', 2, 'DisplayName', '$\Psi_{1}$');
Psitext1 = text(a * 0.5, 1, '$\Psi_{1}$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'bottom', 'Interpreter', 'latex');
Psi2 = plot(x, psi2, 'Color', 'red', 'LineWidth', 2, 'DisplayName', '$\Psi_{2}$');
Psitext2 = text(a * 0.45, -1.2, '$\Psi_{2}$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'bottom', 'Interpreter', 'latex');
Psi3 = plot(x, psi3, 'Color', 'red', 'LineWidth', 2, 'DisplayName', '$\Psi_{3}$');
Psitext3 = text(a * 0.7, -1.2, '$\Psi_{3}$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'bottom', 'Interpreter', 'latex');

P1 = line([0, 0], [-1.5, 1.5], 'Color', 'blue', 'LineWidth', 3);
P2 = line([a, a], [-1.5, 1.5], 'Color', 'blue', 'LineWidth', 3);

P1text = text(a * 1.05, -0.1, '$a$', 'FontSize', 14, 'Color', 'black', 'VerticalAlignment', 'middle', 'HorizontalAlignment', 'center', 'Interpreter', 'latex');

xlabel('$x\ (\AA)$', 'Interpreter', 'latex')
ylabel('$\Psi(x)\ (\AA^{-1/2})$', 'Interpreter', 'latex')

title('$\Psi(x)$', 'Interpreter', 'latex')

scriptFullPath = mfilename('fullpath');
scriptFolder = fileparts(scriptFullPath);

if isempty(scriptFolder)
    scriptFolder = pwd;
end

outputFile = fullfile(scriptFolder, 'Task02.png');

if exist(outputFile, 'file')
    delete(outputFile);
end

exportgraphics(fig, outputFile, ...
    'Resolution', 300, ...
    'BackgroundColor', 'white');