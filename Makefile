COMPILER=gcc

project:
	@$(COMPILER) -c src/Receipt.c -Iinclude/ -o bin/Receipt.o
	@echo "Built Receipt.c"
	@$(COMPILER) -c src/Main.c -Iinclude/ -o bin/Main.o
	@echo "Built Main.c"
	@$(COMPILER) bin/Receipt.o bin/Main.o -o Splitter
	@echo "Built Splitter"

	@$(COMPILER) -c test/Test.c -Iinclude/ -o bin/Test.o -w
	@$(COMPILER) bin/Test.o bin/Receipt.o -o TestSplitter -w
	@./TestSplitter