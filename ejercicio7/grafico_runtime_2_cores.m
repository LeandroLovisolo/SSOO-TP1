function grafico_runtime_2_cores()

% Obtengo matriz de runtimes
R = runtimes(2);

% Rangos para el eje X
X = {};
for i = 1:7
	X{end + 1} = strcat('$(', num2str(i), ', ', num2str(i), ' \\ldots 8)$');
end
X{end + 1} = '$(8, 8)$';

% Rangos para el eje Y
Y = zeros(1, 8);
for i = 1:rows(R)
	Y(R(i, 1)) = R(i, 3);
end

plot(Y);

xlabel('Quantums por core');
ylabel('Tiempo de ejecuci\''on (ticks)');

set(gca, 'xtick', 1:8);
set(gca, 'xticklabel', X);

oldpwd = pwd();
cd('../tex');
print('ej7-grafico-runtime-2-cores', '-S950,450', '-dtex');
cd(oldpwd);