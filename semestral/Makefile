NAME := dobesmic

CXX := g++
FLAGS := -std=c++17 -O2 -Wall -pedantic -lncurses

SOURCE_DIR := src
BUILD_DIR := build


SOURCES := $(wildcard ${SOURCE_DIR}/*.cpp  ${SOURCE_DIR}/*/*.cpp ${SOURCE_DIR}/*/*/*.cpp ${SOURCE_DIR}/*/*/*/*.cpp)
OBJECTS := $(patsubst ${SOURCE_DIR}/%.cpp, ${BUILD_DIR}/%.o, ${SOURCES})
INCLUDE := $(strip $(addprefix -I , $(dir ${SOURCES})))

.PHONY: all compile run clean doc

all: compile doc

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
	mkdir dontdelete
	mv doc/images dontdelete/images
	mv doc/pages dontdelete/pages
	rm -rf ${BUILD_DIR}
	rm -rf ${NAME}
	rm -rf doc/generated
	mv dontdelete/images doc/images
	mv dontdelete/pages doc/pages
	rm -rf dontdelete

doc: 
	doxygen Doxyfile