#include <vector>
#include <queue>
#include "sched_fcfs.h"

using namespace std;

/*Solución: Tengo una cola por core y al llegar tareas las voy poniendo
en la cola con menos tareas
*/
// FCFS recibe la cantidad de cores.
SchedFCFS::SchedFCFS(vector<int> argn) {
	//Creo una lista para cada core
	for(int i = 0; i < argn[0]; ++i) {
		queue<int>* nuevaCola = new queue<int>();
		colasPorCore.push_back(nuevaCola);
	}
}

SchedFCFS::~SchedFCFS() {
	for (unsigned int i = 0; i < colasPorCore.size(); ++i) {
		delete colasPorCore[i];
	}
}

void SchedFCFS::load(int pid) {
	//Le doy el proceso al core con menos tareas
	unsigned int min = colasPorCore[0]->size(), minPos = 0;
	for(unsigned int i = 1; i < colasPorCore.size(); ++i) {
		if(colasPorCore[i]->size() > min) {
			min = colasPorCore[i]->size();
			minPos = i;
		}
	}
	colasPorCore[minPos]->push(pid);
}

void SchedFCFS::unblock(int pid) {
	//Aparentemente deberíamos esperar hasta que se desbloquee, ya que es FIFO
}

int SchedFCFS::tick(int cpu, const enum Motivo m) {
	if(m == TICK) {
		if(colasPorCore[cpu]->size() == 0) return IDLE_TASK;
	}
	if(m == EXIT) {
		colasPorCore[cpu]->pop();
		if(colasPorCore[cpu]->size() == 0) return IDLE_TASK;
		return colasPorCore[cpu]->front();
	}
	else {
		return colasPorCore[cpu]->front();
	}

}