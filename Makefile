JOBS?=4

exec: simulator
	aprun -n ${JOBS} ./simulator

simulator: simulator.c
	cc simulator.c -o simulator
