PROJECT_NAME := lexer
CPP_COMPILER := clang++
CPP_FLAGS 	 := -std=c++11 -ll
BUILD_DIR 	 := bin
SRC_DIR 	 := src
LEXER_FILE   := lexer.l
LEXER_C_FILE := lex.yy.c

all:
	mkdir -p ${BUILD_DIR}
	lex ${SRC_DIR}/${LEXER_FILE}
	mv ${LEXER_C_FILE} ${SRC_DIR}/
	${CPP_COMPILER} ${SRC_DIR}/${LEXER_C_FILE} ${CPP_FLAGS} -o ${BUILD_DIR}/${PROJECT_NAME}

clean:
	rm -r -- ${BUILD_DIR}
	rm -- ${SRC_DIR}/${LEXER_C_FILE}

.PHONY: clean all
