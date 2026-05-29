function Task05
% find and draw wave functions of electron in potential well
% potential well is finitely deep well

clc   %clears all  from the Command Window
clear %removes all variables from the workspace (memory) 


global b;
global c;
global nLevel;


%-------------------------------------------------------------
% PARAMETERS OF TASK

m = 9.1e-31;                %mass of electron (kg)
h = 1.05e-34;               %Plank's constant (J*s)

a = 9.7;                   %width of potential well (A, Angstroem)

U1 = 2.0;                    %left barier of potential well (eV)
U2 = 3.6;                    %rigth barier of potential well (eV)

E = [0.2533 0.9872 1.9979]

%-------------------------------------------------------------
% FIND AND DRAW WAVE FUNCTIONS


%-------------------------------------------------------------
% FIND PARAMETERS WAVE FUNCTIONS

% change energy level so that except division by zero
Umin = min(U1,U2);
for(i=1:length(E))
    if(abs(E(i) - Umin)<1.e-6)
        E(i) = Umin - 1.e-6;    
    end
end


% array <E> transform to array <k>
E0 = h^2/2/m/(a*1.e-10)^2/1.6e-19;
z=sqrt(E/E0);
k = z/a;
    
q1 = sqrt(2*U1*1.6e-19*m)/h*1.e-10;
q2 = sqrt(2*U2*1.6e-19*m)/h*1.e-10;

%-------------------------------------------------------------
% FIND PROBABILITY

nLevl = 1:3;
[NLevl,KLevl]=meshgrid(nLevl,k);

% calculate Probability P(x1 < x < x2)
p = ProbabilityFun(KLevl,NLevl,q1,q2,0,a);

N = length(k);
for(i=1:N)
    stri = num2str(i);
    strE = num2str(E(i));
    Energy_Level = strcat('     E(',stri,') =',strE,'  >>>>>>>>>>')
    Probability = p(i,:)
end

%figure(7)
%plot(nLevl,p(1,:),nLevl,p(2,:),nLevl,p(3,:));

%-------------------------------------------------------------
% DRAW WAVE FUNCTIONS

% find size of viewport
x1 = a/2-1.5*a;
x2 = a/2+1.5*a;;
y1 = -sqrt(2/a);
y2 = sqrt(2/a);


% draw potential well
m2 = max(U1,U2);
yU1 = 2/3*U1*(y2-y1)/m2 +y1+1/6*(y2-y1); 
yU2 = 2/3*U2*(y2-y1)/m2 +y1+1/6*(y2-y1); 
yU0 = 1/6*(y2-y1)+y1; 

figure(2);
line([x1 0 0 a a x2],[yU1 yU1 yU0 yU0 yU2 yU2],'LineWidth',3,'Color',[0 0.2 0.8]);
axis([x1 x2 y1 y2]);
grid on

 
x = x1:0.1:x2;

[X,K]=meshgrid(x,k);

% calculate wave functions for various <x> and <k>
% <y> is two-dimensional array  length(k) x length(x)
y = PsikFun(K,X,q1,q2,0,a);

% draw wave functions
hold on
plot(x,y);

% draw axis
dx = (x2-x1)/50;
dy = (y2-y1)/50;
    
line([0 0],[y1 y2],'Color',[0 0 0]);
line([-dx 0 dx],[y2-dy y2 y2-dy],'Color',[0 0 0]);

line([x1 x2],[0 0],'Color',[0 0 0]);
line([x2-dx x2 x2-dx],[dy 0 -dy],'Color',[0 0 0]);


% draw text

str = ['0'];
text(-dx, -2*dy, str);

str = ['a'];
text(a+dx, -2*dy, str);

str = ['x'];
text(x2-2*dx, -2*dy, str);

str = ['\Psi(x)'];
text(-3*dx, y2-3*dy, str);

title('\Psi(x)','FontSize',12);
xlabel('x   (A)','FontSize',12)
ylabel('\Psi   (A^{-1/2})','FontSize',12)



%-------------------------------------------------------------
% DRAW WAVE FUNCTIONS, new size of viewport

% find size of viewport
x1 = a/2-6.5*a;
x2 = a/2+6.5*a;;
y1 = -sqrt(2/a);
y2 = sqrt(2/a);


% draw potential well
m2 = max(U1,U2);
yU1 = 2/3*U1*(y2-y1)/m2 +y1+1/6*(y2-y1); 
yU2 = 2/3*U2*(y2-y1)/m2 +y1+1/6*(y2-y1); 
yU0 = 1/6*(y2-y1)+y1; 

figure(3);
line([x1 0 0 a a x2],[yU1 yU1 yU0 yU0 yU2 yU2],'LineWidth',3,'Color',[0 0.2 0.8]);
axis([x1 x2 y1 y2]);
grid on




x = x1:0.1:x2;

[X,K]=meshgrid(x,k);

% calculate wave functions for various <x> and <k>
% <y> is two-dimensional array  length(k) x length(x)
y = PsikFun(K,X,q1,q2,0,a);

% draw wave functions
hold on
plot(x,y);

% draw axis
dx = (x2-x1)/50;
dy = (y2-y1)/50;
    
line([0 0],[y1 y2],'Color',[0 0 0]);
line([-dx 0 dx],[y2-dy y2 y2-dy],'Color',[0 0 0]);

line([x1 x2],[0 0],'Color',[0 0 0]);
line([x2-dx x2 x2-dx],[dy 0 -dy],'Color',[0 0 0]);



% draw text

str = ['0'];
text(-dx, -2*dy, str);

str = ['a'];
text(a+dx, -2*dy, str);

str = ['x'];
text(x2-2*dx, -2*dy, str);

str = ['\Psi(x)'];
text(-3*dx, y2-3*dy, str);

title('\Psi(x)','FontSize',12);
xlabel('x   (A)','FontSize',12)
ylabel('\Psi   (A^{-1/2})','FontSize',12)





