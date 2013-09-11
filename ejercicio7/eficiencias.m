function A = eficiencias(cores)

A = [];

files = glob(strcat('resultados/experimento-', num2str(cores), '*.log'));
for i = 1:length(files)
	% Leo el log del experimento y obtengo el runtime
	data = leer_log(files{i});
	ef = eficiencia(data, cores);

	% Leo los enteros en el nombre de archivo (ej.: experimento-1-2-3.log => [1 2 3])
	[s, e, te, m, t, nm] = regexp(files{i}, '\d');

	% Agrego los resultados a la matriz de retorno
	res = [];
	for j = 2:length(m)	% Empiezo desde j = 2 para no incluir la cantidad de cores
		res(end + 1) = str2num(m{j});
	end
	res(end + 1) = ef;
	A = [A; res];
end


function ef = eficiencia(data, cores)

ticks = 0;
ciclos_no_aprovechados = 0;

for i = 1:length(data)
	% Me quedo con el último número de tick observado
	if(strcmp(data{i}{1}, 'CPU')     ||
	   strcmp(data{i}{1}, 'BLOCK')   ||
	   strcmp(data{i}{1}, 'UNBLOCK') ||
	   strcmp(data{i}{1}, 'EXIT'))
		ticks = str2num(data{i}{2});
	end

	% Verifico si algún core está idle o cambiando el contexto
    if((strcmp(data{i}{1}, 'CPU') && strcmp(data{i}{3}, '-1')) ||
       (strcmp(data{i}{1}, '#')   && strcmp(data{i}{2}, 'CONTEXT')))
        ciclos_no_aprovechados++;
    end
end

ciclos = (ticks + 1) * cores;
ef = (ciclos - ciclos_no_aprovechados) / ciclos;