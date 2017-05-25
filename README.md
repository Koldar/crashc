# crashc

Building
--------

    mkdir -p build
    cd build
    cmake ..
    make
    #launch tests
    ./Test
    
If you want to test memory leaks, just use:
    
    valgrind ./Test    
    
