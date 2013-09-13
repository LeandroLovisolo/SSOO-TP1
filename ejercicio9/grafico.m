function grafico()

data = csvread('experimento.csv');

figure;
hold on;
plot(1:rows(data), data(:, 1), 'color', 'red');
plot(1:rows(data), data(:, 2), 'color', 'green');
plot(1:rows(data), data(:, 3), 'color', 'blue');
plot(1:rows(data), data(:, 4), 'color', 'magenta');
hold off;

legend({'Tarea 1', 'Tarea 2', 'Tarea 3', 'Tarea 4'});
legend('boxon');

xlabel('N\''umero de simulaci\''on ($n$)');
ylabel('Ciclos de CPU utilizados (promedio)');

ylim([15, 35]);

oldpwd = pwd();
cd('../tex');
print('ej9-grafico-ecuanimidad', '-S950,450', '-dtex');
cd(oldpwd);