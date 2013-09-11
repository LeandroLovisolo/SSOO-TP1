function grafico_runtime(cores, comparar_min_y_max)

% Obtengo matriz de runtimes
R = runtimes(cores);

% Rangos para el eje X
X = {};
for i = 1:8
	tick = strcat('$(', num2str(i));
	for j = 2:cores
		tick = strcat(tick, ', *');
	end
	tick = strcat(tick, ')$');
	X{end + 1} = tick;
end

% Rangos para el eje Y
Ymin = double(ones(1, 8) * intmax());
Ymax = zeros(1, 8);
for i = 1:rows(R)
	Ymin(R(i, 1)) = min(Ymin(R(i, 1)), R(i, cores + 1));
	Ymax(R(i, 1)) = max(Ymax(R(i, 1)), R(i, cores + 1));
end

figure;
hold on;

plot(Ymin);
plot(Ymax, 'color', 'red');

legend({'M\''inimo', 'M\''aximo'});
legend('boxon');

xlabel('Quantums por core');
ylabel('Tiempo de ejecuci\''on (ticks)');

set(gca, 'xtick', 1:8);
set(gca, 'xticklabel', X);

hold off;

oldpwd = pwd();
cd('../tex');
archivo = strcat('ej7-grafico-runtime-',
	             num2str(cores),
	             ifelse(cores == 1,  '-core', '-cores'));
print(archivo, '-S950,450', '-dtex');
cd(oldpwd);