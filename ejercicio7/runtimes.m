function A = runtimes(cores)

A = [];

files = glob(strcat('resultados/experimento-', num2str(cores), '*.log'));
for i = 1:length(files)
	% Leo el log del experimento y obtengo el runtime
	data = leer_log(files{i});
	rt = runtime(data);

	% Leo los enteros en el nombre de archivo (ej.: experimento-1-2-3.log => [1 2 3])
	[s, e, te, m, t, nm] = regexp(files{i}, '\d');

	% Agrego los resultados a la matriz de retorno
	res = [];
	for j = 2:length(m)	% Empiezo desde j = 2 para no incluir la cantidad de cores
		res(end + 1) = str2num(m{j});
	end
	res(end + 1) = rt;
 	A = [A; res];
end


function t = runtime(data)

t = -1;
for i = 1:length(data)
	if(strcmp(data{i}{1}, 'CPU') && str2num(data{i}{2}) > t)
		t = str2num(data{i}{2});
	end
end