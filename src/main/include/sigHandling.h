#ifndef SIGHANDLING_H_
#define SIGHANDLING_H_

#include <signal.h>
#include <setjmp.h>
#include "section.h"
#include "crashC.h"

//Used by longjmp, no special meaning at the moment
#define JMP_CODE 1
//Used by crashc_sigaction_struct to indicate an empty bitmask
#define NO_FLAGS 0

void failsig_handler(int);

/**
 * Represents the structure used by crashc to handle signals
 */
extern struct sigaction _crashc_sigaction;
extern jmp_buf signal_jump_point;

/**
 * Registers all the signal CRASHC test framework wants to handle
 *
 * \post
 * 	\li ::_crashc_sigaction manages SIGFPE signal
 */
void registerSignalHandlerForSignals();

#endif
