###############################################################################
# Scheduler (Makefile original, salvo target "clean")                         #
###############################################################################

export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_rr2.o sched_lot.o
MAIN=simusched

.PHONY: all
all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
 sched_rr.h sched_rr2.h
sched_fcfs.o: sched_fcfs.cpp sched_fcfs.h basesched.h
sched_rr.o: sched_rr.cpp sched_rr.h basesched.h
sched_rr2.o: sched_rr2.cpp sched_rr2.h basesched.h
shced_lot.o: sched_lot.cpp sched_lot.h basesched.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

.PHONY: clean
clean:
	rm -f $(OBJS) $(MAIN)
	rm -f informe.pdf tex/*.pdf tex/*.aux tex/*.log tex/*.toc

.PHONY: new
new: clean all

###############################################################################
# Informe                                                                     #
###############################################################################

informe.pdf: tex/*.tex
	cd tex; pdflatex -interactive=nonstopmode -halt-on-error informe.tex
	cd tex; pdflatex -interactive=nonstopmode -halt-on-error informe.tex
	cp tex/informe.pdf .