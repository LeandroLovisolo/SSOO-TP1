#include <vector>
#include <queue>
#include "sched_lot.h"

using namespace std;

// El constructor recibe la cantidad de cores, el quantum y la semilla de la secuencia pseudo aleatoria.
SchedLottery::SchedLottery(std::vector<int> argn) {
	systemTickets = 0;					// Inicializo los tickets del sistema en 0.
	sysQuantum = argn[1];
	systemSeed = argn[2];
	int cores = argn[0];
	// Seteo el quantum restante igual al inicial.
	for(int i = 0; i < cores; i++) {
		quantumRestanteCore[i] = sysQuantum;
	}
}

SchedLottery::~SchedLottery() {
}

void SchedLottery::load(int pid) {
	struct tarea nueva;
	nueva.pid = pid;
	nueva.tickets = 1;
	tareas.push_back(nueva);
	systemTickets++;
}

void SchedLottery::unblock(int pid) {
	struct tarea desbloqueada;
	desbloqueada.pid = pid;
	desbloqueada.tickets = tareasBloqueadas[pid];
	tareas.push_back(desbloqueada);
	systemTickets+=(desbloqueada.tickets);
}

int SchedLottery::tick(int cpu, const enum Motivo m) {
	int tareaACorrer = IDLE_TASK;
	if(m == EXIT) {
		if(!tareas.empty()) {
			list<tarea>::iterator tareaElegida = lottery(systemSeed);
			tareasEnEjecucion[cpu-1] = *tareaElegida;
			tareaACorrer = tareaElegida->pid;
			tareas.erase(tareaElegida);
			systemTickets-=(tareasEnEjecucion[cpu-1]).tickets;
			(tareasEnEjecucion[cpu-1]).tickets = 1;
		}
		quantumRestanteCore[cpu] = sysQuantum;
	}
	else if(m == TICK) {
		if(quantumRestanteCore[cpu] == 0) {
			if(!tareas.empty()) {
				// Sacarla de ejecucion y reinsertarla en la lista.
				tareas.push_back(tareasEnEjecucion[cpu]);
				systemTickets+=tareas.back().tickets;
				// Elegir una tarea nueva por lottery, ponerla a ejecutar y sacarla de la lista.
				list<tarea>::iterator tareaElegida = lottery(systemSeed);
				tareasEnEjecucion[cpu-1] = *tareaElegida;
				tareaACorrer = tareaElegida->pid;
				tareas.erase(tareaElegida);
				systemTickets-=(tareasEnEjecucion[cpu-1]).tickets;
				(tareasEnEjecucion[cpu-1]).tickets = 1;
				
				quantumRestanteCore[cpu] = sysQuantum;
			}
		}
		else if(current_pid(cpu) == IDLE_TASK && !tareas.empty()) {
			list<tarea>::iterator tareaElegida = lottery(systemSeed);
			tareasEnEjecucion[cpu-1] = *tareaElegida;
			tareaACorrer = tareaElegida->pid;
			tareas.erase(tareaElegida);
			systemTickets-=(tareasEnEjecucion[cpu-1]).tickets;
			(tareasEnEjecucion[cpu-1]).tickets = 1;
			
			quantumRestanteCore[cpu] = sysQuantum;
		}
		else {
			quantumRestanteCore[cpu]--;
			tareaACorrer = current_pid(cpu);
		}
	}
	else { //Relacionado al BLOCK
		// Compensamos a la tarea por consumir menos de su quantum.
		tareasBloqueadas[current_pid(cpu)] = sysQuantum/quantumRestanteCore[cpu];
		if(!tareas.empty()) {
			list<tarea>::iterator tareaElegida = lottery(systemSeed);
			tareasEnEjecucion[cpu-1] = *tareaElegida;
			tareaACorrer = tareaElegida->pid;
			tareas.erase(tareaElegida);
			systemTickets-=(tareasEnEjecucion[cpu-1]).tickets;
			(tareasEnEjecucion[cpu-1]).tickets = 1;
		}
		quantumRestanteCore[cpu] = sysQuantum;
	}
	return tareaACorrer;
}

std::list<tarea>::iterator SchedLottery::lottery(int semilla) {
	return tareas.begin();
}