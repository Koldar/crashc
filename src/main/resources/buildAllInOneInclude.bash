#!/bin/bash

OUTPUT_FOLDER=$1
OUTPUT_NAME=$2
C_FOLDER=$3
H_FOLDER=$4

echo "output name is ${OUTPUT_NAME}"
echo "output folder is ${OUTPUT_FOLDER}"
echo "c source files are in ${C_FOLDER}"
echo "h header files are in ${H_FOLDER}"

# ***************** SETUP **********************

mkdir -pv ${OUTPUT_FOLDER}
rm -fv "${OUTPUT_FOLDER}/${OUTPUT_NAME}"

# *************** CREATE TMP.C FILE ****************

echo "#ifndef CRASHC_ALL_IN_ONE_H_" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
echo "#define CRASHC_ALL_IN_ONE_H_" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"

#the order is relevant
cat "${H_FOLDER}/hashtable.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${H_FOLDER}/list.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${H_FOLDER}/tag.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${H_FOLDER}/section.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${H_FOLDER}/crashC.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${H_FOLDER}/main_model.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${H_FOLDER}/model.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${H_FOLDER}/command_line.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${H_FOLDER}/sigHandling.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${H_FOLDER}/testReport.h" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"

#the order is irrelevant
cat "${C_FOLDER}/hashtable.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${C_FOLDER}/list.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${C_FOLDER}/section.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${C_FOLDER}/tag.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${C_FOLDER}/crashC.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${C_FOLDER}/main_model.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${C_FOLDER}/model.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${C_FOLDER}/command_line.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${C_FOLDER}/sigHandling.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"
cat "${C_FOLDER}/testReport.c" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"

echo "#endif /* CRASHC_ALL_IN_ONE_H_ */" >> "${OUTPUT_FOLDER}/${OUTPUT_NAME}"


