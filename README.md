# crashc

Building
--------

    mkdir -p build/Debug
    cd build/Debug
    cmake ../..
    make
    #launch tests
    ./Test
    
If you want to test memory leaks, just use:
    
    valgrind ./Test    
    
