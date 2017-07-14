#include <signal.h>
#include <setjmp.h>

//Used by longjmp, no special meaning at the moment
#define JMP_CODE 1

extern jmp_buf signal_jump_point;

void failsig_handler();
