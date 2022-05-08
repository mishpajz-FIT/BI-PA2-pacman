NAME := dobesmic

CXX := g++
FLAGS := -std=c++17 -O2 -Wall -pedantic -lncurses

SOURCE_DIR := src
BUILD_DIR := build


SOURCES := $(wildcard ${SOURCE_DIR}/*.cpp  ${SOURCE_DIR}/*/*.cpp ${SOURCE_DIR}/*/*/*.cpp)
OBJECTS := $(patsubst ${SOURCE_DIR}/%.cpp, ${BUILD_DIR}/%.o, ${SOURCES})
INCLUDE := $(strip $(addprefix -I , $(dir ${SOURCES})))

.PHONY: compile print run clean doc count

compile: ${OBJECTS}
	@${CXX} ${FLAGS} ${INCLUDE} $^ -o ${NAME}
	@echo "Compiling ${NAME}"

${BUILD_DIR}/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@${CXX} ${FLAGS} ${INCLUDE} -c $< -o $@
	@echo "Building $@"

run: compile
	./${NAME}

clean:
	@rm -rf ${BUILD_DIR}
	@rm -rf ${NAME}
	@rm -rf doc/generated
	@echo "Cleaning"

doc: doxygen Doxyfile

count:
	@wc -l ${SOURCE_DIR}/*/*.h ${SOURCE_DIR}/*/*/*.h ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*/*.cpp ${SOURCE_DIR}/*/*/*.h