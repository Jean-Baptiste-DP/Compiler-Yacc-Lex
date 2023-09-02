#!/bin/bash

# Directories
src_dir="src"
precompile_dir="output"

length_src_dir=${#src_dir}

# Yacc/Lex generation
bison -d yaccLex/compiler.y
flex yaccLex/compiler.l

# Yacc/Lex precompilation
gcc -c -o compiler.tab.o compiler.tab.c 
gcc -c -o lex.yy.o lex.yy.c

# Src files.c precompilation
find "$src_dir" -name '*.c' -type f | while read cfile; do
    output_path="$precompile_dir${cfile:${length_src_dir}}"
    mkdir -p "$(dirname "$output_path")"
    gcc -c "$cfile" -o "${output_path%.c}.o"
done

# Src files.cpp precompilation
find "$src_dir" -name '*.cpp' -type f | while read cfile; do
    output_path="$precompile_dir${cfile:${length_src_dir}}"
    mkdir -p "$(dirname "$output_path")"
    g++ -c "$cfile" -o "${output_path%.c}.o"
done

# Compilation
shopt -s globstar
g++ -o compiler compiler.tab.o lex.yy.o $precompile_dir/**/*.o
shopt -u globstar

# Remove precompiled files
rm -rf $precompile_dir
rm lex.yy.o
rm compiler.tab.o

find test -type f -name "*.test.txt" | while read testFile; do
    outputFile=$(echo "$testFile"|sed 's/test.txt/output.txt/')
    (./compiler $testFile) > output.txt

    if test -f "$outputFile"; then
        if diff -q output.txt "$outputFile" >/dev/null 2>&1; then
            echo -e "\x1B[32m Test passed for : $testFile \x1B[0m"
        else
            echo -e "\x1B[31m Test failed for : $testFile \x1B[0m"
        fi
    else
        cat "output.txt"
        echo
        echo "There is no reponse file for $testFile ";
        echo -n "Do you want to save the previous output as the reponse file ? (Y/N) "
        read -r response < /dev/tty;
        if [[ "$response" =~ ^(y|Y)$ ]]; then
            mv output.txt "$outputFile"
        fi
    fi
done

rm output.txt