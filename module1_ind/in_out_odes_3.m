
function y1 = in_out_odes_3(rates, bootstrap_index)

tspan = 0:60:600;
y0 = zeros(7, 2);
[~, y1] = ode15s(@(t, y) odefcn_2(t, y, rates), tspan, y0);
y1 = reshape(y1, [], 1);
y1 = y1(bootstrap_index);

end
