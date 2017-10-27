Introduction
============

This quick document will explain how to setup as quick as possible CrashC. You should nneed at most 3 minutes to do so

Install CrashC on your system
=============================

You can download the all the version (here)[]. Choose the latest one for the most stable and updated version.
Otherwise you can build the framework by yourself. In order to do so:

    cd <MY_COOL_FOLDER>
    git checkout <git url>
    cd crashc
    mkdir -p build/Release
    cd build/Release
    cmake -DCRASH_OUTPUT="XXX" ../..
    make
    sudo make install
    
where "XXX" can be one of the possible choices:

- SHARED_LIBRARY
- STATIC_LIBRARY
- ALL_IN_ONE_HEADER (the **default** one)

What's next?
============

All done? go to the next phase (quick tutorial)[]!
