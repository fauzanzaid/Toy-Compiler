#!/bin/bash

rm -rf ext

export GIT_TERMINAL_PROMPT=0

echo -n 'Downloading BitSet ' ;		git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/BitSet-in-C ./ext/BitSet				&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
echo -n 'Downloading Dfa ' ;		git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/DFA-in-C ./ext/Dfa					&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
echo -n 'Downloading HashTable ' ;	git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/Hash-Table-in-C ./ext/HashTable			&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
echo -n 'Downloading LinkedList ' ;	git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/Linked-List-in-C ./ext/LinkedList			&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
echo -n 'Downloading Lexer ' ;		git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/Lexer-in-C ./ext/Lexer				&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
echo -n 'Downloading Token ' ;		git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/Compiler-Token-in-C ./ext/Token			&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
echo -n 'Downloading ParserLL1 ' ;	git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/LL1-Parser-in-C ./ext/ParserLL1			&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
echo -n 'Downloading ParseTree ' ;	git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/Parse-Tree-in-C ./ext/ParseTree			&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
echo -n 'Downloading SymbolEnv ' ;	git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/Symbol-Table-in-C ./ext/SymbolEnv			&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
echo -n 'Downloading Quad ' ;		git clone -b v0.1.0 --depth=1 https://github.com/fauzanzaid/Quadruple-Three-Address-Code-in-C ./ext/Quad	&> /dev/null && { echo 'done' ; } || { echo 'failed' ; exit 1; }
