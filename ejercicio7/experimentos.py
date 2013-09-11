#!/usr/bin/env python2
# coding: utf-8

from os import system
from itertools import combinations_with_replacement

MAX_CORES = 4
MAX_QUANTUM = 8

# Ejemplo: experimento(2, [4 8])
# Genera:
#  - experimento-2-4-8.log               Output del simulador
#  - experimento-2-4-8-simulacion.png    Diagrama de Gantt de la simulación
#  - experimento-2-4-8-cores.png         Diagrama de Gantt del uso de los cores
def experimento(cores, quantums):
	quantums = map(str, quantums)
	filename = "experimento-%d-%s" % (cores, "-".join(quantums))

	# Corro el simulador
	system("../simusched tareas.tsk %d 1 2 SchedRR %d %s > resultados/%s.log" % \
		(cores, cores, " ".join(quantums), filename))

	# # Genero los diagramas Gantt de la simulación
	# system("cat resultados/%s.log | ../graphsched.py > resultados/%s-simulacion.png" % \
	# 	(filename, filename))

	# # Genero los diagramas Gantt del uso de cores
	# system("cat resultados/%s.log | ../graph_cores.py" % filename)
	# system("mv out_cores_timeline.png resultados/%s-cores.png" % filename)

def main():
	# Recorro todas las combinaciones posibles de cores y quantums, sin repetidos.
	# (Ej.: Si ya se evaluaron los quantums [2 2 4] para cantidad de cores igual a 3,
	# no se evalúan los quantums [2 4 2] pues ambas configuraciones son equivalentes.)
	for cores in range(1, MAX_CORES + 1):
		for quantums in combinations_with_replacement(range(1, MAX_QUANTUM + 1), cores):
			#print list(quantums)
			experimento(cores, list(quantums))


if __name__ == "__main__":
	main()