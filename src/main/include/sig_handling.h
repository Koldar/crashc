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
#define SIGNAL_JUMP_CODE 1
/**
 * Used by ct_model_t::_crashc_sigaction to indicate an empty bitmask
 */
#define NO_FLAGS 0

/**
 * Registers all the signal @crashc test framework wants to handle
 *
 * \post
 * 	\li ct_model_t::_crashc_sigaction manages all signals
 */
void ct_register_signal_handlers();

#endif
