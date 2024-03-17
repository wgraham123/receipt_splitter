COMPILER=gcc

project:
	@$(COMPILER) -c src/Receipt.c -Iinclude/ -o bin/Receipt.o
	@echo "Built Receipt.c"
	@$(COMPILER) -c src/Main.c -Iinclude/ -o bin/Main.o
	@echo "Built Main.c"
	@$(COMPILER) bin/Receipt.o bin/Main.o -o Splitter
	@echo "Built Splitter"

	@$(COMPILER) -c test/TestMain.c -Iinclude/ -o bin/TestMain.o -w
	@$(COMPILER) bin/TestMain.o bin/Receipt.o -o TestSplitter -w
	@./TestSplitter