compile:
	@[ -d build/. ] || mkdir build
	@gcc -c lib/init.c -o ./build/init.o
	@gcc -c lib/cancel.c -o ./build/cancel.o
	@gcc -c lib/createjob.c -o ./build/createjob.o
	@gcc -c lib/listjobs.c -o ./build/listjobs.o
	@gcc -c lib/pause.c -o ./build/pause.o
	@gcc -c lib/progress.c -o ./build/progress.o
	@gcc -c lib/stats.c -o ./build/stats.o
	@gcc -c main.c -o ./build/main.o
	@gcc ./build/init.o ./build/cancel.o ./build/createjob.o ./build/listjobs.o ./build/pause.o ./build/progress.o ./build/stats.o ./build/main.o -o runner -pthread

run: compile
	@./runner

test_compile: clear_tests compile
	@[ -d build/tests/. ] || mkdir build/tests/
	@[ -d tests/dst/. ] || mkdir tests/dst/
	@gcc -c ./tests/createjob.c -o ./build/tests/createjob.o
	@gcc -c ./tests/job_list.c -o ./build/tests/job_list.o
	@gcc ./build/init.o ./build/cancel.o ./build/createjob.o ./build/listjobs.o ./build/pause.o ./build/progress.o ./build/stats.o ./build/tests/createjob.o -o ./build/tests/test_createjob.o -pthread
	@gcc ./build/init.o ./build/cancel.o ./build/createjob.o ./build/listjobs.o ./build/pause.o ./build/progress.o ./build/stats.o ./build/tests/job_list.o -o ./build/tests/test_job_list.o -pthread

test_create_job: test_compile
	@./build/tests/test_createjob.o

test_list_job: test_compile
	@./build/tests/test_job_list.o

clear_tests:
	@rm -rf ./tests/dst
.DEFAULT_GOAL=run