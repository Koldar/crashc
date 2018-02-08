#!/bin/bash

#create Production and Release folder
mkdir -pv build/Production
mkdir -pv build/Release

PRODUCTION="../Production/"

OLDPWD=`pwd`
cd build/Release
	cmake -DU_LIBRARY_TYPE:STRING=SO ../..
	make all
	if test $? -ne 0
	then
		cd ${OLDPWD}
		exit 1
	fi
	#move header files to production
	mkdir -pv "${PRODUCTION}/include"
	cp -fv ../../src/main/include/*.h "${PRODUCTION}/include"
	make allInOneHeader	
	cp -fv libCrashC.so.1.0 "${PRODUCTION}"
	cp -fv allInOne/crashC.all.in.one.h "${PRODUCTION}"
	cmake -DU_LIBRARY_TYPE:STRING=AO ../..
	make all
	if test $? -ne 0
	then
		cd ${OLDPWD}
		exit 1
	fi
	cp -fv libCrashC.a "${PRODUCTION}"
cd ${OLDPWD}


echo "Done! You can fuind everything inside build/Production!"
