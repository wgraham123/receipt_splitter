COMPILER=gcc

project:
	@$(COMPILER) -c src/StringFunctions.c -Iinclude/ -o bin/StringFunctions.o
	@echo "Built StringFunctions.c"
	@$(COMPILER) -c src/HelperFunctions.c -Iinclude/ -o bin/HelperFunctions.o
	@echo "Built HelperFunctions.c"
	@$(COMPILER) -c src/Receipt.c -Iinclude/ -o bin/Receipt.o
	@echo "Built Receipt.c"
	@$(COMPILER) -c src/UserInput.c -Iinclude/ -o bin/UserInput.o
	@echo "Built UserInput.c"
	@$(COMPILER) -c src/SplitReceipt.c -Iinclude/ -o bin/SplitReceipt.o
	@echo "Built SplitReceipt.c"
	@$(COMPILER) -c src/main.c -Iinclude/ -o bin/main.o
	@echo "Built main.c"
	@$(COMPILER) bin/UserInput.o bin/SplitReceipt.o bin/Receipt.o bin/main.o bin/StringFunctions.o bin/HelperFunctions.o -o Splitter
	@echo "Built Splitter"

	@$(COMPILER) -c test/Test.c -Iinclude/ -o bin/Test.o -w
	@$(COMPILER) bin/Test.o bin/UserInput.o bin/SplitReceipt.o bin/Receipt.o bin/StringFunctions.o bin/HelperFunctions.o -o TestSplitter -w
	@./TestSplitter