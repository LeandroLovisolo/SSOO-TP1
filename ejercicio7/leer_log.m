function A = leer_log(filename)

A = {};
fid = fopen(filename, 'r');
while true
	line = fgetl(fid);
	if line == -1; break; end;
	A{end + 1} = cellstr(strsplit(line, ' '));
end
fclose(fid);