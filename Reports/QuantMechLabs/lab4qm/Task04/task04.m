function Task04
% find and draw energy levels of electron in potential well
% potential well is finitely deep well

clc   %clears all  from the Command Window
clear %removes all variables from the workspace (memory) 


global b;
global c;
global nLevel;


%-------------------------------------------------------------
% PARAMETERS OF TASK

m = 9.1e-31;               %mass of electron (kg)
h = 1.05e-34;              %Plank's constant (J*s)

a = 9.7;                   %width of potential well (A, Angstroem)

U1 = 2.0;                  %left barier of potential well (eV)
U2 = 3.6;                  %rigth barier of potential well (eV)

%-------------------------------------------------------------
% FIND ENERGY LEVELS

b = (a*1.e-10)/h*sqrt(2*m*U1*1.6e-19);
c = (a*1.e-10)/h*sqrt(2*m*U2*1.6e-19);

m1 = min(b,c);
m2 = max(b,c);

if(m1 == m2)
    del = 0;
else
    del = pi/2 - atan(m1/sqrt(m2^2 - m1^2));
end

n = 0; % find of number of energy levels
if(m1 < del)
    n = 0;
else
    while(~(((n-1)*pi+del <= m1)&&(n*pi+del > m1)))
        n=n+1;    
    end
end


z=[]; % find of roots of dispersion equation
if(n>0)
    for(i=1:n)
        nLevel = i;
        zr =fzero(@FunDisper,[0,m1-1.e-10]);
        z = [z zr];
    end
end

E0 = h^2/2/m/(a*1.e-10)^2/1.6e-19;
E=E0*z.^2    % energy levels


%-------------------------------------------------------------
% DRAWD ENERGY LEVELS

% find size of viewport
dmax = 10;
if(a/2 >0.75*dmax)
    x1 = a/2-0.7*a;
    x2 = a/2+0.7*a;
else
    if(a<0.1*dmax)
    x1 = a/2-10.*a;
    x2 = a/2+10.*a;
    else
    x1 = a/2-dmax;
    x2 = a/2+dmax;
    end
end
y1 = -0.25*max(U1,U2);
y2 = 1.25*max(U1,U2);

% draw potential well
yU1 = U1;
yU2 = U2;
line([x1 0 0 a a x2],[yU1 yU1 0 0 yU2 yU2],'LineWidth',3,'Color',[0 0.2 0.8]);
axis([x1 x2 y1 y2]);
grid on

% draw axis
dx = (x2-x1)/50;
dy = (y2-y1)/50;

line([0 0],[y1 y2],'Color',[0 0 0]);
line([-dx 0 dx],[y2-dy y2 y2-dy],'Color',[0 0 0]);

line([x1 x2],[0 0],'Color',[0 0 0]);
line([x2-dx x2 x2-dx],[dy 0 -dy],'Color',[0 0 0]);


% draw energy levels
Estr = ['E_{1}';'E_{2}';'E_{3}';'E_{4}';'E_{5}';...
        'E_{6}';'E_{7}';'E_{8}';'E_{9}'];

if(n>0)
    for(i=1:n)
        line([x1 x2-5*dx],[E(i) E(i)],'LineWidth',2,'Color',[1 0 1]);
        if(n<=9)
            text(x2-4*dx,E(i),Estr(i,:));
        end
    end
end


% draw text
str = ['U_{1}'];
text(-3*dx, yU1+3*dy, str);

str = ['U_{2}'];
text(a+1*dx, yU2+3*dy, str);

str = ['0'];
text(-dx, -2*dy, str);

str = ['a'];
text(a+dx, -2*dy, str);

str = ['x'];
text(x2-2*dx, -2*dy, str);

str = ['U(x)'];
text(dx, y2-3*dy, str);

title('U(x), E','FontSize',[12]);
xlabel('x   (A)','FontSize',[12])
ylabel('U, E   (eV)','FontSize',[12])

