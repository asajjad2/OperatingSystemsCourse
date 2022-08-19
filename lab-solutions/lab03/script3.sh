#!/bin/bash

inp=""
op1=""
op2=""
op3=""

read -p "input a. multiply, b. divide, c. factorial" inp
read -p "Enter op1: " op1
read -p "Enter op2: " op2


function fact(){

	op3=$op1

	while [ $op1 -ne 2 ]
	do
 	  
 	  ((op1--))
	  op3=$(expr $op3 \* $op1)

	done

	echo $op3

}



case "$inp" in 
   [aA]) 
     expr $op1 \* $op2
     ;;
   [bB])
     expr $op1 / $op2
     ;;
   [cC])
     fact
     ;;
   *)
     echo "default"
     ;;
esac




