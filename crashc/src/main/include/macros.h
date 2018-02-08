/**
 * @file
 *
 * Module containing a set of general purpose macros used throughout all the project
 *
 * @author koldar
 * @date Jun 29, 2017
 */

#ifndef MACROS_H_
#define MACROS_H_

/**
 * the character used to divide tags inside a single string
 *
 * Setting this macro to '#' means that the tags are required to be defined as "hello#world".
 * See @ref tags for further information
 */
#ifndef CT_TAGS_SEPARATOR
#	define CT_TAGS_SEPARATOR ' '
#endif

/**
 * The size of temporary buffers created, when needed, throughout the project
 *
 * Enlarge such buffer if you happen to have to deal with longer strings
 */
#ifndef CT_BUFFER_SIZE
#	define CT_BUFFER_SIZE 300
#endif

/**
 * macro used to perform macro concatenation.
 *
 * @note
 * do not used this directly. Use ::CT_TOKENPASTE2 instead
 *
 * @param[in] x the first identifier to concat
 * @param[in] y the second identifier to concat
 * @return \c x concatenated with \c y
 * @see CT_TOKENPASTE2
 */
#ifdef CT_TOKENPASTE
#	error "CrashC - CT_TOKENPASTE macro is already defined!"
#endif
#define CT_TOKENPASTE(x, y) x ## y
/**
 * Macro used to perform concatenation.
 *
 * @note
 * this is the macro you want to use to perform concatenation
 *
 * @param[in] x the first identifier to concat
 * @param[in] y the second identifier to concat
 * @return \c x concatenated with \c y
 */
#ifdef CT_TOKENPASTE2
#	error "CrashC - CT_TOKENPASTE2 macro is already defined!"
#endif
#define CT_TOKENPASTE2(x, y) CT_TOKENPASTE(x, y)

/**
 * Macro used to paste to a string \c v another sting, making \c v unique.
 *
 * This macro is incredible useful to produce variables names which you need to ensure are unique.
 * For example they are useful in for loops where you don't actually care about the names of the counter, but you only
 * want to fetch some data from a collection. For example:
 *
 * @code
 * int CT_UV(a); //produce a12 assuming the code is on line 12
 * int CT_UV(a); //produce a13 assuming the code is on line 13
 * //produce 2 different "a" variables
 * @endcode
 *
 * @param[in] v code to prepend to the line number
 * @return if \c v is a valid C identifier, this may represent a new unique variable name
 */
#ifdef CT_UV
#	error "CrashC - CT_UV macro is already defined!"
#endif
#define CT_UV(v) CT_TOKENPASTE2(v, __LINE__)

