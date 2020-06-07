CC = gcc
INC = include
SRC = src
BUILD = build
BIN = bin
INCLUDE = -I ./$(INC)

TARGET = pp

OBJECTS = $(BUILD)/main.o $(BUILD)/task.o $(BUILD)/subset.o $(BUILD)/queue.o

$(BIN)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN)
	@$(CC) -o $@ $^ $(INCLUDE)

$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(BUILD)
	@$(CC) -o $@ -c $^ $(INCLUDE)

run: $(BIN)/$(TARGET)
	@$(BIN)/$(TARGET)

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD) $(BIN)
	@rm -f output.txt