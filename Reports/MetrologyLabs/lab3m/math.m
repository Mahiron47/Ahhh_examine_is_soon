clc; clear;

function g = Gamma(z)
    if z == 2
        g = 1;
    elseif z == 1/2
        g = sqrt(pi);
    else
        g = (z - 1) * Gamma(z - 1);
    end
end

n = 102;
q = 0.05;

nu = n - 1;
p = 1 - q/2;

f = @(x) (Gamma((nu + 1)/2)) / (sqrt(pi * nu) * Gamma(nu/2)) * (1 + x.^2/nu).^(-(nu + 1)/2);

cdf = @(x) 0.5 + integral(@(t) f(t), 0, x);

quantile_func = @(target) fzero(@(x) cdf(x) - target, 0);

t_quantile = quantile_func(p);

fprintf('Student quantile (t-distribution, nu=%d, q=%.4f): %.15f\n', nu, q, t_quantile);