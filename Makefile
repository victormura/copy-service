compile:
	@[ -d build/. ] || mkdir build
	@gcc -c lib/cancel.c -o ./build/cancel.o
	@gcc -c lib/createjob.c -o ./build/createjob.o
	@gcc -c lib/freestats.c -o ./build/freestats.o
	@gcc -c lib/listjobs.c -o ./build/listjobs.o
	@gcc -c lib/pause.c -o ./build/pause.o
	@gcc -c lib/progress.c -o ./build/progress.o
	@gcc -c lib/stats.c -o ./build/stats.o
	@gcc -c main.c -o ./build/main.o
	@gcc ./build/cancel.o ./build/createjob.o ./build/freestats.o ./build/listjobs.o ./build/pause.o ./build/progress.o ./build/stats.o ./build/main.o -o runner

run:
	@./runner

.DEFAULT_GOAL=run