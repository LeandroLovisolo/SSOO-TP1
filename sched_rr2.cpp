#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	//Inicializo el vector con los quantum para cada core
	for(int i = 1; i < argn[0]; ++i) {
		colasTareasPorCore.push_back(new queue<int>());
		quantumCore.push_back(argn[i]);
		quantumRestanteCore.push_back(argn[i]);
	}
}

SchedRR2::~SchedRR2() {
	for(unsigned int i = 0; i < colasTareasPorCore.size(); ++i) {
		delete colasTareasPorCore[i];
	}
}

int SchedRR2::next(int cpu) {
	return 0;
}

void SchedRR2::load(int pid) {
	//Cargo la nueva tarea en el CPU con menos tareas
	unsigned int min = colasTareasPorCore[0]->size();
	unsigned int minPos = 0;
	for(unsigned int i = 1; i < colasTareasPorCore.size(); ++i) {
		if(colasTareasPorCore[i]->size() < min) {
			min = colasTareasPorCore[i]->size();
			minPos = i;
		}
	}
	colasTareasPorCore[minPos]->push(pid);
}

void SchedRR2::unblock(int pid) {
	colasTareasPorCore[tareasBloqueadas[pid]]->push(pid);
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	int tareaACorrer = IDLE_TASK;
	if(m == EXIT) {
		colasTareasPorCore[cpu]->pop();
		if(!colasTareasPorCore[cpu]->empty()) {
			tareaACorrer = colasTareasPorCore[cpu]->front();
		}
		quantumRestanteCore[cpu] = quantumCore[cpu];
	}
	else if(m == TICK) {
		if(quantumRestanteCore[cpu] == 0) {
			if(!colasTareasPorCore[cpu]->empty()) {
				int tareaACorrer = colasTareasPorCore[cpu]->front();
				colasTareasPorCore[cpu]->pop();
				quantumRestanteCore[cpu] = quantumCore[cpu];
			}
		}
		else {
			quantumRestanteCore[cpu]--;
			return current_pid(cpu);
		}
	}
	else { //Relacionado al block FALTA
		tareasBloqueadas[pid] = cpu;
		return current_pid(cpu);
	}
}
