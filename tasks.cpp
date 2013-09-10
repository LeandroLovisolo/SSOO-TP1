#include <cstdlib>
#include <ctime>
#include <iostream>

#include "tasks.h"

using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void TaskConsola(int pid, vector<int> params) { // params: n (llamadas), bmin y bmax (tiempo)
	for(int i=0; i<params[0]; i++) {
		int duracion = rand() % (params[2]+1 - params[1]) + params[1];
		uso_IO(pid, duracion);
	}
}

void TaskBatch(int pid, vector<int> params) { // params: total_cpu, cant_bloqueos
	// Tipos de operaciones: 0 = CPU, 1 = IO.
	vector<int> operaciones(params[0], 0); // Inicializo con total_cpu operaciones de tipo CPU.

	// Agrego cant_bloqueos operaciones de IO en posiciones al azar.
	int bloqueos = 0;
	while(bloqueos < params[1]) {
		int i = rand() % params[0];
		if(operaciones[i] == 0) {
			operaciones[i] = 1;
			bloqueos++;
		}
	}

	for(size_t i = 0; i < operaciones.size(); i++) {
		if(operaciones[i] == 0) uso_CPU(pid, 1);
		else uso_IO(pid, 1);
	}
}

void RandomTask(int pid, vector<int> params) { // max_operaciones, max_tiempo_cpu, max_tiempo_io, seed
	srand(params[3]);
	int cantOp = rand() % params[0];
	vector<int> operaciones(cantOp, 0);
	for(int i = 0; i < cantOp; i++) {
		//0 cpu
		if(rand() % 2 == 0) {
			operaciones[i] = 0;
		}
		//1 i/o
		else {
			operaciones[i] = 1;
		}
	}
	for(size_t i = 0; i < operaciones.size(); i++) {
		if(operaciones[i] == 0) uso_CPU(pid, rand() % params[1]);
		else uso_IO(pid, rand() % params[2]);
	}
}

void tasks_init(void) {
	srand(time(0));

	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(RandomTask, 4);
	register_task(TaskBatch, 2);
}
