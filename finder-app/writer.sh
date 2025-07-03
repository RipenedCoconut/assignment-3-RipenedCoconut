#!/bin/sh
# File writer script

# Check for valid number of arguments
if [ $# -lt 2 ]
then
	echo "ERROR: An incorrect number of arguments were specificed: $#"
	exit 1
fi

# Argument one: path to new file
writeFile=$1

# Argument two: the contents to be written to the above directory
writeStr=$2

#Check if path exists, if not create it
if [ ! -e $writeFile ]
then
	mkdir -p $writeFile
	rm -rf $writeFile
fi

# Write the contnents of writeStr to writeFile
echo $writeStr > $writeFile

# Clean exit
exit 0
