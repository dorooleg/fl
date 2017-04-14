PROJECT_NAME := cyk
CPP_COMPILER := clang++
CPP_FLAGS 	 := -MMD -Werror -Wall -std=c++14 -O3 -Itests -Isrc
BUILD_DIR 	 := bin
SRC_DIR 	 := src
TEST_DIR 	 := tests

CPP_FILES 	 := $(wildcard ${SRC_DIR}/*.cpp)
CPP_FILES 	 += $(wildcard ${TEST_DIR}/*.cpp)
OBJ_FILES 	 := $(addprefix ${BUILD_DIR}/,$(notdir $(CPP_FILES:.cpp=.o)))
OBJ_FILES 	 := $(OBJ_FILES:.cc=.o)


all: mkdir ${BUILD_DIR}/${PROJECT_NAME}

mkdir : 
	mkdir -p bin

clean:
	rm -- ${BUILD_DIR}/*.o ${BUILD_DIR}/*.d ${BUILD_DIR}/${PROJECT_NAME}

${BUILD_DIR}/${PROJECT_NAME}: ${OBJ_FILES}
	${CPP_COMPILER} ${CPP_FLAGS} -o $@ $^ 

${BUILD_DIR}/%.o: ${SRC_DIR}/%.cpp
	${CPP_COMPILER} ${CPP_FLAGS} -c $< -o $@ 

${BUILD_DIR}/%.o: ${TEST_DIR}/%.cpp
	${CPP_COMPILER} ${CPP_FLAGS} -c $< -o $@ 


-include ${BUILD_DIR}/*.d

.PHONY: clean all
