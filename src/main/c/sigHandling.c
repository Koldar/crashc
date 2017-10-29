/**
 * Signals handler functions.
 * These functions are used by CrashC to control its behaviour when certain signals
 * are delivered. For example, if a tested function tries to access invalid memory,
 * and it receives a SIGSEGV signal, CrashC terminates the function, proceeds with
 * the next one and reports about the error in the final test report.
 *
 * Note that CrashC behaviour is designed to let the programmer use his own signal
 * handling routines, in order not to limitate the possible tests types supported by
 * the library. CrashC in fact assumes that if the programmer did not specify any
 * custom signal handler, any possible "bad" signal is to be considered an error
 * that the user wants to be noticed about.
 *
 * \author Lorenzo Nodari <lorenzo.nodari.it@ieee.org>
 *
 *
 */

#include "sigHandling.h"
#include "main_model.h"

static void failsig_handler(int signum);

void registerSignalHandlerForSignals() {
	(&cc_model)->_crashc_sigaction.sa_handler = &failsig_handler;
	//register signals
	//TODO add even this signals
//	if (sigaction(SIGHUP, &sa, NULL) == -1) {
//		perror("Error: cannot handle SIGHUP"); //Should not happen
//	}
//
//	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
//		perror("Error: cannot handle SIGUSR1"); //should not happen
//	}
//
//	if (sigaction(SIGINT, &sa, NULL) == -1) {
//		perror("Error: cannot handle SIGUSR1"); //should not happen
//	}
	//TODO do not register signal if it  is already registers by the program under test itself
	if (sigaction(SIGFPE, &((&cc_model)->_crashc_sigaction), NULL) == -1) {
		perror("Error: cannot handle SIGUSR1"); //should not happen
	}
}

/**
 * Code to be run when a signal was not handled by the program under test itself
 *
 * This handler is used for every signal whose delivery is considered by CrashC to
 * imply a failure in the running test. This handler is used only if the programmer
 * does not register any other signal handler for the particular signal being considered.
 * What we need to do is mark the current running test as failed and update its status
 * in order that it is not run again on the next LOOPER iteration.
 *
 * @param signum an ID representing the signal detected
 *
 */
static void failsig_handler(int signum) {

	printf("marking section \"%s\" as signal detected!\n", (&cc_model)->currentSection->description);
    //Mark test as failed code
	markSectionAsSignalDetected((&cc_model)->currentSection);
	(&cc_model)->currentSection->signalDetected = signum;

	//after handling the signal we return to sigsetjmp function (we will enter in the "if" where sigsetjmp is located)
    siglongjmp((&cc_model)->signal_jump_point, 1);
}
