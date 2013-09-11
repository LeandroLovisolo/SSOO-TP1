function grafico_runtime_3_cores(R)

% Obtengo matriz de runtimes
% R = runtimes(3);

% Rangos para el eje X
X = {};
for i = 1:8
	X{end + 1} = strcat('$(', num2str(i), ', *, *)$');
end

% Rangos para el eje Y
Ymin = ones(1, 8) * intmax();
Ymax = zeros(1, 8);
for i = 1:rows(R)
	Ymin(R(i, 1)) = min(Ymin(R(i, 1)), R(i, 4));
	Ymax(R(i, 1)) = max(Ymax(R(i, 1)), R(i, 4));
end

figure;
hold on;
plot(Ymin);
plot(Ymax, 'color', 'red');
hold off;

xlabel('Quantums por core');
ylabel('Tiempo de ejecuci\''on (ticks)');

set(gca, 'xtick', 1:8);
set(gca, 'xticklabel', X);

legend({'M\''inimo', 'M\''aximo'});
legend('boxon');

ylim([38 45]);

oldpwd = pwd();
cd('../tex');
print('ej7-grafico-runtime-3-cores', '-S950,450', '-dtex');
cd(oldpwd);