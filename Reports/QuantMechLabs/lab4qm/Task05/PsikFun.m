function Psik = PsikFun(k,x,q1,q2,x1,x2)

%function finds of Eigenfunctions of Hamilton operator in task
%electron in potential well

    g1 = sqrt(q1^2 - k.^2);
    g2 = sqrt(q2^2 - k.^2);
  
    d = x2 - x1;
    
    C = 1./sqrt(d/2 + 1/2./g1 + 1/2./g2);
    A = k/q1.*C;
    B = k/q2.*C;
    [N,M]=size(k);
    ss = -1;
    for(i=1:N)
        ss = -ss;
        B(i,:) = ss*B(i,:);
    end
    
    bet = atan(k./g1);
    
    
    m1 = find( x(1,:) < x1);
    Psik(:,m1) = A(:,m1).*exp(g1(:,m1).*x(:,m1));
    
    m3 = find( x(1,:) > x2);
     Psik(:,m3) = B(:,m3).*exp(-g2(:,m3).*(x(:,m3)-d));

    m2 = find( x(1,:) >= x1 & x(1,:) <= x2);

    Psik(:,m2) = C(:,m2).*sin(k(:,m2).*x(:,m2) + bet(:,m2));

 
    
    