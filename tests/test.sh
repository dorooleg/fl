#!/bin/bash
VALUE="$(../bin/lexer.sh base_test)"
if [[ "$VALUE" == 'KW_Read(0, 0, 3); Ident("x", 0, 5, 5); Colon(0, 6, 6); KW_If(0, 8, 9); Ident("y", 0, 11, 11); Op(PLUS, 0, 13, 13); Num(1, 0, 15, 15); Op(EQ, 0, 17, 18); Ident("x", 0, 20, 20); KW_Then(0, 22, 25); KW_Write(0, 27, 31); Ident("y", 0, 33, 33); KW_Else(0, 35, 38); KW_Write(0, 40, 44); Ident("x", 0, 46, 46); ' ]]; then
    echo "base_test: done"
else
    echo "base_test: failed"
    echo "$VALUE"
fi

VALUE="$(../bin/lexer.sh kw)"
if [[ "$VALUE" == 'KW_If(0, 0, 1); KW_Then(0, 3, 6); KW_Else(0, 8, 11); KW_While(0, 13, 17); KW_Do(0, 19, 20); KW_Read(0, 22, 25); KW_Write(0, 27, 31); KW_Begin(0, 33, 37); KW_End(0, 39, 41); ' ]]; then
    echo "kw: done"
else
    echo "kw: failed"
    echo "$VALUE"
fi

VALUE="$(../bin/lexer.sh kw_no_spaces)"
if [[ "$VALUE" == 'Ident("ifthenelsewhiledoreadwritebeginend", 0, 0, 33); ' ]]; then
    echo "kw_no_spaces: done"
else
    echo "kw_no_spaces: failed"
    echo "$VALUE"
fi

VALUE="$(../bin/lexer.sh error)"
if [[ "$VALUE" == 'KW_If(0, 0, 1); Error(#, 0, 3, 3); ' ]]; then
    echo "error: done"
else
    echo "error: failed"
    echo "$VALUE"
fi

VALUE="$(../bin/lexer.sh double)"
if [[ "$VALUE" == 'Num(-1e-68, 0, 0, 7); Num(0.387, 1, 0, 4); Num(0.57, 2, 0, 3); Num(837, 3, 0, 2); Num(1e+38, 4, 0, 4); Num(0, 5, 0, 0); Num(0.355, 6, 0, 3); Num(25.6, 7, 0, 3); Num(-0, 8, 0, 1); ' ]]; then
    echo "double: done"
else
    echo "double: failed"
    echo "$VALUE"
fi

VALUE="$(../bin/lexer.sh fac)"
if [[ "$VALUE" == 'KW_Read(0, 0, 3); Ident("n", 0, 5, 5); Ident("res", 1, 0, 2); Op(EQ, 1, 4, 5); Num(1, 1, 7, 7); KW_While(2, 0, 4); Ident("n", 2, 6, 6); Op(NEQ, 2, 8, 9); Num(0, 2, 11, 11); KW_Begin(2, 13, 17); Ident("res", 3, 4, 6); Op(EQ, 3, 8, 9); Ident("res", 3, 12, 14); Op(MUL, 3, 16, 16); Ident("n", 3, 18, 18); Ident("n", 4, 4, 4); Op(EQ, 4, 6, 7); Ident("n", 4, 10, 10); Op(MINUS, 4, 12, 12); Num(1, 4, 14, 14); KW_End(5, 0, 2); KW_Write(6, 0, 4); Ident("res", 6, 6, 8); ' ]]; then
    echo "fac: done"
else
    echo "fac: failed"
    echo "$VALUE"
fi

VALUE="$(../bin/lexer.sh full)"
if [[ "$VALUE" == 'KW_If(0, 0, 1); KW_Then(0, 3, 6); KW_Else(0, 8, 11); KW_While(0, 13, 17); KW_Do(0, 19, 20); KW_Read(0, 22, 25); KW_Write(0, 27, 31); KW_Begin(0, 33, 37); KW_End(0, 39, 41); Op(PLUS, 1, 0, 0); Op(MINUS, 1, 2, 2); Op(MUL, 1, 4, 4); Op(DIV, 1, 6, 6); Op(MOD, 1, 8, 8); Op(EQ, 1, 10, 11); Op(NEQ, 1, 13, 14); Op(GT, 1, 16, 16); Op(GTE, 1, 18, 19); Op(LT, 1, 21, 21); Op(LTE, 1, 23, 24); Op(AND, 1, 26, 27); Op(OR, 1, 29, 30); Colon(2, 0, 0); Colon(2, 2, 2); Colon(2, 4, 4); Comment(hello world, 3, 0, 12); Ident("variable", 5, 0, 7); Ident("_var", 6, 0, 3); Ident("v123b", 7, 0, 4); Bool(TRUE, 8, 0, 3); Bool(FALSE, 9, 0, 4); ' ]]; then
    echo "full: done"
else
    echo "full: failed"
    echo "$VALUE"
fi
