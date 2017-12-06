#ifndef SIG_HANDLING_H_
#define SIG_HANDLING_H_

#include <signal.h>
#include <setjmp.h>

#include "crashC.h"
#include "section.h"

//Used by longjmp, no special meaning at the moment
#define SIGNAL_JUMP_CODE 1
//Used by crashc_sigaction_struct to indicate an empty bitmask
#define NO_FLAGS 0

/**
 * Registers all the signal CRASHC test framework wants to handle
 *
 * \post
 * 	\li ::_crashc_sigaction manages SIGFPE signal
 */
void ct_register_signal_handlers();

#endif
