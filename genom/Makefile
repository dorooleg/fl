PROJECT_NAME := genom
CPP_COMPILER := clang++
CPP_FLAGS 	 := -MMD -Wall -lgtest -std=c++14 -O3 -Itests -Isrc -I../shared
BUILD_DIR 	 := bin
SRC_DIR 	 := src
TEST_DIR 	 := test
SHARED_DIR 	 := ../shared

CPP_FILES 	 := $(wildcard ${SRC_DIR}/*.cpp)
CPP_FILES 	 += $(wildcard ${SHARED_DIR}/*.cpp)
OBJ_FILES 	 := $(addprefix ${BUILD_DIR}/,$(notdir $(CPP_FILES:.cpp=.o)))
OBJ_FILES 	 := $(OBJ_FILES:.cc=.o)


all: mkdir ${BUILD_DIR}/${PROJECT_NAME} ${BUILD_DIR}/tests

mkdir: 
	mkdir -p bin

clean:
	rm -r -- ${BUILD_DIR}

${BUILD_DIR}/${PROJECT_NAME}: ${OBJ_FILES}
	${CPP_COMPILER} ${CPP_FLAGS} -o $@ $^ 

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CPP_COMPILER} ${CPP_FLAGS} -c $< -o $@ 

${BUILD_DIR}/tests: ${TEST_DIR}/tests.cpp
	${CPP_COMPILER} ${CPP_FLAGS} $< -o $@ 

${BUILD_DIR}/%.o: ${SHARED_DIR}/%.cpp
	${CPP_COMPILER} ${CPP_FLAGS} -c $< -o $@ 

-include ${BUILD_DIR}/*.d

.PHONY: clean all
