COMPILER=gcc

project:
	@$(COMPILER) -c src/ReceiptLoader/*.c -Iinclude/ -o bin/ReceiptLoader.o
	@echo "Built ReceiptLoader"
	@$(COMPILER) -c src/Main.c -Iinclude/ -o bin/Main.o
	@echo "Built Main"
	@$(COMPILER) bin/ReceiptLoader.o bin/Main.o -o Splitter
	@echo "Built Splitter"

	@$(COMPILER) -c test/TestMain.c -Iinclude/ -o bin/TestMain.o -w
	@$(COMPILER) bin/TestMain.o bin/ReceiptLoader.o -o TestSplitter -w
	@./TestSplitter