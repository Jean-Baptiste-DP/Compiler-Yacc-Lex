bison -d yaccLex/compiler.y
flex yaccLex/compiler.l
# gcc -o compiler compiler.tab.c lex.yy.c src/*.c src/*/*.c
gcc -o compiler compiler.tab.c lex.yy.c src/variables.c src/stack.c src/prgmStructure2.c src/calc2/*.c

find test -type f -name "*.test.txt" | while read testFile; do
    outputFile=$(echo "$testFile"|sed 's/test.txt/output.txt/')
    (./compiler < $testFile) > output.txt

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