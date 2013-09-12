#!/usr/bin/env python2
# coding: utf-8

import subprocess
from random import randint

tarea     = "tarea1.tsk"
cores     = 1
contexto  = 0
migracion = 0
quantums  = [2]

csv = open("Lot.csv", "w+")

for i in range(1, 1000):
	seed = randint(0, 900000)

	command = "../simusched %s %d %d %d SchedLottery %s %d" % \
		(tarea, cores, contexto, migracion, " ".join(map(str, quantums)), seed)

	print "Experimento %d: %s" % (i, command)

	output = subprocess.check_output(command, shell=True).split('\n')
	
	primer_exit = None
	ultimo_exit = None
	
	for line in output:
		line = line.split(" ")
		if line[0] == "EXIT":
			ultimo_exit = int(line[1])
			if(primer_exit == None):
				primer_exit = ultimo_exit

	csv.write(str(ultimo_exit - primer_exit) + '\n')

csv.close()