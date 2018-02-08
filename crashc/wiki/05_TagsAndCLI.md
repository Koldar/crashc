Introduction
============

In this page we will explain what are the **tags** and how they can be used to tweak which tests you want to run.

Especially in big projects, tests may need quite some times to run. Usually, while developing, you might want to test only some specific tests
in order to understand whether or not the code you're building is working. After ensuring it, usually you want to test the code against all
your suites. 

Tags provide an easy solution to achieve this. Again, this is nothing new: our insipration project, [Catch Cpp](https://github.com/philsquared/Catch) had this
feature since a long time.

How to use tags?
================

Consider the example from [Meet the Powerhouse](https://github.com/Koldar/crashc/blob/master/wiki/03_MeetThePowerhouse.md):

```
#include <crashC.h>

TESTS_START
REGISTER_SUITE(1);
TESTS_END

TESTSUITE(1) {
    TESTCASE("list", "") {
        list* l = initList();
        assert(isEmpty(l));
        assert(getSize(l)==0);
        WHEN("adding on tail", "") {
            addTail(l, 5);
            THEN("size increments","") {
                assert(!isEmpty(l));
                assert(getSize(l)==1);
            }
        }
        WHEN("adding on head", "") {
            addHead(l, 3);
            THEN("size increments", "") {
                assert(!isEmpty(l));
                assert(getSize(l)==1);
            }
        }
        WHEN("lists of 3 items", "") {
            addTail(l, 5);
            addHead(l, 3);
            addTail(l, 7);
            THEN("size is 3", "") {
                assert(!isEmpty(l));
                assert(getSize(l)==3);    
            }
            WHEN("checking presence", "") {
                assert(isPresent(l, 7));
                assert(!isPresent(l, 4));
            }  
            WHEN("removing present item", "") {
                removeItem(l, 7);
                THEN("size decrements", "") {
                    assert(!isPresent(l, 7));
                    assert(!isEmpty(l));
                    assert(getSize(l)==2);
                }
            }
            WHEN("removing absent item", "") {
                removeItem(l, 4);
                THEN("size is unchanged", "") {
                    assert(!isPresent(l, 4));
                    assert(!isEmpty(l));
                    assert(getSize(l)==3);
                }
            }
        }
        destroyList(l);
    }
}
```

Tags are strings that decorates a crashC concept. A concept can be decorated with as many tags as you want. Just separate them with spaces.
Usually tags are defined in the second parameter of the concept (read "second parameter of `THEN`, `WHEN`, `TESTCASE` and so on"). For example:

```
THEN("size increments","hello world") {
     assert(!isEmpty(l));
     assert(getSize(l)==1);
}
```

The previous `THEN` has 2 tags: `hello` and `world`.
There are 2 different scenarios: testing only something or exclude from testing something.

Testing only something
----------------------

 In this case you need to add, for the test you want, a tag. For example let's create an "hello" tag for the first `WHEN` in `TESTCASE`:
 
    WHEN("adding on tail", "hello") {
        addTail(l, 5);
        THEN("size increments","") {
            assert(!isEmpty(l));
            assert(getSize(l)==1);
        }
    }
 
 Super easy, isn't it? Now, to tell CrashC you want to run the sections with tag "hello" you need to call the executable with the following parameters, like this:
 
 ```
 ./Test --include_tag="hello"
 ```
 
 Exclude something from testing
 ------------------------------
 
 If you want to exclude some section from testing, you can add those sections with a tag and then add the following parameters when calling the test:
 
 
 ```
 ./Test --exclude_tag="hello"
 ```
 
