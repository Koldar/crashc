/**
 * @file
 *
 * Module containing functions related to signal handling
 *
 * @author noodles
 * @date Jul 17, 2017
 */

#ifndef SIG_HANDLING_H_
#define SIG_HANDLING_H_

#include <signal.h>
#include <setjmp.h>

#include "crashc.h"
#include "section.h"

/**
 * Used by longjmp, no special meaning at the moment
 */
#ifdef CT_SIGNAL_JUMP_CODE
#	error "CrashC - CT_SIGNAL_JUMP_CODE macro already defined!"
#endif
#define CT_SIGNAL_JUMP_CODE 1

/**
 * Used by struct ct_model::_crashc_sigaction to indicate an empty bitmask
 */
#ifdef CT_NO_FLAGS
#	error "CrashC - CT_NO_FLAGS macro already defined!"
#endif
#define CT_NO_FLAGS 0

/**
 * Registers all the signal @crashc test framework wants to handle
 *
 * \post
 * 	\li struct ct_model::_crashc_sigaction manages all signals
 */
void ct_register_signal_handlers();

#endif
