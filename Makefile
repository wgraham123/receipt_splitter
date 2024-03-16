COMPILER=gcc

project: helloworld
	@$(COMPILER) -c src/helloworld/*.c -Iinclude/ -o bin/helloworld.o
	@$(COMPILER) -c src/main.c -Iinclude/ -o bin/main.o
	@$(COMPILER) bin/helloworld.o bin/main.o -o helloworld
	@echo "Finished building helloworld"