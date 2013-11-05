#include <vector>
#include <queue>
#include <stdlib.h>
#include "sched_lot_nc.h"

using namespace std;

// El constructor recibe la cantidad de cores, el quantum y la semilla de la secuencia pseudo aleatoria.
SchedLotteryNC::SchedLotteryNC(std::vector<int> argn) {
	systemTickets = 0;				// Inicializo los tickets del sistema en 0.
	int cores = argn[0];
	systemSeed = argn[cores+1];
	tareaNC idle = {IDLE_TASK, 0};
	
	for(int i = 0; i < cores; i++) {
		sysQuantumCore.push_back(argn[i+1]);
		quantumRestanteCore.push_back(argn[i+1]);
		tareasEnEjecucion.push_back(idle);
	}
}

SchedLotteryNC::~SchedLotteryNC() {
}

void SchedLotteryNC::load(int pid) {
	tareaNC nueva = {pid, 1};
	tareas.push_back(nueva);
	systemTickets++;
}

void SchedLotteryNC::unblock(int pid) {
	tareaNC desbloqueada = {pid, tareasBloqueadas[pid]};
	tareas.push_back(desbloqueada);
	systemTickets+=(desbloqueada.tickets);
}

int SchedLotteryNC::tick(int cpu, const enum Motivo m) {
	int tareaACorrer = IDLE_TASK;
	if(m == EXIT) {
		if(!tareas.empty()) {
			list<tareaNC>::iterator tareaElegida = lottery();
			tareasEnEjecucion[cpu] = *tareaElegida;
			tareaACorrer = tareaElegida->pid;
			tareas.erase(tareaElegida);
			systemTickets-=(tareasEnEjecucion[cpu]).tickets;
			(tareasEnEjecucion[cpu]).tickets = 1;
		}
		quantumRestanteCore[cpu] = sysQuantumCore[cpu];
	}
	else if(m == TICK) {
		quantumRestanteCore[cpu]--;
		if(quantumRestanteCore[cpu] == 0) {
			if(!tareas.empty()) {
				// Sacarla de ejecucion y reinsertarla en la lista.
				tareas.push_back(tareasEnEjecucion[cpu]);
				systemTickets+=tareas.back().tickets;
				// Elegir una tarea nueva por lottery, ponerla a ejecutar y sacarla de la lista.
				list<tareaNC>::iterator tareaElegida = lottery();
				tareasEnEjecucion[cpu] = *tareaElegida;
				tareaACorrer = tareaElegida->pid;
				tareas.erase(tareaElegida);
				systemTickets-=(tareasEnEjecucion[cpu]).tickets;
				(tareasEnEjecucion[cpu]).tickets = 1;
				
				quantumRestanteCore[cpu] = sysQuantumCore[cpu];
			} else {
				quantumRestanteCore[cpu] = sysQuantumCore[cpu];
				tareaACorrer = current_pid(cpu);
			}
		}
		else if(current_pid(cpu) == IDLE_TASK && !tareas.empty()) {
			list<tareaNC>::iterator tareaElegida = lottery();
			tareasEnEjecucion[cpu] = *tareaElegida;
			tareaACorrer = tareaElegida->pid;
			tareas.erase(tareaElegida);
			systemTickets-=(tareasEnEjecucion[cpu]).tickets;
			(tareasEnEjecucion[cpu]).tickets = 1;
			
			quantumRestanteCore[cpu] = sysQuantumCore[cpu];
		}
		else {
			tareaACorrer = current_pid(cpu);
		}
	}
	else { //Relacionado al BLOCK
		quantumRestanteCore[cpu]--;
		// Compensamos a la tarea por consumir menos de su quantum.
		tareasBloqueadas[current_pid(cpu)] = 1;
		if(!tareas.empty()) {
			list<tareaNC>::iterator tareaElegida = lottery();
			tareasEnEjecucion[cpu] = *tareaElegida;
			tareaACorrer = tareaElegida->pid;
			tareas.erase(tareaElegida);
			systemTickets-=(tareasEnEjecucion[cpu]).tickets;
			(tareasEnEjecucion[cpu]).tickets = 1;
		}
		quantumRestanteCore[cpu] = sysQuantumCore[cpu];
	}
	return tareaACorrer;
}

list<tareaNC>::iterator SchedLotteryNC::lottery() {
	int sorteado = random_gen() % systemTickets;
	int suma = 0;
	
	// Recorro la lista de tareas y devuelvo el iterador a la elegida.
	list<tareaNC>::iterator it = tareas.begin();
	while (suma < sorteado) {
		suma += it->tickets;
		if (suma > sorteado) break;
		it++;
	}
	
	return it;
}

unsigned int SchedLotteryNC::random_gen() {
	unsigned int hi, lo;
	
	lo = 16807 * (systemSeed & 0xFFFF);
	hi = 16807 * (systemSeed >> 16);
	
	lo += (hi & 0x7FFF) << 16;
	lo += hi >> 15;
	
	if (lo > 0x7FFFFFFF) lo -= 0x7FFFFFFF;
	
	return (systemSeed = lo);
}