#!/usr/bin/env python2
# coding: utf-8

import subprocess
from random import randint

RRlist = list()
RR2list = list()
quantum = 10
cores = 1
switchCost = 0
migrateCost = 0
tarea = "tarea1.tsk"
RRf = open("Lot.csv", "w+")
valuesRR = list()
for i in range(1, 10000):
	finalizacion = list()
	output = subprocess.check_output("./simusched "+ tarea + " " + str(cores) +
		" " + str(switchCost) + " " + str(migrateCost) + " " + "SchedLottery " +
		str(quantum) + " " + str(randint(0, 900000)), shell=True)
	outputLines = output.split('\n')
	for line in outputLines:
		if line.find("EXIT") != -1:
			finalizacion.append(int(line[5:-4]))
	valuesRR.append(finalizacion[-1] - finalizacion[0])
	RRf.write(str(sum(valuesRR)/len(valuesRR)) + ', ')
RRf.close()