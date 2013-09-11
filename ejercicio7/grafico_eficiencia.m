function grafico_eficiencia(cores)

% Obtengo matriz de eficiencias
E = eficiencias(cores);

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
for i = 1:rows(E)
	Ymin(E(i, 1)) = min(Ymin(E(i, 1)), E(i, cores + 1));
	Ymax(E(i, 1)) = max(Ymax(E(i, 1)), E(i, cores + 1));
end

figure;
hold on;

plot(Ymin);
plot(Ymax, 'color', 'red');

legend({'M\''inimo', 'M\''aximo'});
legend('boxon');

xlabel('Quantums por core');
ylabel('Eficiencia (ciclos \''utiles / ciclos totales)');

set(gca, 'xtick', 1:8);
set(gca, 'xticklabel', X);

hold off;

oldpwd = pwd();
cd('../tex');
archivo = strcat('ej7-grafico-eficiencia-',
	             num2str(cores),
	             ifelse(cores == 1,  '-core', '-cores'));
print(archivo, '-S950,450', '-dtex');
cd(oldpwd);