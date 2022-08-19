#!/bin/bash

mkdir areeb
cd areeb
touch file1
cp file1 file1Copy
ls >> file1
ls -l >> file1
ls | wc >> file1
cp file1 file1NewCopy
mv file1NewCopy ..
cat file1Copy
cd ..
echo "Task1 completed"
echo "Going to delete the directory"
#rm -rf dir areeb
#echo "Done"
