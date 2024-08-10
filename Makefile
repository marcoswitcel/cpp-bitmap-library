# Configurações de compilação
## usando g++ não precisa do parâmetro -lstdc++
CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++2a # -fsanitize=address -g
LFLAGS =
DEF = -DDEV_CODE_ENABLED

# Configurações gerais
SOURCE_FOLDER_NAME=src
BUILD_FOLDER_NAME=target
PROGRAM_NAME=bmp_tool

CURRENT_HASH=$(shell git rev-parse HEAD)
BUILD_TIMESTAMP="\"$(shell date +'%F %H:%M:%S')\""
PROGRAM_VERSION="\"0.0.1 ($(CURRENT_HASH))\""

build-folder-setup:
	@ mkdir -p $(BUILD_FOLDER_NAME)

main: build-folder-setup ./$(SOURCE_FOLDER_NAME)/main.cpp  
	rm -f $(BUILD_FOLDER_NAME)/$(PROGRAM_NAME)
	$(CC) ./$(SOURCE_FOLDER_NAME)/main.cpp -o $(BUILD_FOLDER_NAME)/$(PROGRAM_NAME) $(CFLAGS) $(LFLAGS) $(DEF) -DPROGRAM_VERSION=$(PROGRAM_VERSION) -DBUILD_TIMESTAMP=$(BUILD_TIMESTAMP)

run: main
	@echo "cd ./$(BUILD_FOLDER_NAME)"
	@echo ./$(PROGRAM_NAME) $(ARGS)
	@echo "================"
	@cd ./$(BUILD_FOLDER_NAME) && ./$(PROGRAM_NAME) $(ARGS)
	@echo "\n================\n"
