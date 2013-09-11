function grafico_runtime_4_cores()

% Obtengo matriz de runtimes
R = runtimes(4);

% Rangos para el eje X
X = {};
for i = 1:8
	X{end + 1} = strcat('$(', num2str(i), ', *, *, *)$');
end

% Rangos para el eje Y
Ymin = ones(1, 8) * intmax();
Ymax = zeros(1, 8);
for i = 1:rows(R)
	Ymin(R(i, 1)) = min(Ymin(R(i, 1)), R(i, 5));
	Ymax(R(i, 1)) = max(Ymax(R(i, 1)), R(i, 5));
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

ylim([30 35]);

oldpwd = pwd();
cd('../tex');
print('ej7-grafico-runtime-4-cores', '-S950,450', '-dtex');
cd(oldpwd);