CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17
BUILD    := ./build
BIN		 := ./bin
OBJ_DIR  := $(BUILD)
APP_DIR  := $(BIN)
TARGET  := main
INCLUDE  := -I. -Iinclude/math
SRC      := $(wildcard ./**/*.cpp) main.cpp

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(OBJECTS:.o=.d)

all : build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rf $(OBJ_DIR)/*
	-@rm -rf $(APP_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"