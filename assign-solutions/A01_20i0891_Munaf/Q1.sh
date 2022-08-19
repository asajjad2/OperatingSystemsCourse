echo "Enter a Number: ";
read num1
echo "Enter a Number: ";
read num2
echo "Enter a Number: ";
read num3

n1=$((num1 % 10))
n1=$((num2 % 10))
n2=$((num3 % 10))

if [$n1=$n3] || [$n1=$n2] || [$n2=$n3]
then
    echo "True"
else
    echo "False"
fi
