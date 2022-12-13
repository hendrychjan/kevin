
#!/bin/bash

####### SETTINGS: #######
TEST_COUNT=3
EXIT_ON_FAIL=true
DISPLAY_DIFF=true
OPEN_MELD=false
#########################
  
# The final result code
RESULT=0

# Build the main class
# export ASAN_OPTIONS='abort_on_error=1'
g++ -Wall -pedantic -g main.c -o a.out

# Execute and run for all tests (7)
for i in $(seq 0 $TEST_COUNT) ; do

  FILE="$( printf "%04d" $i )"

  # Run
  ./a.out < data/"$FILE"_in.txt > output/"$FILE"_out.txt
  
  # Test
  if ! cmp -s output/"$FILE"_out.txt data/"$FILE"_out.txt ; then
    
    # Test failed
    echo $'\e[1;31m'✖ Test $FILE failed$'\e[0m'

    if $DISPLAY_DIFF ; then

      echo "##### INPUT #######"
      cat data/"$FILE"_in.txt

      echo "##### DIFF ########"
      diff data/"$FILE"_out.txt output/"$FILE"_out.txt 

    fi

    if $OPEN_MELD ; then

      meld data/"$FILE"_out.txt output/"$FILE"_out.txt

    fi

    # Check 
    if $EXIT_ON_FAIL -eq "true" ; then
      exit 1
    else
      RESULT=1
    fi

  else

    # Test succeeded
    echo $'\e[1;32m'✔ Test ok: $FILE$'\e[0m'

  fi


done

exit $RESULT
