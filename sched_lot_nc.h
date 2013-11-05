#ifndef __SCHED_LOTNC__
#define __SCHED_LOTNC__

#include <vector>
#include <map>
#include <list>
#include "basesched.h"

struct tareaNC {
			int pid;
			int tickets;
		};

class SchedLotteryNC : public SchedBase {
	public:
		SchedLotteryNC(std::vector<int> argn);
        ~SchedLotteryNC();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	
	private:
		std::list<tareaNC>::iterator lottery();
		unsigned int random_gen();
		
		std::list<tareaNC> tareas;
		std::map<int, int> tareasBloqueadas;
		std::vector<tareaNC> tareasEnEjecucion;
		std::vector<int> quantumRestanteCore;
		std::vector<int> sysQuantumCore;
		int systemTickets;
		int systemSeed;
};

#endif
