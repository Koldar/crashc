Introduction
============

This quick document will explain how to setup as quick as possible CrashC. You should nneed at most 3 minutes to do so

Install CrashC on your system
=============================

You can download the all the version (here)[]. Choose the latest one for the most stable and updated version.
Otherwise you can build the framework by yourself. In order to do so:

    cd <MY_COOL_FOLDER>
    git checkout https://github.com/Koldar/crashc.git
    cd crashc
    mkdir -p build/Release
    cd build/Release
    cmake -DCRASH_OUTPUT="XXX" ../..
    make
    make allInOneHeader //run if you want to create an header including everything in crashC
    sudo make install
    
where "XXX" can be one of the possible choices:

- SHARED_LIBRARY (the **default** one)
- STATIC_LIBRARY
- ALL_IN_ONE_HEADER

What's next?
============

All done? go to the next phase [quick tutorial](https://github.com/Koldar/crashc/blob/master/wiki/02_QuickTour.md)!
