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
	tarea idle = {IDLE_TASK, 0};
	// Seteo el quantum restante igual al inicial.
	for(int i = 0; i < cores; i++) {
		quantumRestanteCore.push_back(sysQuantum);
		tareasEnEjecucion.push_back(idle);
	}
}

SchedLottery::~SchedLottery() {
}

void SchedLottery::load(int pid) {
	tarea nueva = {pid, 1};
	tareas.push_back(nueva);
	systemTickets++;
}

void SchedLottery::unblock(int pid) {
	tarea desbloqueada = {pid, tareasBloqueadas[pid]};
	tareas.push_back(desbloqueada);
	systemTickets+=(desbloqueada.tickets);
}

int SchedLottery::tick(int cpu, const enum Motivo m) {
	int tareaACorrer = IDLE_TASK;
	if(m == EXIT) {
		if(!tareas.empty()) {
			list<tarea>::iterator tareaElegida = lottery(systemSeed);
			tareasEnEjecucion[cpu] = *tareaElegida;
			tareaACorrer = tareaElegida->pid;
			tareas.erase(tareaElegida);
			systemTickets-=(tareasEnEjecucion[cpu]).tickets;
			(tareasEnEjecucion[cpu]).tickets = 1;
		}
		quantumRestanteCore[cpu] = sysQuantum;
	}
	else if(m == TICK) {
		quantumRestanteCore[cpu]--;
		if(quantumRestanteCore[cpu] == 0) {
			if(!tareas.empty()) {
				// Sacarla de ejecucion y reinsertarla en la lista.
				tareas.push_back(tareasEnEjecucion[cpu]);
				systemTickets+=tareas.back().tickets;
				// Elegir una tarea nueva por lottery, ponerla a ejecutar y sacarla de la lista.
				list<tarea>::iterator tareaElegida = lottery(systemSeed);
				tareasEnEjecucion[cpu] = *tareaElegida;
				tareaACorrer = tareaElegida->pid;
				tareas.erase(tareaElegida);
				systemTickets-=(tareasEnEjecucion[cpu]).tickets;
				(tareasEnEjecucion[cpu]).tickets = 1;
				
				quantumRestanteCore[cpu] = sysQuantum;
			} else {
				quantumRestanteCore[cpu] = sysQuantum;
				tareaACorrer = current_pid(cpu);
			}
		}
		else if(current_pid(cpu) == IDLE_TASK && !tareas.empty()) {
			list<tarea>::iterator tareaElegida = lottery(systemSeed);
			tareasEnEjecucion[cpu] = *tareaElegida;
			tareaACorrer = tareaElegida->pid;
			tareas.erase(tareaElegida);
			systemTickets-=(tareasEnEjecucion[cpu]).tickets;
			(tareasEnEjecucion[cpu]).tickets = 1;
			
			quantumRestanteCore[cpu] = sysQuantum;
		}
		else {
			tareaACorrer = current_pid(cpu);
		}
	}
	else { //Relacionado al BLOCK
		quantumRestanteCore[cpu]--;
		// Compensamos a la tarea por consumir menos de su quantum.
		tareasBloqueadas[current_pid(cpu)] = sysQuantum/(sysQuantum-quantumRestanteCore[cpu]);
		if(!tareas.empty()) {
			list<tarea>::iterator tareaElegida = lottery(systemSeed);
			tareasEnEjecucion[cpu] = *tareaElegida;
			tareaACorrer = tareaElegida->pid;
			tareas.erase(tareaElegida);
			systemTickets-=(tareasEnEjecucion[cpu]).tickets;
			(tareasEnEjecucion[cpu]).tickets = 1;
		}
		quantumRestanteCore[cpu] = sysQuantum;
	}
	return tareaACorrer;
}

list<tarea>::iterator SchedLottery::lottery(int semilla) {
	return tareas.begin();
}