#include <vector>
#include <queue>
#include "sched_fcfs.h"

using namespace std;

/* Solucion: Una cola FIFO para los dos cores, cada uno agarra una tarea
y no la suelta hasta terminarla. Cuando termina saca otra de la cola.
*/

// FCFS recibe la cantidad de cores.
SchedFCFS::SchedFCFS(vector<int> argn) {
	//Creo una lista para cada core
	tareaActivaCore = vector<int>(argn[0], IDLE_TASK);
}

SchedFCFS::~SchedFCFS() {
}

void SchedFCFS::load(int pid) {
	//Recorro los cores para ver si tienen alguna tarea activa, si tienen
	//pusheo la tarea a la cola, si no, la pongo como activa
	bool seEncolaTarea = true;
	for(unsigned int i = 1; i < tareaActivaCore.size(); ++i) {
		if(tareaActivaCore[i] == IDLE_TASK) {
			tareaActivaCore[i] = pid;
			seEncolaTarea = false;
		}
	}
	if(seEncolaTarea) {
		tareasEnEspera.push(pid);
	}
}

void SchedFCFS::unblock(int pid) {
	//Aparentemente deberíamos esperar hasta que se desbloquee, ya que es FIFO
}

int SchedFCFS::tick(int cpu, const enum Motivo m) {
	if(m == EXIT) {
		if(!tareasEnEspera.empty()) {
			tareaActivaCore[cpu] = tareasEnEspera.front();
			tareasEnEspera.pop();
		}
		else {
			tareaActivaCore[cpu] = IDLE_TASK;
		}
	}
	return tareaActivaCore[cpu];

}