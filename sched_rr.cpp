#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	//Inicializo tareas activas por core
	tareaActivaCore = vector<int>(argn[0], IDLE_TASK);
	//Inicializo el vector con los quantum para cada core
	for(int i = 1; i < argn[0]; ++i) {
		quantumCore.push_back(argn[i]);
		quantumRestanteCore.push_back(argn[i]);
	}
}

SchedRR::~SchedRR() {

}

void SchedRR::load(int pid) {
	//Ver si darle la tarea a los cores que no hacen en vez de encolar
	tareasEnEspera.push(pid);
}

void SchedRR::unblock(int pid) {
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	if(m == EXIT || tareaActivaCore[cpu] == IDLE_TASK) {
		if(!tareasEnEspera.empty()) {
			tareaActivaCore[cpu] = tareasEnEspera.front();
			tareasEnEspera.pop();
		}
		else {
			tareaActivaCore[cpu] = IDLE_TASK;
		}
		quantumRestanteCore[cpu] = quantumCore[cpu];
	}
	else if(m == TICK) {
		//Si se le termino el quantum, cambio de tarea (si existe otra)
		if(quantumRestanteCore[cpu] == 0) {
			if(!tareasEnEspera.empty()) {
				int tareaNueva = tareasEnEspera.front();
				tareasEnEspera.pop();
				tareasEnEspera.push(tareaActivaCore[cpu]);
				tareaActivaCore[cpu] = tareaNueva;
			}
			quantumRestanteCore[cpu] = quantumCore[cpu];
		}
		else {
			quantumRestanteCore[cpu]--;
		}
	}
	else if(m == BLOCK) {
		//Ver que hacer con el bloqueo
	}
	return tareaActivaCore[cpu];
}