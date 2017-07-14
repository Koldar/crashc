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
#include "crashC.h" //Mi serve davvero
#include "sigHandling.h"

/**
 * These macros represent the fail messages associated to CrashC-handled
 * bad signals test termination. Since they are to be used only by this
 * source file, they are not included in sigHandling.h
 */
 #define SIGSEGV_FAIL_MSG "SIGSEGV received: test caused segmentation fault"
 #define SIGFPE_FAIL_MSG "SIGFPE received: test caused a floating point exception"


/**
 * This variable is used to keep track of whenever we are handling a fatal
 * signal. It needs to be an atomic type as we must be sure that the
 * update of the variable cannot be interrupted
 */
 volatile sig_atomic_t handling_fatal_signal = 0;

 /**
  * This variable is used to store the execution state to be restored thanks to
  * setjmp and longjmp when needed during faulty test execution.
  * By faulty test we mean tests which generate SIGSEGV or SIGFPE.
  */
  jmp_buf signal_jump_point;

/**
 * This handler is used for every signal whose delivery is considered by CrashC to
 * imply a failure in the running test. This handler is used only if the programmer
 * does not register any other signal handler for the particular signal being considered.
 * What we need to do is mark the current running test as failed and update its status
 * in order that it is not run again on the next LOOPER iteration.
 *
 */
void failsig_handler() {
    //We check wheter or not we were already handling the signal in case that
    //other fatal signals are delivered during the previous handling of the
    //signal
    if (!handling_fatal_signal) {
        handling_fatal_signal = 1;

        //We mark the currently executed section as executed and jump back to LOOPER
        markSectionAsExecuted(currentSection);
        //Mark test as failed?
        longjmp(signal_jump_point, JMP_CODE);
    }
}
