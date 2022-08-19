#!/bin/bash

str1=""
str2=""
str3=""

read -p "Enter string1: " str1
read -p "Enter string2: " str2
read -p "Enter string3: " str3

if [ "$str1" == "$str2" ]
then
	if [ "$str1" == "$str3" ]
	then
		echo "equal"
	else
		echo "not equal"
	fi
	
else
	echo "not equal"
fi
