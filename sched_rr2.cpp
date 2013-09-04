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
	cout << "Creando nuevo sched" << endl;
	for(int i = 1; i <= argn[0]; ++i) {
		colasTareasPorCore.push_back(queue<int>());
		quantumCore.push_back(argn[i]);
		quantumRestanteCore.push_back(argn[i]);
		tareasBloqueadasPorCore.push_back(0);
		cout << "Quantum " << argn[i] << endl;
	}
	cout << "Size colas " << colasTareasPorCore.size() << endl;
	cout << "Tareas size " << tareasBloqueadasPorCore.size() << endl;
}

SchedRR2::~SchedRR2() {
	cout << "Borrado de RR2" << endl;
	//for(unsigned int i = 0; i < colasTareasPorCore.size(); ++i) {
	//	delete colasTareasPorCore[i];
	//}
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
	cout << "Tarea " << pid << " cargada en cpu " << minPos << endl;
}

void SchedRR2::unblock(int pid) {
	cout << "Se desbloquea la tarea " << pid << endl;
	colasTareasPorCore[tareasBloqueadas[pid]].push(pid);
	--tareasBloqueadasPorCore[tareasBloqueadas[pid]];
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
	int tareaACorrer = IDLE_TASK;
	if(m == EXIT) {
		cout << "Termino tarea " << current_pid(cpu) << " CPU " << cpu << endl;
		colasTareasPorCore[cpu].pop();
		if(!colasTareasPorCore[cpu].empty()) {
			tareaACorrer = colasTareasPorCore[cpu].front();
		}
		quantumRestanteCore[cpu] = quantumCore[cpu];
	}
	else if(m == TICK) {
		cout << "Tick tarea "<< current_pid(cpu) << " CPU " << cpu << endl;
		if(quantumRestanteCore[cpu] == 0 || current_pid(cpu) == IDLE_TASK) {
			if(quantumRestanteCore[cpu] == 0) {
				cout << "Termina el quatum " << endl;
			}
			if(!colasTareasPorCore[cpu].empty()) {
				cout << "Hay más espacio en la cola" << endl;
				int tareaAnterior = colasTareasPorCore[cpu].front();
				colasTareasPorCore[cpu].pop();
				colasTareasPorCore[cpu].push(tareaAnterior);
				tareaACorrer = colasTareasPorCore[cpu].front();
				quantumRestanteCore[cpu] = quantumCore[cpu];
			}
		}
		else {
			cout << "Se descuenta el quantum" << endl;
			quantumRestanteCore[cpu]--;
			tareaACorrer = current_pid(cpu);
		}
	}
	else { //Relacionado al BLOCK
		cout << "Tarea " << current_pid(cpu) << " bloqueada, CPU " << cpu << endl;
		tareasBloqueadas[current_pid(cpu)] = cpu;
		++tareasBloqueadasPorCore[cpu];
		colasTareasPorCore[cpu].pop();
		if(!colasTareasPorCore[cpu].empty()) {
			tareaACorrer = colasTareasPorCore[cpu].front();
		}
	}
	return tareaACorrer;
}
