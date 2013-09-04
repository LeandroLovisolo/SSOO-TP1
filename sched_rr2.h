#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <map>
#include <queue>
#include "basesched.h"


class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	private:
		std::vector<int> quantumCore;
		std::vector<int> quantumRestanteCore;
		std::vector<int> tareasBloqueadasPorCore;
		std::vector<std::queue<int> > colasTareasPorCore;
		std::map<int, int> tareasBloqueadas;
		int next(int cpu);
};

#endif
