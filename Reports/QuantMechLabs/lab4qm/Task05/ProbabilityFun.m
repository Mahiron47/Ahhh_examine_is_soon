function Psik = ProbabilityFun(k,x,q1,q2,x1,x2)

%function finds of Eigenfunctions of Hamilton operator in task
%electron in potential well

    g1 = sqrt(q1^2 - k.^2);
    g2 = sqrt(q2^2 - k.^2);
  
    d = x2 - x1;
    
    C = 1./sqrt(d/2 + 1/2./g1 + 1/2./g2);
    A = k/q1.*C;
    B = k/q2.*C;
    %[N,M]=size(k);
    %ss = -1;
    %for(i=1:N)
    %    ss = -ss;
    %    B(i,:) = ss*B(i,:);
    %end
    
    %bet = atan(k./g1);
    
    
    m1 = find( x(1,:) == 1);
    Psik(:,m1) = A(:,m1).^2/2./g1(:,m1);
    
    m3 = find( x(1,:) == 3);
     Psik(:,m3) = B(:,m3).^2/2./g2(:,m3);

    m2 = find( x(1,:) == 2);

    Psik(:,m2) = C(:,m2).^2/2.*(d + g1(:,m2)/q1^2 + g2(:,m2)/q2^2);

 
