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

 /**
  * This variable is used to store the execution state to be restored thanks to
  * setjmp and longjmp when needed during faulty test execution.
  * By faulty test we mean tests which generate SIGSEGV or SIGFPE.
  */
jmp_buf signal_jump_point;
struct sigaction _crashc_sigaction;

/**
 * This function is used inside the TESTCASE, aka LOOPER, in the boolean condition section
 * to install the jump point used by the fail signal handling routines to abort the current
 * test and go on with the others
 * The functions also checks if the user installed any custom signal handler and, if not,
 * installs CrashC handler for SIGSEGV, SIGFPE and SIGBUS
 */
bool haveWeRunEveryChildrenAndSignalHandlingSetup(Section * section) {
//    //Here we set out signal handler if the user didn't set any
//    sigset_t blocked_signals;
//    struct sigaction old_action;
//    struct sigaction crashc_action;
//
//    //Initializes sigaction structure for our handler
//    sigemptyset(&blocked_signals);
//    crashc_action = (struct sigaction) {
//        failsig_handler,
//        blocked_signals,
//        NO_FLAGS
//    };
//
//    //Check if SIGSEGV handler was changed
//    sigaction(SIGSEGV, NULL, &old_action);
//    if (old_action.sa_handler == SIG_DFL || old_action.sa_handler == SIG_IGN) {
//        sigaction(SIGSEGV, &crashc_action, NULL);
//    }
//
//    //Check if SIGFPE handler was changed
//    sigaction(SIGFPE, NULL, &old_action);
//    if (old_action.sa_handler == SIG_DFL || old_action.sa_handler == SIG_IGN) {
//        sigaction(SIGFPE, &crashc_action, NULL);
//    }
//
//    //Check if SIGBUS handler was changed
//    sigaction(SIGBUS, NULL, &old_action);
//    if (old_action.sa_handler == SIG_DFL || old_action.sa_handler == SIG_IGN) {
//        sigaction(SIGBUS, &crashc_action, NULL);
//    }

    return haveWeRunEveryChildrenInSection(section);
}

void registerSignalHandlerForSignals() {
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
	if (sigaction(SIGFPE, &_crashc_sigaction, NULL) == -1) {
		perror("Error: cannot handle SIGUSR1"); //should not happen
	}
}

/**
 * This handler is used for every signal whose delivery is considered by CrashC to
 * imply a failure in the running test. This handler is used only if the programmer
 * does not register any other signal handler for the particular signal being considered.
 * What we need to do is mark the current running test as failed and update its status
 * in order that it is not run again on the next LOOPER iteration.
 *
 */
void failsig_handler(int signum) {

    //Mark test as failed code//

    longjmp(signal_jump_point, JMP_CODE);
}
