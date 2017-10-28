#!/bin/bash

OUTPUT_FOLDER=$1
OUTPUT_NAME=$2

TMP_SOURCE_FILE=allInOneSourceFile.c

echo "output name is ${OUTPUT_NAME}"
echo "output folder is ${OUTPUT_FOLDER}"

mkdir -pv ${OUTPUT_FOLDER}

#only *.h
rm -fv "${OUTPUT_FOLDER}/tmp.c"
rm -fv "${OUTPUT_FOLDER}/${OUTPUT_NAME}.h"
for hfile in `ls "../../src/main/include" | grep -e ".h$"`
do
	echo "#include \"${hfile}\"" >> "${OUTPUT_FOLDER}/tmp.c"
done
cp -v "${OUTPUT_FOLDER}/tmp.c" "${OUTPUT_FOLDER}/${OUTPUT_NAME}.h"

#both *.c and *.h
rm -fv "${OUTPUT_FOLDER}/tmp.c"
rm -fv "${OUTPUT_FOLDER}/${OUTPUT_NAME}.all.in.one.h"
for cfile in `ls "../../src/main/c" | grep -e ".c$"`
do
	echo "#include \"${cfile}\"" >> "${OUTPUT_FOLDER}/tmp.c"
done
gcc -DALL_IN_ONE -E -I../../src/main/include -I../../src/main/c -o "${OUTPUT_FOLDER}/${OUTPUT_NAME}.all.in.one.h" "${OUTPUT_FOLDER}/tmp.c"
if test $? -ne 0
then
	exit 1
fi

#touch "${OUTPUT_FOLDER}/${OUTPUT_NAME}.h"
#for hfile in `ls "../../src/main/include" | grep -e ".h$"`
#do
#	cat "../../src/main/include/${hfile}" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}.h"
#done
#
#touch "${OUTPUT_FOLDER}/${OUTPUT_NAME}.c"
#for hfile in `ls "../../src/main/include" | grep -e ".h$"`
#do
#	cat "../../src/main/include/${hfile}" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}.c"
#done
#for cfile in `ls "../../src/main/c" | grep -e ".c$"`
#do
#	cat "../../src/main/c/${cfile}" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}.c"
#done
