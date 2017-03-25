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
	echo '#!/bin/bash \nSCRIPT="$$(readlink -f $$0)"\nSCRIPTPATH=$$(dirname $$SCRIPT)\ncat -- "$$1" | "$${SCRIPTPATH}"/lexer' > ${BUILD_DIR}/${PROJECT_NAME}.sh
	chmod 777 ${BUILD_DIR}/${PROJECT_NAME}.sh

clean:
	rm -r -- ${BUILD_DIR}
	rm -- ${SRC_DIR}/${LEXER_C_FILE}

.PHONY: clean all
