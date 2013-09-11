#ifndef __SCHED_LOT__
#define __SCHED_LOT__

#include <vector>
#include <map>
#include <list>
#include "basesched.h"

struct tarea {
			int pid;
			int tickets;
		};

class SchedLottery : public SchedBase {
	public:
		SchedLottery(std::vector<int> argn);
        ~SchedLottery();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	
	private:
		std::list<tarea>::iterator lottery(int semilla);
		
		std::list<tarea> tareas;
		std::map<int, int> tareasBloqueadas;
		std::vector<tarea> tareasEnEjecucion;
		std::vector<int> quantumRestanteCore;
		int sysQuantum;
		int systemTickets;
		int systemSeed;
};

#endif
