#ifndef __SCHED_LOT__
#define __SCHED_LOT__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedLot : public SchedBase {
	public:
		SchedLot(std::vector<int> argn);
        ~SchedLot();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	
	private:

};

#endif
