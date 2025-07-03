#!/bin/bash
# File search script

# Check for valid number of arguments
if [ $# -lt 2 ]
then
	echo "ERROR: An incorrect number of arguments were specificed: $#"
	exit 1
fi

# Argument one: file directory to search
filesDir=$1

# Argument two: the string to search for
searchStr=$2

#Check if path exists
if [ ! -e $filesDir ]
then
	echo "The path $filesdir does not exist"
	exit 1
fi

#Print the total number of directories (recursive) and name matches
echo "The number of files are $( find $filesDir -type f | wc -l ) and the number of matching lines are $( grep -Ior $searchStr $filesDir | wc -l)"


# Successful exit
exit 0
