#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	//Inicializo el vector con los quantum para cada core
	cout << "Cores " << argn[0] << endl;
	cout << "IDLE " << IDLE_TASK << endl;
	for(int i = 1; i <= argn[0]; ++i) {
		cout << "Qunatums " << argn[i] << endl;
		quantumCore.push_back(argn[i]);
		quantumRestanteCore.push_back(argn[i]);
	}
	cout << "Se crea" << endl;
}

SchedRR::~SchedRR() {

}

void SchedRR::load(int pid) {
	//Ver si darle la tarea a los cores que no hacen en vez de encolar
	tareasEnEspera.push(pid);
}

void SchedRR::unblock(int pid) {
	tareasEnEspera.push(pid);
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	int tareaAEjecutar = IDLE_TASK;
	if(m == EXIT || current_pid(cpu) == IDLE_TASK) {
		if(!tareasEnEspera.empty()) {
			tareaAEjecutar = tareasEnEspera.front();
			tareasEnEspera.pop();
		}
		quantumRestanteCore[cpu] = quantumCore[cpu];

	}
	else if(m == TICK) {
		//Si se le termino el quantum, cambio de tarea (si existe otra)
		if(quantumRestanteCore[cpu] == 0) {
			if(!tareasEnEspera.empty()) {
				tareaAEjecutar = tareasEnEspera.front();
				tareasEnEspera.pop();
				tareasEnEspera.push(current_pid(cpu));
			}
			else {
				tareaAEjecutar = current_pid(cpu);
			}
			quantumRestanteCore[cpu] = quantumCore[cpu];
		}
		else {
			quantumRestanteCore[cpu]--;
			tareaAEjecutar = current_pid(cpu);
		}
	}
	else {
		if(!tareasEnEspera.empty()) {
			tareaAEjecutar = tareasEnEspera.front();
			tareasEnEspera.pop();
		}
	}
	return tareaAEjecutar;
}