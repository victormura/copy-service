compile:
	@[ -d build/. ] || mkdir build
	@gcc -c lib/helpers.c -o ./build/helpers.o
	@gcc -c lib/cancel.c -o ./build/cancel.o
	@gcc -c lib/createjob.c -o ./build/createjob.o
	@gcc -c lib/listjobs.c -o ./build/listjobs.o
	@gcc -c lib/pause.c -o ./build/pause.o
	@gcc -c lib/resume.c -o ./build/resume.o
	@gcc -c lib/progress.c -o ./build/progress.o
	@gcc -c lib/stats.c -o ./build/stats.o
	@gcc -c main.c -o ./build/main.o
	@gcc ./build/helpers.o ./build/cancel.o ./build/createjob.o ./build/listjobs.o ./build/pause.o ./build/resume.o ./build/progress.o ./build/stats.o ./build/main.o -o runner -pthread

run: compile
	@./runner

test_compile: clear_tests compile
	@[ -d build/tests/. ] || mkdir build/tests/
	@[ -d tests/dst/. ] || mkdir tests/dst/
	@gcc -c ./tests/test_createjob.c -o ./build/tests/test_createjob.o
	@gcc -c ./tests/test_listjobs.c -o ./build/tests/test_listjobs.o
	@gcc -c ./tests/test_pause_resume.c -o ./build/tests/test_pause_resume.o
	@gcc ./build/helpers.o ./build/cancel.o ./build/createjob.o ./build/listjobs.o ./build/pause.o ./build/resume.o ./build/progress.o ./build/stats.o ./build/tests/test_createjob.o -o ./build/tests/test_createjob -pthread
	@gcc ./build/helpers.o ./build/cancel.o ./build/createjob.o ./build/listjobs.o ./build/pause.o ./build/resume.o ./build/progress.o ./build/stats.o ./build/tests/test_listjobs.o -o ./build/tests/test_listjobs -pthread
	@gcc ./build/helpers.o ./build/cancel.o ./build/createjob.o ./build/listjobs.o ./build/pause.o ./build/resume.o ./build/progress.o ./build/stats.o ./build/tests/test_pause_resume.o -o ./build/tests/test_pause_resume -pthread

test_create_job: test_compile
	@./build/tests/test_createjob

test_list_job: test_compile
	@./build/tests/test_listjobs

test_resume: test_compile 
	@./build/tests/test_pause_resume

test: 
	@make test_create_job
	@make test_list_job
	@make test_resume

clear_tests:
	@rm -rf ./tests/dst

.DEFAULT_GOAL=run