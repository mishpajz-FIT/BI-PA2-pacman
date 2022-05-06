NAME := dobesmic

CXX := g++
FLAGS := -std=c++17 -O2 -Wall -pedantic

SOURCE_DIR := src
BUILD_DIR := build
SOURCES := $(wildcard ${SOURCE_DIR}/*.cpp  ${SOURCE_DIR}/*/*.cpp ${SOURCE_DIR}/*/*/*.cpp)
OBJECTS := $(patsubst ${SOURCE_DIR}/%.cpp, ${BUILD_DIR}/%.o, ${SOURCES})
INCLUDE := $(strip $(addprefix -I , $(dir ${SOURCES})))

.PHONY: compile print run

compile: ${OBJECTS}
	${CXX} ${FLAGS} ${INCLUDE} $^ -o ${NAME}

${BUILD_DIR}/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@${CXX} ${FLAGS} ${INCLUDE} -c $< -o $@
	@echo "Building $@"

run: compile
	./${NAME}
