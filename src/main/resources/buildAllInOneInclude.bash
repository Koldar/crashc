#!/bin/bash

OUTPUT_FOLDER=$1
OUTPUT_NAME=$2

TMP_SOURCE_FILE=allInOneSourceFile.c

echo "output name is ${OUTPUT_NAME}"
echo "output folder is ${OUTPUT_FOLDER}"

# ***************** SETUP **********************

mkdir -pv ${OUTPUT_FOLDER}

#both *.c and *.h
rm -fv "${OUTPUT_FOLDER}/tmp.c"
rm -fv "${OUTPUT_FOLDER}/${OUTPUT_NAME}"

# *************** CREATE TMP.C FILE ****************

#create guard of tmp.c
echo "#ifndef CRASHC_H_" >> "${OUTPUT_FOLDER}/tmp.c"
echo "	#define CRASHC_H_" >> "${OUTPUT_FOLDER}/tmp.c"

#include headers in tmp.c
#in this way we insert the guards of each file as well
for hfile in `ls "../../src/main/include" | grep -e ".h$"`
do
	echo "copying ${hfile} inside tmp..."
	echo "#include \"${hfile}\"" >> "${OUTPUT_FOLDER}/tmp.c"
done

#include all the source files: in this way we have the implementations of the header functions as well
for cfile in `ls "../../src/main/c" | grep -e ".c$"`
do
	echo "copying ${cfile} inside tmp..."
	echo "#include \"${cfile}\"" >> "${OUTPUT_FOLDER}/tmp.c"
done

#close crashC guard
echo "#endif /* CRASHC_H */" >> "${OUTPUT_FOLDER}/tmp.c"

#how does it work? see https://stackoverflow.com/a/38163793/1887602
echo "generating all in one header..."
gcc -E -Dinclude=#include -I../../src/main/c -I../../src/main/include -I../../src/main/system_include -o "${OUTPUT_FOLDER}/${OUTPUT_NAME}" "${OUTPUT_FOLDER}/tmp.c"
#gcc -DALL_IN_ONE -E -I../../src/main/include -I../../src/main/c -o "${OUTPUT_FOLDER}/${OUTPUT_NAME}" "${OUTPUT_FOLDER}/tmp.c"
if test $? -ne 0
then
	exit 1
fi

#remove tmp.c
rm -fv "${OUTPUT_FOLDER}/tmp.c"