//FOREACH macros, used to iterate on the arguments passed to the REG_SUITES macro
#define CT_FE_0
#define CT_FE_1(WHAT, X) WHAT(X)
#define CT_FE_2(WHAT, X, ...) WHAT(X); CT_FE_1(WHAT, __VA_ARGS__)
#define CT_FE_3(WHAT, X, ...) WHAT(X); CT_FE_2(WHAT, __VA_ARGS__)
#define CT_FE_4(WHAT, X, ...) WHAT(X); CT_FE_3(WHAT, __VA_ARGS__)
#define CT_FE_5(WHAT, X, ...) WHAT(X); CT_FE_4(WHAT, __VA_ARGS__)
#define CT_FE_6(WHAT, X, ...) WHAT(X); CT_FE_5(WHAT, __VA_ARGS__)
#define CT_FE_7(WHAT, X, ...) WHAT(X); CT_FE_6(WHAT, __VA_ARGS__)
#define CT_FE_8(WHAT, X, ...) WHAT(X); CT_FE_7(WHAT, __VA_ARGS__)
#define CT_FE_9(WHAT, X, ...) WHAT(X); CT_FE_8(WHAT, __VA_ARGS__)
#define CT_FE_10(WHAT, X, ...) WHAT(X); CT_FE_9(WHAT, __VA_ARGS__)
#define CT_FE_11(WHAT, X, ...) WHAT(X); CT_FE_10(WHAT, __VA_ARGS__)
#define CT_FE_12(WHAT, X, ...) WHAT(X); CT_FE_11(WHAT, __VA_ARGS__)
#define CT_FE_13(WHAT, X, ...) WHAT(X); CT_FE_12(WHAT, __VA_ARGS__)
#define CT_FE_14(WHAT, X, ...) WHAT(X); CT_FE_13(WHAT, __VA_ARGS__)
#define CT_FE_15(WHAT, X, ...) WHAT(X); CT_FE_14(WHAT, __VA_ARGS__)
#define CT_FE_16(WHAT, X, ...) WHAT(X); CT_FE_15(WHAT, __VA_ARGS__)
#define CT_FE_17(WHAT, X, ...) WHAT(X); CT_FE_16(WHAT, __VA_ARGS__)
#define CT_FE_18(WHAT, X, ...) WHAT(X); CT_FE_17(WHAT, __VA_ARGS__)
#define CT_FE_19(WHAT, X, ...) WHAT(X); CT_FE_18(WHAT, __VA_ARGS__)
#define CT_FE_20(WHAT, X, ...) WHAT(X); CT_FE_19(WHAT, __VA_ARGS__)
#define CT_FE_21(WHAT, X, ...) WHAT(X); CT_FE_20(WHAT, __VA_ARGS__)
#define CT_FE_22(WHAT, X, ...) WHAT(X); CT_FE_21(WHAT, __VA_ARGS__)
#define CT_FE_23(WHAT, X, ...) WHAT(X); CT_FE_22(WHAT, __VA_ARGS__)
#define CT_FE_24(WHAT, X, ...) WHAT(X); CT_FE_23(WHAT, __VA_ARGS__)
#define CT_FE_25(WHAT, X, ...) WHAT(X); CT_FE_24(WHAT, __VA_ARGS__)
#define CT_FE_26(WHAT, X, ...) WHAT(X); CT_FE_25(WHAT, __VA_ARGS__)
#define CT_FE_27(WHAT, X, ...) WHAT(X); CT_FE_26(WHAT, __VA_ARGS__)
#define CT_FE_28(WHAT, X, ...) WHAT(X); CT_FE_27(WHAT, __VA_ARGS__)
#define CT_FE_29(WHAT, X, ...) WHAT(X); CT_FE_28(WHAT, __VA_ARGS__)
#define CT_FE_30(WHAT, X, ...) WHAT(X); CT_FE_29(WHAT, __VA_ARGS__)
#define CT_FE_31(WHAT, X, ...) WHAT(X); CT_FE_30(WHAT, __VA_ARGS__)
#define CT_FE_32(WHAT, X, ...) WHAT(X); CT_FE_31(WHAT, __VA_ARGS__)
#define CT_FE_33(WHAT, X, ...) WHAT(X); CT_FE_32(WHAT, __VA_ARGS__)
#define CT_FE_34(WHAT, X, ...) WHAT(X); CT_FE_33(WHAT, __VA_ARGS__)
#define CT_FE_35(WHAT, X, ...) WHAT(X); CT_FE_34(WHAT, __VA_ARGS__)
#define CT_FE_36(WHAT, X, ...) WHAT(X); CT_FE_35(WHAT, __VA_ARGS__)
#define CT_FE_37(WHAT, X, ...) WHAT(X); CT_FE_36(WHAT, __VA_ARGS__)
#define CT_FE_38(WHAT, X, ...) WHAT(X); CT_FE_37(WHAT, __VA_ARGS__)
#define CT_FE_39(WHAT, X, ...) WHAT(X); CT_FE_38(WHAT, __VA_ARGS__)
#define CT_FE_40(WHAT, X, ...) WHAT(X); CT_FE_39(WHAT, __VA_ARGS__)
#define CT_FE_41(WHAT, X, ...) WHAT(X); CT_FE_40(WHAT, __VA_ARGS__)
#define CT_FE_42(WHAT, X, ...) WHAT(X); CT_FE_41(WHAT, __VA_ARGS__)
#define CT_FE_43(WHAT, X, ...) WHAT(X); CT_FE_42(WHAT, __VA_ARGS__)
#define CT_FE_44(WHAT, X, ...) WHAT(X); CT_FE_43(WHAT, __VA_ARGS__)
#define CT_FE_45(WHAT, X, ...) WHAT(X); CT_FE_44(WHAT, __VA_ARGS__)
#define CT_FE_46(WHAT, X, ...) WHAT(X); CT_FE_45(WHAT, __VA_ARGS__)
#define CT_FE_47(WHAT, X, ...) WHAT(X); CT_FE_46(WHAT, __VA_ARGS__)
#define CT_FE_48(WHAT, X, ...) WHAT(X); CT_FE_47(WHAT, __VA_ARGS__)
#define CT_FE_49(WHAT, X, ...) WHAT(X); CT_FE_48(WHAT, __VA_ARGS__)
#define CT_FE_50(WHAT, X, ...) WHAT(X); CT_FE_49(WHAT, __VA_ARGS__)
#define CT_FE_51(WHAT, X, ...) WHAT(X); CT_FE_50(WHAT, __VA_ARGS__)
#define CT_FE_52(WHAT, X, ...) WHAT(X); CT_FE_51(WHAT, __VA_ARGS__)
#define CT_FE_53(WHAT, X, ...) WHAT(X); CT_FE_52(WHAT, __VA_ARGS__)
#define CT_FE_54(WHAT, X, ...) WHAT(X); CT_FE_53(WHAT, __VA_ARGS__)
#define CT_FE_55(WHAT, X, ...) WHAT(X); CT_FE_54(WHAT, __VA_ARGS__)
#define CT_FE_56(WHAT, X, ...) WHAT(X); CT_FE_55(WHAT, __VA_ARGS__)
#define CT_FE_57(WHAT, X, ...) WHAT(X); CT_FE_56(WHAT, __VA_ARGS__)
#define CT_FE_58(WHAT, X, ...) WHAT(X); CT_FE_57(WHAT, __VA_ARGS__)
#define CT_FE_59(WHAT, X, ...) WHAT(X); CT_FE_58(WHAT, __VA_ARGS__)
#define CT_FE_60(WHAT, X, ...) WHAT(X); CT_FE_59(WHAT, __VA_ARGS__)
#define CT_FE_61(WHAT, X, ...) WHAT(X); CT_FE_60(WHAT, __VA_ARGS__)
#define CT_FE_62(WHAT, X, ...) WHAT(X); CT_FE_61(WHAT, __VA_ARGS__)
#define CT_FE_63(WHAT, X, ...) WHAT(X); CT_FE_62(WHAT, __VA_ARGS__)
#define CT_FE_64(WHAT, X, ...) WHAT(X); CT_FE_63(WHAT, __VA_ARGS__)

