#include <vector>
#include <queue>
#include "sched_fcfs.h"
#include <iostream>

using namespace std;

/* Solucion: Una cola FIFO para los dos cores, cada uno agarra una tarea
y no la suelta hasta terminarla. Cuando termina saca otra de la cola.
*/

// FCFS recibe la cantidad de cores.
SchedFCFS::SchedFCFS(vector<int> argn) {
	for(int i=0; i < argn[0]; i++) {
		tareaActualCore.push_back(tareaActual(Corriendo, IDLE_TASK));
	}
}

SchedFCFS::~SchedFCFS() {
}

void SchedFCFS::load(int pid) {
	tareasEnEspera.push(pid);
}

void SchedFCFS::unblock(int pid) {
	tareaActualCore[tareasBloqueadas[pid]]._estado = Desbloqueandose;
}

int SchedFCFS::tick(int cpu, const enum Motivo m) {
	int tareaAEjecutar = IDLE_TASK;
	//Si la tarea terminó
	//Si la tarea era IDLE_TASK y estaba corriendo, es decir, no había ninguna tarea
	//y se agregó una nueva, cargarla.
	if(m == EXIT || (current_pid(cpu) == IDLE_TASK && tareaActualCore[cpu]._estado == Corriendo)) {
		if(!tareasEnEspera.empty()) {
			tareaAEjecutar = tareasEnEspera.front();
			tareasEnEspera.pop();
		}
	}
	//La tarea está corriendo
	else if(m == TICK && (tareaActualCore[cpu]._estado == Corriendo)) {
		tareaAEjecutar = current_pid(cpu); //Seguir corriendola
	}
	//La tarea estuvo bloqueada, desbloquearla
	else if(m == TICK && (tareaActualCore[cpu]._estado == Desbloqueandose)) {
		tareaActualCore[cpu]._estado = Corriendo; //Cambio el estado de la tarea actual
		tareaAEjecutar = tareaActualCore[cpu]._pid; //Pongo a ejecutar la tarea desbloqueada
	}
	else if(m == BLOCK){ //La tarea se bloqueó
		tareaActualCore[cpu]._estado = Bloqueada; //La tarea actual pasa a estar bloqueada
		tareaActualCore[cpu]._pid = current_pid(cpu); //Se guarda la tarea que se bloqueó
		tareasBloqueadas[current_pid(cpu)] = cpu;
		//Se deja la tarea IDLE_TASK mientras está bloqueada
	}
	return tareaAEjecutar;

}