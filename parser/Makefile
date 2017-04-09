PROJECT_NAME := parser
CPP_COMPILER := clang++
CPP_FLAGS 	 := -std=c++11 -ll
BUILD_DIR 	 := bin
SRC_DIR 	 := src
LEXER_FILE   := lexer.l
PARSER_FILE  := parser.y
LEXER_C_FILE := ${SRC_DIR}/lex.yy.c ${SRC_DIR}/y.tab.c ${SRC_DIR}/graph.c ${SRC_DIR}/error.c
RM_FILES     := ${SRC_DIR}/lex.yy.c ${SRC_DIR}/y.tab.c ${SRC_DIR}/y.tab.h

LEXER_C_FILE_EXT := lex.yy.c y.tab.c

all:
	mkdir -p ${BUILD_DIR}
	bison -y -d ${SRC_DIR}/${PARSER_FILE}
	lex ${SRC_DIR}/${LEXER_FILE}
	mv ${LEXER_C_FILE_EXT} ${SRC_DIR}/
	mv y.tab.h ${SRC_DIR}/
	${CPP_COMPILER} ${LEXER_C_FILE} ${CPP_FLAGS} -o ${BUILD_DIR}/${PROJECT_NAME}

clean:
	rm -r -- ${BUILD_DIR}
	rm -- ${RM_FILES}

.PHONY: clean all