/**
 * Macro used to implement macro variadic arguments
 */
#ifdef CT_GET_FE_NAME
#	error "CrashC - CT_GET_FE_NAME macro is already defined!"
#endif
#define CT_GET_FE_NAME( 										\
          _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, 		\
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, 			\
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, 			\
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, 			\
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, 			\
         _51, _52, _53, _54, _55, _56, _57, _58, _59, 		\
         _60, _61, _62, _63, name, ...) name

#ifdef CT_VARIADIC_MACRO
#	error "CrashC - CT_VARIADIC_MACRO macro is already defined!"
#endif
#define CT_VARIADIC_MACRO(name, ...) CT_GET_FE_NAME(__VA_ARGS__, CT_FE_64, CT_FE_63, CT_FE_62, 				\
    CT_FE_61, CT_FE_60, CT_FE_59, CT_FE_58, CT_FE_57, CT_FE_56, CT_FE_55, CT_FE_54, CT_FE_53, CT_FE_52, CT_FE_51,CT_FE_50, 			\
    CT_FE_49, CT_FE_48, CT_FE_47,CT_FE_46,CT_FE_45,CT_FE_44,CT_FE_43,CT_FE_42,CT_FE_41,CT_FE_40, 								\
    CT_FE_39,CT_FE_38,CT_FE_37,CT_FE_36,CT_FE_35,CT_FE_34,CT_FE_33,CT_FE_32,CT_FE_31,CT_FE_30, 								\
    CT_FE_29,CT_FE_28,CT_FE_27,CT_FE_26,CT_FE_25,CT_FE_24,CT_FE_23,CT_FE_22,CT_FE_21,CT_FE_20, 								\
    CT_FE_19,CT_FE_18,CT_FE_17,CT_FE_16,CT_FE_15,CT_FE_14,CT_FE_13,CT_FE_12,CT_FE_11,CT_FE_10, 								\
    CT_FE_9,CT_FE_8,CT_FE_7,CT_FE_6,CT_FE_5,CT_FE_4,CT_FE_3,CT_FE_2,CT_FE_1, CT_FE_0)(name, __VA_ARGS__)


#endif /* MACROS_H_ */
