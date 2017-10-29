#!/bin/bash

#create Production and Release folder
mkdir -pv build/Production
mkdir -pv build/Release

OLDPWD=`pwd`
cd build/Release
	cmake -DU_LIBRARY_TYPE:STRING=SO ../..
	make all
	if test $? -ne 0
	then
		cd ${OLDPWD}
		exit 1
	fi
	make allInOneHeader
	cp -fv libCrashC.so.1.0 ../Production/
	cp -fv allInOne/crashC.all.in.one.h ../Production/
	cmake -DU_LIBRARY_TYPE:STRING=AO ../..
	make all
	if test $? -ne 0
	then
		cd ${OLDPWD}
		exit 1
	fi
	cp -fv libCrashC.a ../Production/
cd ${OLDPWD}


echo "Done! You can fuind everything inside build/Production!"
