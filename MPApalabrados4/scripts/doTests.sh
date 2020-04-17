#!  /bin/bash
# Author: Luis Castillo Vidal L.Castillo@decsai.ugr.es
VERSION=1.0
# Load configuration & moves to the project root folder
if [ -d scripts ]
then
    source scripts/doConfig.sh
else
    source doConfig.sh
fi
clear
if [ -z "$(ls -A $TESTS_FOLDER/*.test)" ]
then
    echo "NO TESTS AVAILABLE"
    exit
fi
printf "${WHITE}Generating fresh binaries\n${GRAY}"
make build
printf "${WHITE}\n"
# Marks to parse .test files
CALL_MARK="%%%CALL"
OUTPUT_MARK="%%%OUTPUT"
OUTPUTFILE_MARK="%%%FROMFILE"
VALGRIND_MARK="%%%VALGRIND"
# Prebuild the command line for the call 
DIST=dist/Debug/GNU-Linux/
BINARY=$(ls $DIST)
# Auxiliary files
# .out* Raw output of the run
# .due* Expected predefined output
# .real* Real predefined output
# .fail* Output of every failed test
# .call* Description of the call
rm -f $TESTS_FOLDER/.out* $TESTS_FOLDER/.due* $TESTS_FOLDER/.real* $TESTS_FOLDER/.fail* $TESTS_FOLDER/.call*
# Flag to detect FAILED tests
FAILED_TESTS=NO
k=1
for test in $TESTS_FOLDER/*.test
do
    # Parse the codes for the call
    if grep --quiet $CALL_MARK $test; # Cond1
    then 
        MYCALL=$(grep $CALL_MARK $test | sed s/$CALL_MARK//)
        # Parse the expected output from the test
        DUE_OUTPUT=$(sed -n /$OUTPUT_MARK/,\$p $test)
        # Parse possible  Valgrind request
        if  grep --quiet $VALGRIND_MARK $test; 
        then
            FORCE_VALGRIND=YES
        else
            FORCE_VALGRIND=NO
        fi
        # Compose the call and execute it
        if [ $USE_VALGRIND == "YES" ] || [ $FORCE_VALGRIND == "YES" ]
        then 
            VALGRIND="valgrind --leak-check=full "
            FULL_BINARY=$VALGRIND$PROJECT_FOLDER/$DIST/$BINARY
            echo "Test #$k [$test] $VALGRIND $BINARY $MYCALL" > $TESTS_FOLDER/.call$k
            eval "$FULL_BINARY $MYCALL &> $TESTS_FOLDER/.out$k"
            # If Valgrind is requested, an additional check is carried out
            # to llok for possible memory leaks
            VALGRIND_OUTPUT="\n        "$(grep -a "ERROR SUMMARY" $TESTS_FOLDER/.out$k | sed s/==[0-9]*==//)
            if grep -a "ERROR SUMMARY" $TESTS_FOLDER/.out$k | grep --quiet " 0 errors";
            then 
                VALGRIND_LEAKS=FALSE
            else
                VALGRIND_LEAKS=TRUE
            fi
        else
            VALGRIND=""
            FULL_BINARY=$VALGRIND$PROJECT_FOLDER/$DIST/$BINARY
            echo "Test #$k [$test] $VALGRIND $BINARY $MYCALL" > $TESTS_FOLDER/.call$k
            eval "$FULL_BINARY $MYCALL &> $TESTS_FOLDER/.out$k"
            VALGRIND_OUTPUT=""
            VALGRIND_LEAKS=FALSE
        fi
        # If the real output of the program cotains %%%OUTPUT marks, then only those marks
        # are compared for validity. Otherwise, the full output is compared
        if  grep -a --quiet $OUTPUT_MARK $TESTS_FOLDER/.out$k; 
        then
            REAL_OUTPUT=$(grep -a -v == $TESTS_FOLDER/.out$k | sed -n /$OUTPUT_MARK/,\$p )
        else
            if  grep --quiet $OUTPUTFILE_MARK $test; 
            then
                FILE_OUTPUT=$(grep $OUTPUTFILE_MARK $test | sed s/$OUTPUTFILE_MARK//)
                REAL_OUTPUT=$(echo $OUTPUT_MARK;cat $FILE_OUTPUT )
            else
                REAL_OUTPUT=$(echo  $OUTPUT_MARK; grep -v == $TESTS_FOLDER/.out$k)
            fi
        fi
        # Save due and real outputs in disk for further use
        echo "$DUE_OUTPUT" > $TESTS_FOLDER/.due$k
        echo "$REAL_OUTPUT" > $TESTS_FOLDER/.real$k
        # When both outputs (expected and real) differ or valgrind detects leaks of memory
        # the test FAILS
        if [ ! "$DUE_OUTPUT" == "$REAL_OUTPUT" ] || [ "$VALGRIND_LEAKS" == "TRUE" ]
        then
            printf  "${RED}[FAILED] Test #$k [$test] ($VALGRIND $BINARY $MYCALL) $VALGRIND_OUTPUT\n"
            cp $TESTS_FOLDER/.out$k $TESTS_FOLDER/.fail$k
            FAILED_TESTS=YES
        else
            printf  "${GREEN}[  OK  ] Test #$k [$test] ($VALGRIND $BINARY $MYCALL) $VALGRIND_OUTPUT\n"
        fi
    else # Cond1
        printf "${RED}[ SKIP ] Test #$k [$test] CALL MARK not found in $test . ${GRAY}\n"
    fi # Cond 1
    k=$[k + 1]
done 
printf "${WHITE}\n"
if [ "$FAILED_TESTS" == "YES" ] && [ "$DO_REPORT" == "YES" ]
then 
    echo "Press [y] for SHORT REPORTS, [f] for LONG REPORTS, other to skip"
    read REVIEW
    if [ "$REVIEW" == "y" ] || [ "$REVIEW" == "f" ]
    then
        clear        
        printf "\n${WHITE}SHORT REPORT"
        for test in $TESTS_FOLDER/.fail*
        do
            NUMBER=$(echo $test | sed s/^.*fail//)
            printf "\n${WHITE}$(head -1 $TESTS_FOLDER/.call$NUMBER)${GRAY}"
            printf "\n${GRAY}Differences found between ${GREEN}DUE${GRAY} and ${RED}REAL${GRAY} outputs:\n${GRAY}"
            DIFF=$(diff $TESTS_FOLDER/.due$NUMBER $TESTS_FOLDER/.real$NUMBER | sed s/^\>/'\\033[0;31m'\>/ | sed s/^\</'\\033[0;32m'\</ | sed s/$/'\\033[1;30m'/)
#            DIFF=$(diff $TESTS_FOLDER/.due$NUMBER $TESTS_FOLDER/.real$NUMBER)
            printf "$DIFF"
            printf "\n"
        done 
    fi
    if [ "$REVIEW" == "f" ]
    then
        for test in $TESTS_FOLDER/.fail*
        do
            NUMBER=$(echo $test | sed s/^.*fail//)
            printf "\n${WHITE}$(head -1 $TESTS_FOLDER/.call$NUMBER)${GRAY}\n"
            cat $test
            printf "\n"
        done 
    fi
fi
# Remove auxiliary files
rm -f $TESTS_FOLDER/.out* $TESTS_FOLDER/.due* $TESTS_FOLDER/.real* $TESTS_FOLDER/.fail* $TESTS_FOLDER/.call*
printf "\n${WHITE}"
