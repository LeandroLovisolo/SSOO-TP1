#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"


class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	private:
		std::queue<int> tareasEnEspera;
		std::vector<int> quantumCore;
		std::vector<int> quantumRestanteCore;
		int next(int cpu);
};

#endif
