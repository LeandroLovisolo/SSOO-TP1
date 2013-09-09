#ifndef __SCHED_FCFS__
#define __SCHED_FCFS__

#include <vector>
#include <queue>
#include <map>
#include "basesched.h"

enum Estado {Corriendo, Bloqueada, Desbloqueandose};

struct tareaActual {
	Estado _estado;
	int _pid;
	tareaActual(Estado estado, int pid) {
		_estado = estado;
		_pid = pid;
	}
};

class SchedFCFS : public SchedBase {
	public:
		SchedFCFS(std::vector<int> argn);
        ~SchedFCFS();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	
	private:
		std::queue<int> tareasEnEspera;
		std::vector<int> tareaActivaCore;
		int pidTareaBloqueada;
		std::vector<tareaActual> tareaActualCore;
		std::map<int, int> tareasBloqueadas;
};

#endif
