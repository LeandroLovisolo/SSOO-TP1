#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

/*
Solución: se tiene una cola por core.
La tarea que se está ejecutando está en el frente
de la cola.
Cuando se necesita pasar a la próxima, se desencola, se pone al final de la cola
y se pasa a ejecutar la siguiente.
Para las tareas bloqueadas se utiliza "map" (esto es necesario ya que se debe
conocer de que procesador provenía la tarea bloqueada).

*/

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	//Inicializo el vector con los quantum para cada core;
	for(int i = 1; i <= argn[0]; ++i) {
		colasTareasPorCore.push_back(queue<int>());
		quantumCore.push_back(argn[i]);
		quantumRestanteCore.push_back(argn[i]);
		tareasBloqueadasPorCore.push_back(0);
	}
}

SchedRR2::~SchedRR2() {
}

int SchedRR2::next(int cpu) {
	return 0;
}

void SchedRR2::load(int pid) {
	//Cargo la nueva tarea en el CPU con menos tareas
	unsigned int min = colasTareasPorCore[0].size();
	unsigned int minPos = 0;
	for(int i = 1; i < (int) colasTareasPorCore.size(); ++i) {
		if((colasTareasPorCore[i].size() + tareasBloqueadasPorCore[i]) < min) {
			min = colasTareasPorCore[i].size() + tareasBloqueadasPorCore[i];
			minPos = i;
		}
	}
	colasTareasPorCore[minPos].push(pid);
}

void SchedRR2::unblock(int pid) {
	colasTareasPorCore[tareasBloqueadas[pid]].push(pid);
	--tareasBloqueadasPorCore[tareasBloqueadas[pid]];
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	int tareaACorrer = IDLE_TASK;
	if(m == EXIT) {
		colasTareasPorCore[cpu].pop();
		if(!colasTareasPorCore[cpu].empty()) {
			tareaACorrer = colasTareasPorCore[cpu].front();
		}
		quantumRestanteCore[cpu] = quantumCore[cpu];
	}
	else if(m == TICK) {
		if(quantumRestanteCore[cpu] == 0 || current_pid(cpu) == IDLE_TASK) {
			if(!colasTareasPorCore[cpu].empty()) {
				int tareaAnterior = colasTareasPorCore[cpu].front();
				colasTareasPorCore[cpu].pop();
				colasTareasPorCore[cpu].push(tareaAnterior);
				tareaACorrer = colasTareasPorCore[cpu].front();
				quantumRestanteCore[cpu] = quantumCore[cpu];
			}
		}
		else {
			quantumRestanteCore[cpu]--;
			tareaACorrer = current_pid(cpu);
		}
	}
	else { //Relacionado al BLOCK
		tareasBloqueadas[current_pid(cpu)] = cpu;
		++tareasBloqueadasPorCore[cpu];
		colasTareasPorCore[cpu].pop();
		if(!colasTareasPorCore[cpu].empty()) {
			tareaACorrer = colasTareasPorCore[cpu].front();
		}
	}
	return tareaACorrer;
}
