
function dydt = odefcn_2(t, y, rates)

y = reshape(y, 7, 2);
dydt = zeros(7, 2);
ind_concs = [0;0.25;0.5;1;2.5;5;10];
rates1 = 10.^rates;
modelfun1 = @(a, x) ((1 - a(5)) * (1 - 1./(1+(x/a(6)).^(a(7)) )) + a(5));
modelfun = @(a, x) ((1 - a(8))./(1+(x/(a(9) )).^(a(10)) ) + a(8));
dydt(:, 1) = (rates1(1)) * modelfun1(rates1, ind_concs) - (rates1(2)) * y(:, 1);
dydt(:, 2) = (rates1(3)) * modelfun(rates1, y(:, 1)) - (rates1(4)) * y(:, 2);
dydt = reshape(dydt, [], 1);

end
