#!/usr/bin/env python2
# coding: utf-8

import subprocess
from random import randint

RRlist = list()
RR2list = list()
quantum = 100
cores = 2
switchCost = 5
migrateCost = 10
tarea = "tarea1.tsk"
RRf = open("RRtest2.csv", "w+")
RR2f = open("RR2test2.csv", "w+")

for migCost in range(0, 400, 5):
	valuesRR = list()
	valuesRR2 = list()
	for j in range(0,50):
		tareaF = open(tarea, "w")
		tareaF.write("@0:\n")
		tareaF.write("RandomTask 4 100 150 " + str(randint(1,5000)) + "\n")
		tareaF.write("RandomTask 2 80 130 " + str(randint(1,5000)) + "\n")
		tareaF.write("RandomTask 3 110 50 " + str(randint(1,5000)) + "\n")
		tareaF.write("RandomTask 4 50 100 " + str(randint(1,5000)) + "\n")
		tareaF.close()

		output = subprocess.check_output("./simusched "+ tarea + " " + str(cores) +
			" " + str(switchCost) + " " + str(migrateCost) + " " + "SchedRR " +
			str(quantum) + " " + str(quantum), shell=True)

		outputLines = output.split('\n')
		valuesRR.append(int(outputLines[len(outputLines)-2][4:-5]))

		output = subprocess.check_output("./simusched "+ tarea+ " " + str(cores) +
			" " + str(switchCost) + " " + str(migrateCost) + " " + "SchedRR2 " +
			str(quantum) + " " + str(quantum), shell=True)

		outputLines = output.split('\n')
		valuesRR2.append(int(outputLines[len(outputLines)-2][4:-5]))
	RRf.write(str(sum(valuesRR)/len(valuesRR)) + ', ')
	RR2f.write(str(sum(valuesRR2)/len(valuesRR2)) + ', ')
RRf.close()
RR2f.close()