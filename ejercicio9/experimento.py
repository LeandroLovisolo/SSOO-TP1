#!/usr/bin/env python2
# encoding: utf-8

import csv
from random import randint
from subprocess import check_output

cant_tareas      = 4
ticks_observados = 100
lote             = "tareas.tsk" # Lote con cant_tares tareas de tipo TaskCPU de igual duración
cores            = 1
contexto         = 0
migracion        = 0
quantums         = [5]
iteraciones      = 1000

def leer_ticks(output):
    ticks = [0 for i in range(0, cant_tareas)]

    for line in output.splitlines():
        line = line.split(" ")
        
        if line[0] == "CPU":
            if int(line[1]) >= ticks_observados:
                break
            ticks[int(line[2])] += 1

    return ticks

sums = [0 for i in range(0, cant_tareas)]

with open('experimento.csv', 'w') as csvfile:
    writer = csv.writer(csvfile)

    for i in range(1, iteraciones + 1):
        # Ejecuto el simulador
        seed = randint(0, 999999)
        command = "../simusched %s %d %d %d SchedLottery %s %d" % \
            (lote, cores, contexto, migracion, " ".join(map(str, quantums)), seed)
        output = check_output(command, shell=True)

        # Leo los ticks por proceso entre los primeros 100 ticks ejecutados
        ticks = leer_ticks(output)

        # Acumulo el total de ticks por proceso
        for j in range(0, cant_tareas):
            sums[j] += ticks[j]

        # Promedio
        promedio = map(lambda x: float(x) / i, sums)

        # Escribo el promedio de ticks por proceso
        writer.writerow(promedio)