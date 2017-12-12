
tspan = 0:60:600;
in_lst = zeros(11, 7);
out_lst = zeros(11, 7);
for i = 1:11,
	for j = 1:7,
		results = csvread(strcat('./module1_', int2str(j), '_output/result_t_' , int2str(tspan(i)), '.txt'));
		in_mean = mean(results(:, 3));
		out_mean = mean(results(:, 5));
		in_lst(i, j) = in_mean;
		out_lst(i, j) = out_mean;
	end
end

save('data.mat', 'in_lst', 'out_lst');

% fit model to the trajectories

load('data.mat')

n = 1000;
beta3_lst = zeros(n, 10);
exitflag3_lst = zeros(n, 1);
rse3_lst = zeros(n, 1);
bootstrap_index_matrix = randi([1 14*11/2], 14*11/2, n);
bootstrap_index_matrix = [bootstrap_index_matrix; bootstrap_index_matrix + 77];
in_out_lst = cat(2, in_lst, out_lst);
in_out_lst = reshape(in_out_lst, [], 1);
tmp_in_out_lst_matrix = in_out_lst(bootstrap_index_matrix);
beta0 = [-1, -1, -1, -1, -2, 2, 0, -2, 2, 0];
lb = [-Inf, -Inf, -Inf, -Inf, -Inf, log10(1), log10(1/3), -Inf, log10(1), log10(1/3)];
ub = [Inf, 0, Inf, 0, 0, log10(1000), log10(3), 0, log10(1000), log10(3)];

parfor i = 1:n,
    tmp_bootstrap_index = bootstrap_index_matrix(:, i);
    tmp_in_out_lst = tmp_in_out_lst_matrix(:, i);
    [beta3, resnorm3, residual3, exitflag3,output3,lambda3,jacobian3] = lsqcurvefit(@in_out_odes_3,beta0,tmp_bootstrap_index,tmp_in_out_lst, lb, ub);
    beta3_lst(i, :) = beta3;
    exitflag3_lst(i) = exitflag3;
    rse3_lst(i, :) = resnorm3;
end
save('module1_ind_3.mat', 'beta3_lst', 'exitflag3_lst', 'rse3_lst', 'bootstrap_index_matrix', 'tmp_in_out_lst_matrix');

load('module1_ind_3.mat')
fileID = fopen('module1_3_bootstrap_params_err.txt', 'w');
fprintf(fileID, '%10s %10s %10s %10s %10s %10s %10s %10s %10s %10s\n', 'beta1','lambda1', 'beta2', 'lambda2', 'gamma1', 'K1', 'h1', 'gamma2', 'K2', 'h2');
fprintf(fileID, '%10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n', beta3_lst');
fclose(fileID);

