## CrashC Codestyle - Version 1.1 - January 2018

In order to keep the CrashC project codebase as clean as possible  
we follow a set of conventions in the way we name types, variables  
and write code. Anyone willing to contribute to the framework **must**  
follow these rules.

### General codestyle

1. Identifiers are written using snake case: e.g `this_is_an_identifier`. No uppercase letters are admitted
2. Pointer types are declared with the asterisk on the **right** side of the **type**, with no spaces: e.g `int* pointer`
_SECTION UNDER CONSTRUCTION_

### Type definitions

In CrashC development we use a specific convention regarding type definitions and usage aimed at obtaining  
both clearness and readability.

Firstly, we don't use **any** typedef for structs, enums and unions. Even though using tags might add some bloat to  
the code, we believe that _indiscriminate_ typedefs would "hide" the real nature of the data type we are using, thus  
limiting the comprehensibility of the code for a new developer.

On the other side, a well-thought use of typedef can also _greatly_ increase code readability and cleanness.  
For this reason, we use them in 2 possible scenarios:
* Function pointer types
* Opaque types
The reasons are simple: C's function pointers syntax is just too annoying to carry around the code, and opaque types must hide their implementation details.  
For these two cases we also use different suffixes in order to distinguish between them:
* "\_c" for function pointers (the "c " stands for _callback_ )
* "\_o" for opaque types

### Public identifiers conventions

With regards to public identifiers (i.e exported function names, types and global variables and _certain_ macros) we prepend  
the "ct\_" in order to, at least _try_, to avoid name clashes with the user code.

**_DOCUMENT UNDER CONSTRUCTION_**