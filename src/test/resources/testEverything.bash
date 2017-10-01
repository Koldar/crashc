#!/bin/bash


#we need to go to src/test/c, aka the place where all the tests are present
#we fetch all the issues id. There is contract:
# 1) every file is in the form test_issue%04d.c where "%04d" is the id of the issue.
# 2) the issue source code is always included in #ifdef, actitable by a define macro called TEST_%04d where "%04d" is the id of the issue
#
# so, we fetch all the issues in side src/test/c; then, for each of them, we compile

cd .
if test `basename $(realpath .)` != "crashc"
then
	echo "You are not in 'crashc' folder! I won't execute this script because I'm not sure this will work!"
	exit 2
fi
CRASHC_MAINFOLDER=`pwd`

mkdir -pv build/Debug

cd src/test/c
if test $? -ne 0
then
	echo "going into src/test/c has failed. Exiting!"
	exit 2
fi
CRASHC_TEST_C=`pwd`
cd "${CRASHC_MAINFOLDER}/build/Debug"
if test $? -ne 0
then
	echo "going into build/Debug has failed. Exiting!"
	exit 2
fi
CRASHC_BUILD_DEBUG=`pwd`



OLDPWD=`pwd`

cd ${CRASHC_BUILD_DEBUG}
	for issueID in `ls -l ${CRASHC_TEST_C}| grep -E "issue[0-9]+\." | sed 's/  */ /g' | cut -d" " -f9 | sed 's/test_issue\([0-9][0-9]*\)\.c/\1/g'`
	do
		printf "testing issue #%04d\n" "${issueID}"
		#alter CMakeFiles.txt in the main directory
		printf -v define_value 'TEST_%04d' "${issueID}"
		sed -i "s/add_definitions(-DTEST_[0-9][0-9]*)/add_definitions(-D${define_value})/" "${CRASHC_MAINFOLDER}/CMakeLists.txt"
		#now we compile everything
	
		cmake ../.. && make && clear && ./Test > output.${issueID}.txt
		#we ensure the just created output has no KO
		if test `cat output.${issueID}.txt | grep "KO" | wc -l` -gt 0
		then
			echo "Error while executing test ${issueID}!"
			cd ${CRASHC_MAIN_FOLDER}
			exit 1
		fi
	done

cd $OLDPWD
echo "EVERYTHING HAS BEEN DONE!"
