#ifndef CRASHC_ALL_IN_ONE_H_
#define CRASHC_ALL_IN_ONE_H_
/**
 * @file
 *
 * Contains all the general purpose typedefs used in @crashc
 *
 * @author koldar
 * @date Oct 11, 2017
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

struct ct_model;
struct ct_section;
struct ct_test_report;
struct ct_snapshot;

/**
 * Represents the signature of a function which release a structure from the memory
 *
 * @param[inout] obj the structure to release from the memory. See <a ref="http://yarchive.net/comp/const.html">Linus thread</a>
 * for the rationale behind *const*.
 */
typedef void (*ct_destroyer_c)(const void* obj);

/**
 * Function signature used to point at the functions which represent @testsuite
 */
typedef void (*ct_test_c)(void);

/**
 * Function signature for a teardown function parameter
 *
 * The type of the function pointer of the global teardown function
 *
 * @see ct_model::ct_teardown
 */
typedef void (*ct_teardown_c)(void);

/**
 * @addtogroup reportFunctionType Report Function Types
 * @brief function types used in ::ct_report_producer in order to build a test report human readable
 * @{
 */

/**
 * This type defines the function pointer to the function used to produce the report of a single test.
 *
 * This function usually calls a ct_snapshot_tree_reporter.
 *
 * @param[inout] model the model under analysis
 * @param[in] test_report the test report we need to create a human readable representation of
 */
typedef void (*ct_test_reporter_c)(struct ct_model* model, struct ct_test_report* test_report);

/**
 * This type defines the function pointer to the function used to produce the report of a snapshot tree
 *
 * @param[inout] model the model under analysis
 * @param[in] snapshot the ::ct_snapshot we need to create a human readable representation of
 * @param[in] level the depth (in the snapshot tree) of the \c snapshot
 */
typedef void (*ct_snapshot_tree_reporter_c)(struct ct_model* model, struct ct_snapshot* snapshot, int level);

/**
 * This type defines the function pointer to the function used to produce the tests summary.
 *
 * The statistics of the tests, if used by the model, can be reached through it, so they are not passed
 * as an argument explicitly
 *
 * @param[inout] model the model to use to create the human readable report of the statistics
 */
typedef void (*ct_summary_reporter_c)(struct ct_model* model);

/**
 * This type defines the function pointer to the function used to produce the reports for the assertions executed during a specific snapshot.
 *
 * @param[inout] model the model under analysis
 * @param[in] snapshot the ::ct_snapshot containing the relevant asseritons. This argument **can be used** to get the list of struct ct_assert_report.
 * @param[in] level the depth (in the snapshot tree) of the \c snapshot
 */
typedef void (*ct_assert_reporter_c)(struct ct_model* model, struct ct_snapshot* snapshot, int level);

/**
 * function pointer type used to create the whole report by calling the other \ref reportFunctionType.
 *
 * The list of test reports is passed through the model. In the same way it is possible to reach
 * the report_producer implementation.
 *
 * @param[inout] model the model to consider
 */
typedef void (*ct_reporter_c)(struct ct_model* model);

///@}

#endif /* TYPEDEFS_H_ */
/*
Copyright (c) 2003-2016, Troy D. Hanson     http://troydhanson.github.com/uthash/
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef UTHASH_H
#define UTHASH_H

#define UTHASH_VERSION 2.0.1

#include <string.h>   /* memcmp,strlen */
#include <stddef.h>   /* ptrdiff_t */
#include <stdlib.h>   /* exit() */

/* These macros use decltype or the earlier __typeof GNU extension.
   As decltype is only available in newer compilers (VS2010 or gcc 4.3+
   when compiling c++ source) this code uses whatever method is needed
   or, for VS2008 where neither is available, uses casting workarounds. */
#if defined(_MSC_VER)   /* MS compiler */
#if _MSC_VER >= 1600 && defined(__cplusplus)  /* VS2010 or newer in C++ mode */
#define DECLTYPE(x) (decltype(x))
#else                   /* VS2008 or older (or VS2010 in C mode) */
#define NO_DECLTYPE
#define DECLTYPE(x)
#endif
#elif defined(__BORLANDC__) || defined(__LCC__) || defined(__WATCOMC__)
#define NO_DECLTYPE
#define DECLTYPE(x)
#else                   /* GNU, Sun and other compilers */
#define DECLTYPE(x) (__typeof(x))
#endif

#ifdef NO_DECLTYPE
#define DECLTYPE_ASSIGN(dst,src)                                                 \
do {                                                                             \
  char **_da_dst = (char**)(&(dst));                                             \
  *_da_dst = (char*)(src);                                                       \
} while (0)
#else
#define DECLTYPE_ASSIGN(dst,src)                                                 \
do {                                                                             \
  (dst) = DECLTYPE(dst)(src);                                                    \
} while (0)
#endif

/* a number of the hash function use uint32_t which isn't defined on Pre VS2010 */
#if defined(_WIN32)
#if defined(_MSC_VER) && _MSC_VER >= 1600
#include <stdint.h>
#elif defined(__WATCOMC__) || defined(__MINGW32__) || defined(__CYGWIN__)
#include <stdint.h>
#else
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
#endif
#elif defined(__GNUC__) && !defined(__VXWORKS__)
#include <stdint.h>
#else
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
#endif

#ifndef uthash_fatal
#define uthash_fatal(msg) exit(-1)        /* fatal error (out of memory,etc) */
#endif
#ifndef uthash_malloc
#define uthash_malloc(sz) malloc(sz)      /* malloc fcn                      */
#endif
#ifndef uthash_free
#define uthash_free(ptr,sz) free(ptr)     /* free fcn                        */
#endif
#ifndef uthash_strlen
#define uthash_strlen(s) strlen(s)
#endif
#ifndef uthash_memcmp
#define uthash_memcmp(a,b,n) memcmp(a,b,n)
#endif

#ifndef uthash_noexpand_fyi
#define uthash_noexpand_fyi(tbl)          /* can be defined to log noexpand  */
#endif
#ifndef uthash_expand_fyi
#define uthash_expand_fyi(tbl)            /* can be defined to log expands   */
#endif

/* initial number of buckets */
#define HASH_INITIAL_NUM_BUCKETS 32U     /* initial number of buckets        */
#define HASH_INITIAL_NUM_BUCKETS_LOG2 5U /* lg2 of initial number of buckets */
#define HASH_BKT_CAPACITY_THRESH 10U     /* expand when bucket count reaches */

/* calculate the element whose hash handle address is hhp */
#define ELMT_FROM_HH(tbl,hhp) ((void*)(((char*)(hhp)) - ((tbl)->hho)))
/* calculate the hash handle from element address elp */
#define HH_FROM_ELMT(tbl,elp) ((UT_hash_handle *)(((char*)(elp)) + ((tbl)->hho)))

#define HASH_VALUE(keyptr,keylen,hashv)                                          \
do {                                                                             \
  HASH_FCN(keyptr, keylen, hashv);                                               \
} while (0)

#define HASH_FIND_BYHASHVALUE(hh,head,keyptr,keylen,hashval,out)                 \
do {                                                                             \
  (out) = NULL;                                                                  \
  if (head) {                                                                    \
    unsigned _hf_bkt;                                                            \
    HASH_TO_BKT(hashval, (head)->hh.tbl->num_buckets, _hf_bkt);                  \
    if (HASH_BLOOM_TEST((head)->hh.tbl, hashval) != 0) {                         \
      HASH_FIND_IN_BKT((head)->hh.tbl, hh, (head)->hh.tbl->buckets[ _hf_bkt ], keyptr, keylen, hashval, out); \
    }                                                                            \
  }                                                                              \
} while (0)

#define HASH_FIND(hh,head,keyptr,keylen,out)                                     \
do {                                                                             \
  unsigned _hf_hashv;                                                            \
  HASH_VALUE(keyptr, keylen, _hf_hashv);                                         \
  HASH_FIND_BYHASHVALUE(hh, head, keyptr, keylen, _hf_hashv, out);               \
} while (0)

#ifdef HASH_BLOOM
#define HASH_BLOOM_BITLEN (1UL << HASH_BLOOM)
#define HASH_BLOOM_BYTELEN (HASH_BLOOM_BITLEN/8UL) + (((HASH_BLOOM_BITLEN%8UL)!=0UL) ? 1UL : 0UL)
#define HASH_BLOOM_MAKE(tbl)                                                     \
do {                                                                             \
  (tbl)->bloom_nbits = HASH_BLOOM;                                               \
  (tbl)->bloom_bv = (uint8_t*)uthash_malloc(HASH_BLOOM_BYTELEN);                 \
  if (!((tbl)->bloom_bv))  { uthash_fatal( "out of memory"); }                   \
  memset((tbl)->bloom_bv, 0, HASH_BLOOM_BYTELEN);                                \
  (tbl)->bloom_sig = HASH_BLOOM_SIGNATURE;                                       \
} while (0)

#define HASH_BLOOM_FREE(tbl)                                                     \
do {                                                                             \
  uthash_free((tbl)->bloom_bv, HASH_BLOOM_BYTELEN);                              \
} while (0)

#define HASH_BLOOM_BITSET(bv,idx) (bv[(idx)/8U] |= (1U << ((idx)%8U)))
#define HASH_BLOOM_BITTEST(bv,idx) (bv[(idx)/8U] & (1U << ((idx)%8U)))

#define HASH_BLOOM_ADD(tbl,hashv)                                                \
  HASH_BLOOM_BITSET((tbl)->bloom_bv, (hashv & (uint32_t)((1ULL << (tbl)->bloom_nbits) - 1U)))

#define HASH_BLOOM_TEST(tbl,hashv)                                               \
  HASH_BLOOM_BITTEST((tbl)->bloom_bv, (hashv & (uint32_t)((1ULL << (tbl)->bloom_nbits) - 1U)))

#else
#define HASH_BLOOM_MAKE(tbl)
#define HASH_BLOOM_FREE(tbl)
#define HASH_BLOOM_ADD(tbl,hashv)
#define HASH_BLOOM_TEST(tbl,hashv) (1)
#define HASH_BLOOM_BYTELEN 0U
#endif

#define HASH_MAKE_TABLE(hh,head)                                                 \
do {                                                                             \
  (head)->hh.tbl = (UT_hash_table*)uthash_malloc(                                \
                  sizeof(UT_hash_table));                                        \
  if (!((head)->hh.tbl))  { uthash_fatal( "out of memory"); }                    \
  memset((head)->hh.tbl, 0, sizeof(UT_hash_table));                              \
  (head)->hh.tbl->tail = &((head)->hh);                                          \
  (head)->hh.tbl->num_buckets = HASH_INITIAL_NUM_BUCKETS;                        \
  (head)->hh.tbl->log2_num_buckets = HASH_INITIAL_NUM_BUCKETS_LOG2;              \
  (head)->hh.tbl->hho = (char*)(&(head)->hh) - (char*)(head);                    \
  (head)->hh.tbl->buckets = (UT_hash_bucket*)uthash_malloc(                      \
          HASH_INITIAL_NUM_BUCKETS*sizeof(struct UT_hash_bucket));               \
  if (! (head)->hh.tbl->buckets) { uthash_fatal( "out of memory"); }             \
  memset((head)->hh.tbl->buckets, 0,                                             \
          HASH_INITIAL_NUM_BUCKETS*sizeof(struct UT_hash_bucket));               \
  HASH_BLOOM_MAKE((head)->hh.tbl);                                               \
  (head)->hh.tbl->signature = HASH_SIGNATURE;                                    \
} while (0)

#define HASH_REPLACE_BYHASHVALUE_INORDER(hh,head,fieldname,keylen_in,hashval,add,replaced,cmpfcn) \
do {                                                                             \
  (replaced) = NULL;                                                             \
  HASH_FIND_BYHASHVALUE(hh, head, &((add)->fieldname), keylen_in, hashval, replaced); \
  if (replaced) {                                                                \
     HASH_DELETE(hh, head, replaced);                                            \
  }                                                                              \
  HASH_ADD_KEYPTR_BYHASHVALUE_INORDER(hh, head, &((add)->fieldname), keylen_in, hashval, add, cmpfcn); \
} while (0)

#define HASH_REPLACE_BYHASHVALUE(hh,head,fieldname,keylen_in,hashval,add,replaced) \
do {                                                                             \
  (replaced) = NULL;                                                             \
  HASH_FIND_BYHASHVALUE(hh, head, &((add)->fieldname), keylen_in, hashval, replaced); \
  if (replaced) {                                                                \
     HASH_DELETE(hh, head, replaced);                                            \
  }                                                                              \
  HASH_ADD_KEYPTR_BYHASHVALUE(hh, head, &((add)->fieldname), keylen_in, hashval, add); \
} while (0)

#define HASH_REPLACE(hh,head,fieldname,keylen_in,add,replaced)                   \
do {                                                                             \
  unsigned _hr_hashv;                                                            \
  HASH_VALUE(&((add)->fieldname), keylen_in, _hr_hashv);                         \
  HASH_REPLACE_BYHASHVALUE(hh, head, fieldname, keylen_in, _hr_hashv, add, replaced); \
} while (0)

#define HASH_REPLACE_INORDER(hh,head,fieldname,keylen_in,add,replaced,cmpfcn)    \
do {                                                                             \
  unsigned _hr_hashv;                                                            \
  HASH_VALUE(&((add)->fieldname), keylen_in, _hr_hashv);                         \
  HASH_REPLACE_BYHASHVALUE_INORDER(hh, head, fieldname, keylen_in, _hr_hashv, add, replaced, cmpfcn); \
} while (0)

#define HASH_APPEND_LIST(hh, head, add)                                          \
do {                                                                             \
  (add)->hh.next = NULL;                                                         \
  (add)->hh.prev = ELMT_FROM_HH((head)->hh.tbl, (head)->hh.tbl->tail);           \
  (head)->hh.tbl->tail->next = (add);                                            \
  (head)->hh.tbl->tail = &((add)->hh);                                           \
} while (0)

#define HASH_ADD_KEYPTR_BYHASHVALUE_INORDER(hh,head,keyptr,keylen_in,hashval,add,cmpfcn) \
do {                                                                             \
  unsigned _ha_bkt;                                                              \
  (add)->hh.hashv = (hashval);                                                   \
  (add)->hh.key = (char*) (keyptr);                                              \
  (add)->hh.keylen = (unsigned) (keylen_in);                                     \
  if (!(head)) {                                                                 \
    (add)->hh.next = NULL;                                                       \
    (add)->hh.prev = NULL;                                                       \
    (head) = (add);                                                              \
    HASH_MAKE_TABLE(hh, head);                                                   \
  } else {                                                                       \
    void *_hs_iter = (head);                                                     \
    (add)->hh.tbl = (head)->hh.tbl;                                              \
    do {                                                                         \
      if (cmpfcn(DECLTYPE(head)(_hs_iter), add) > 0)                             \
        break;                                                                   \
    } while ((_hs_iter = HH_FROM_ELMT((head)->hh.tbl, _hs_iter)->next));         \
    if (_hs_iter) {                                                              \
      (add)->hh.next = _hs_iter;                                                 \
      if (((add)->hh.prev = HH_FROM_ELMT((head)->hh.tbl, _hs_iter)->prev)) {     \
        HH_FROM_ELMT((head)->hh.tbl, (add)->hh.prev)->next = (add);              \
      } else {                                                                   \
        (head) = (add);                                                          \
      }                                                                          \
      HH_FROM_ELMT((head)->hh.tbl, _hs_iter)->prev = (add);                      \
    } else {                                                                     \
      HASH_APPEND_LIST(hh, head, add);                                           \
    }                                                                            \
  }                                                                              \
  (head)->hh.tbl->num_items++;                                                   \
  HASH_TO_BKT(hashval, (head)->hh.tbl->num_buckets, _ha_bkt);                    \
  HASH_ADD_TO_BKT((head)->hh.tbl->buckets[_ha_bkt], &(add)->hh);                 \
  HASH_BLOOM_ADD((head)->hh.tbl, hashval);                                       \
  HASH_EMIT_KEY(hh, head, keyptr, keylen_in);                                    \
  HASH_FSCK(hh, head);                                                           \
} while (0)

#define HASH_ADD_KEYPTR_INORDER(hh,head,keyptr,keylen_in,add,cmpfcn)             \
do {                                                                             \
  unsigned _hs_hashv;                                                            \
  HASH_VALUE(keyptr, keylen_in, _hs_hashv);                                      \
  HASH_ADD_KEYPTR_BYHASHVALUE_INORDER(hh, head, keyptr, keylen_in, _hs_hashv, add, cmpfcn); \
} while (0)

#define HASH_ADD_BYHASHVALUE_INORDER(hh,head,fieldname,keylen_in,hashval,add,cmpfcn) \
  HASH_ADD_KEYPTR_BYHASHVALUE_INORDER(hh, head, &((add)->fieldname), keylen_in, hashval, add, cmpfcn)

#define HASH_ADD_INORDER(hh,head,fieldname,keylen_in,add,cmpfcn)                 \
  HASH_ADD_KEYPTR_INORDER(hh, head, &((add)->fieldname), keylen_in, add, cmpfcn)

#define HASH_ADD_KEYPTR_BYHASHVALUE(hh,head,keyptr,keylen_in,hashval,add)        \
do {                                                                             \
  unsigned _ha_bkt;                                                              \
  (add)->hh.hashv = (hashval);                                                   \
  (add)->hh.key = (char*) (keyptr);                                              \
  (add)->hh.keylen = (unsigned) (keylen_in);                                     \
  if (!(head)) {                                                                 \
    (add)->hh.next = NULL;                                                       \
    (add)->hh.prev = NULL;                                                       \
    (head) = (add);                                                              \
    HASH_MAKE_TABLE(hh, head);                                                   \
  } else {                                                                       \
    (add)->hh.tbl = (head)->hh.tbl;                                              \
    HASH_APPEND_LIST(hh, head, add);                                             \
  }                                                                              \
  (head)->hh.tbl->num_items++;                                                   \
  HASH_TO_BKT(hashval, (head)->hh.tbl->num_buckets, _ha_bkt);                    \
  HASH_ADD_TO_BKT((head)->hh.tbl->buckets[_ha_bkt], &(add)->hh);                 \
  HASH_BLOOM_ADD((head)->hh.tbl, hashval);                                       \
  HASH_EMIT_KEY(hh, head, keyptr, keylen_in);                                    \
  HASH_FSCK(hh, head);                                                           \
} while (0)

#define HASH_ADD_KEYPTR(hh,head,keyptr,keylen_in,add)                            \
do {                                                                             \
  unsigned _ha_hashv;                                                            \
  HASH_VALUE(keyptr, keylen_in, _ha_hashv);                                      \
  HASH_ADD_KEYPTR_BYHASHVALUE(hh, head, keyptr, keylen_in, _ha_hashv, add);      \
} while (0)

#define HASH_ADD_BYHASHVALUE(hh,head,fieldname,keylen_in,hashval,add)            \
  HASH_ADD_KEYPTR_BYHASHVALUE(hh, head, &((add)->fieldname), keylen_in, hashval, add)

#define HASH_ADD(hh,head,fieldname,keylen_in,add)                                \
  HASH_ADD_KEYPTR(hh, head, &((add)->fieldname), keylen_in, add)

#define HASH_TO_BKT(hashv,num_bkts,bkt)                                          \
do {                                                                             \
  bkt = ((hashv) & ((num_bkts) - 1U));                                           \
} while (0)

/* delete "delptr" from the hash table.
 * "the usual" patch-up process for the app-order doubly-linked-list.
 * The use of _hd_hh_del below deserves special explanation.
 * These used to be expressed using (delptr) but that led to a bug
 * if someone used the same symbol for the head and deletee, like
 *  HASH_DELETE(hh,users,users);
 * We want that to work, but by changing the head (users) below
 * we were forfeiting our ability to further refer to the deletee (users)
 * in the patch-up process. Solution: use scratch space to
 * copy the deletee pointer, then the latter references are via that
 * scratch pointer rather than through the repointed (users) symbol.
 */
#define HASH_DELETE(hh,head,delptr)                                              \
do {                                                                             \
    struct UT_hash_handle *_hd_hh_del;                                           \
    if ( ((delptr)->hh.prev == NULL) && ((delptr)->hh.next == NULL) )  {         \
        uthash_free((head)->hh.tbl->buckets,                                     \
                    (head)->hh.tbl->num_buckets*sizeof(struct UT_hash_bucket) ); \
        HASH_BLOOM_FREE((head)->hh.tbl);                                         \
        uthash_free((head)->hh.tbl, sizeof(UT_hash_table));                      \
        head = NULL;                                                             \
    } else {                                                                     \
        unsigned _hd_bkt;                                                        \
        _hd_hh_del = &((delptr)->hh);                                            \
        if ((delptr) == ELMT_FROM_HH((head)->hh.tbl,(head)->hh.tbl->tail)) {     \
            (head)->hh.tbl->tail =                                               \
                (UT_hash_handle*)((ptrdiff_t)((delptr)->hh.prev) +               \
                (head)->hh.tbl->hho);                                            \
        }                                                                        \
        if ((delptr)->hh.prev != NULL) {                                         \
            ((UT_hash_handle*)((ptrdiff_t)((delptr)->hh.prev) +                  \
                    (head)->hh.tbl->hho))->next = (delptr)->hh.next;             \
        } else {                                                                 \
            DECLTYPE_ASSIGN(head,(delptr)->hh.next);                             \
        }                                                                        \
        if (_hd_hh_del->next != NULL) {                                          \
            ((UT_hash_handle*)((ptrdiff_t)_hd_hh_del->next +                     \
                    (head)->hh.tbl->hho))->prev =                                \
                    _hd_hh_del->prev;                                            \
        }                                                                        \
        HASH_TO_BKT( _hd_hh_del->hashv, (head)->hh.tbl->num_buckets, _hd_bkt);   \
        HASH_DEL_IN_BKT(hh,(head)->hh.tbl->buckets[_hd_bkt], _hd_hh_del);        \
        (head)->hh.tbl->num_items--;                                             \
    }                                                                            \
    HASH_FSCK(hh,head);                                                          \
} while (0)


/* convenience forms of HASH_FIND/HASH_ADD/HASH_DEL */
#define HASH_FIND_STR(head,findstr,out)                                          \
    HASH_FIND(hh,head,findstr,(unsigned)uthash_strlen(findstr),out)
#define HASH_ADD_STR(head,strfield,add)                                          \
    HASH_ADD(hh,head,strfield[0],(unsigned)uthash_strlen(add->strfield),add)
#define HASH_REPLACE_STR(head,strfield,add,replaced)                             \
    HASH_REPLACE(hh,head,strfield[0],(unsigned)uthash_strlen(add->strfield),add,replaced)
#define HASH_FIND_INT(head,findint,out)                                          \
    HASH_FIND(hh,head,findint,sizeof(int),out)
#define HASH_ADD_INT(head,intfield,add)                                          \
    HASH_ADD(hh,head,intfield,sizeof(int),add)
#define HASH_REPLACE_INT(head,intfield,add,replaced)                             \
    HASH_REPLACE(hh,head,intfield,sizeof(int),add,replaced)
#define HASH_FIND_PTR(head,findptr,out)                                          \
    HASH_FIND(hh,head,findptr,sizeof(void *),out)
#define HASH_ADD_PTR(head,ptrfield,add)                                          \
    HASH_ADD(hh,head,ptrfield,sizeof(void *),add)
#define HASH_REPLACE_PTR(head,ptrfield,add,replaced)                             \
    HASH_REPLACE(hh,head,ptrfield,sizeof(void *),add,replaced)
#define HASH_DEL(head,delptr)                                                    \
    HASH_DELETE(hh,head,delptr)

/* HASH_FSCK checks hash integrity on every add/delete when HASH_DEBUG is defined.
 * This is for uthash developer only; it compiles away if HASH_DEBUG isn't defined.
 */
#ifdef HASH_DEBUG
#define HASH_OOPS(...) do { fprintf(stderr,__VA_ARGS__); exit(-1); } while (0)
#define HASH_FSCK(hh,head)                                                       \
do {                                                                             \
    struct UT_hash_handle *_thh;                                                 \
    if (head) {                                                                  \
        unsigned _bkt_i;                                                         \
        unsigned _count;                                                         \
        char *_prev;                                                             \
        _count = 0;                                                              \
        for( _bkt_i = 0; _bkt_i < (head)->hh.tbl->num_buckets; _bkt_i++) {       \
            unsigned _bkt_count = 0;                                             \
            _thh = (head)->hh.tbl->buckets[_bkt_i].hh_head;                      \
            _prev = NULL;                                                        \
            while (_thh) {                                                       \
               if (_prev != (char*)(_thh->hh_prev)) {                            \
                   HASH_OOPS("invalid hh_prev %p, actual %p\n",                  \
                    _thh->hh_prev, _prev );                                      \
               }                                                                 \
               _bkt_count++;                                                     \
               _prev = (char*)(_thh);                                            \
               _thh = _thh->hh_next;                                             \
            }                                                                    \
            _count += _bkt_count;                                                \
            if ((head)->hh.tbl->buckets[_bkt_i].count !=  _bkt_count) {          \
               HASH_OOPS("invalid bucket count %u, actual %u\n",                 \
                (head)->hh.tbl->buckets[_bkt_i].count, _bkt_count);              \
            }                                                                    \
        }                                                                        \
        if (_count != (head)->hh.tbl->num_items) {                               \
            HASH_OOPS("invalid hh item count %u, actual %u\n",                   \
                (head)->hh.tbl->num_items, _count );                             \
        }                                                                        \
        /* traverse hh in app order; check next/prev integrity, count */         \
        _count = 0;                                                              \
        _prev = NULL;                                                            \
        _thh =  &(head)->hh;                                                     \
        while (_thh) {                                                           \
           _count++;                                                             \
           if (_prev !=(char*)(_thh->prev)) {                                    \
              HASH_OOPS("invalid prev %p, actual %p\n",                          \
                    _thh->prev, _prev );                                         \
           }                                                                     \
           _prev = (char*)ELMT_FROM_HH((head)->hh.tbl, _thh);                    \
           _thh = ( _thh->next ?  (UT_hash_handle*)((char*)(_thh->next) +        \
                                  (head)->hh.tbl->hho) : NULL );                 \
        }                                                                        \
        if (_count != (head)->hh.tbl->num_items) {                               \
            HASH_OOPS("invalid app item count %u, actual %u\n",                  \
                (head)->hh.tbl->num_items, _count );                             \
        }                                                                        \
    }                                                                            \
} while (0)
#else
#define HASH_FSCK(hh,head)
#endif

/* When compiled with -DHASH_EMIT_KEYS, length-prefixed keys are emitted to
 * the descriptor to which this macro is defined for tuning the hash function.
 * The app can #include <unistd.h> to get the prototype for write(2). */
#ifdef HASH_EMIT_KEYS
#define HASH_EMIT_KEY(hh,head,keyptr,fieldlen)                                   \
do {                                                                             \
    unsigned _klen = fieldlen;                                                   \
    write(HASH_EMIT_KEYS, &_klen, sizeof(_klen));                                \
    write(HASH_EMIT_KEYS, keyptr, (unsigned long)fieldlen);                      \
} while (0)
#else
#define HASH_EMIT_KEY(hh,head,keyptr,fieldlen)
#endif

/* default to Jenkin's hash unless overridden e.g. DHASH_FUNCTION=HASH_SAX */
#ifdef HASH_FUNCTION
#define HASH_FCN HASH_FUNCTION
#else
#define HASH_FCN HASH_JEN
#endif

/* The Bernstein hash function, used in Perl prior to v5.6. Note (x<<5+x)=x*33. */
#define HASH_BER(key,keylen,hashv)                                               \
do {                                                                             \
  unsigned _hb_keylen=(unsigned)keylen;                                          \
  const unsigned char *_hb_key=(const unsigned char*)(key);                      \
  (hashv) = 0;                                                                   \
  while (_hb_keylen-- != 0U) {                                                   \
      (hashv) = (((hashv) << 5) + (hashv)) + *_hb_key++;                         \
  }                                                                              \
} while (0)


/* SAX/FNV/OAT/JEN hash functions are macro variants of those listed at
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx */
#define HASH_SAX(key,keylen,hashv)                                               \
do {                                                                             \
  unsigned _sx_i;                                                                \
  const unsigned char *_hs_key=(const unsigned char*)(key);                      \
  hashv = 0;                                                                     \
  for(_sx_i=0; _sx_i < keylen; _sx_i++) {                                        \
      hashv ^= (hashv << 5) + (hashv >> 2) + _hs_key[_sx_i];                     \
  }                                                                              \
} while (0)
/* FNV-1a variation */
#define HASH_FNV(key,keylen,hashv)                                               \
do {                                                                             \
  unsigned _fn_i;                                                                \
  const unsigned char *_hf_key=(const unsigned char*)(key);                      \
  hashv = 2166136261U;                                                           \
  for(_fn_i=0; _fn_i < keylen; _fn_i++) {                                        \
      hashv = hashv ^ _hf_key[_fn_i];                                            \
      hashv = hashv * 16777619U;                                                 \
  }                                                                              \
} while (0)

#define HASH_OAT(key,keylen,hashv)                                               \
do {                                                                             \
  unsigned _ho_i;                                                                \
  const unsigned char *_ho_key=(const unsigned char*)(key);                      \
  hashv = 0;                                                                     \
  for(_ho_i=0; _ho_i < keylen; _ho_i++) {                                        \
      hashv += _ho_key[_ho_i];                                                   \
      hashv += (hashv << 10);                                                    \
      hashv ^= (hashv >> 6);                                                     \
  }                                                                              \
  hashv += (hashv << 3);                                                         \
  hashv ^= (hashv >> 11);                                                        \
  hashv += (hashv << 15);                                                        \
} while (0)

#define HASH_JEN_MIX(a,b,c)                                                      \
do {                                                                             \
  a -= b; a -= c; a ^= ( c >> 13 );                                              \
  b -= c; b -= a; b ^= ( a << 8 );                                               \
  c -= a; c -= b; c ^= ( b >> 13 );                                              \
  a -= b; a -= c; a ^= ( c >> 12 );                                              \
  b -= c; b -= a; b ^= ( a << 16 );                                              \
  c -= a; c -= b; c ^= ( b >> 5 );                                               \
  a -= b; a -= c; a ^= ( c >> 3 );                                               \
  b -= c; b -= a; b ^= ( a << 10 );                                              \
  c -= a; c -= b; c ^= ( b >> 15 );                                              \
} while (0)

#define HASH_JEN(key,keylen,hashv)                                               \
do {                                                                             \
  unsigned _hj_i,_hj_j,_hj_k;                                                    \
  unsigned const char *_hj_key=(unsigned const char*)(key);                      \
  hashv = 0xfeedbeefu;                                                           \
  _hj_i = _hj_j = 0x9e3779b9u;                                                   \
  _hj_k = (unsigned)(keylen);                                                    \
  while (_hj_k >= 12U) {                                                         \
    _hj_i +=    (_hj_key[0] + ( (unsigned)_hj_key[1] << 8 )                      \
        + ( (unsigned)_hj_key[2] << 16 )                                         \
        + ( (unsigned)_hj_key[3] << 24 ) );                                      \
    _hj_j +=    (_hj_key[4] + ( (unsigned)_hj_key[5] << 8 )                      \
        + ( (unsigned)_hj_key[6] << 16 )                                         \
        + ( (unsigned)_hj_key[7] << 24 ) );                                      \
    hashv += (_hj_key[8] + ( (unsigned)_hj_key[9] << 8 )                         \
        + ( (unsigned)_hj_key[10] << 16 )                                        \
        + ( (unsigned)_hj_key[11] << 24 ) );                                     \
                                                                                 \
     HASH_JEN_MIX(_hj_i, _hj_j, hashv);                                          \
                                                                                 \
     _hj_key += 12;                                                              \
     _hj_k -= 12U;                                                               \
  }                                                                              \
  hashv += (unsigned)(keylen);                                                   \
  switch ( _hj_k ) {                                                             \
     case 11: hashv += ( (unsigned)_hj_key[10] << 24 ); /* FALLTHROUGH */        \
     case 10: hashv += ( (unsigned)_hj_key[9] << 16 );  /* FALLTHROUGH */        \
     case 9:  hashv += ( (unsigned)_hj_key[8] << 8 );   /* FALLTHROUGH */        \
     case 8:  _hj_j += ( (unsigned)_hj_key[7] << 24 );  /* FALLTHROUGH */        \
     case 7:  _hj_j += ( (unsigned)_hj_key[6] << 16 );  /* FALLTHROUGH */        \
     case 6:  _hj_j += ( (unsigned)_hj_key[5] << 8 );   /* FALLTHROUGH */        \
     case 5:  _hj_j += _hj_key[4];                      /* FALLTHROUGH */        \
     case 4:  _hj_i += ( (unsigned)_hj_key[3] << 24 );  /* FALLTHROUGH */        \
     case 3:  _hj_i += ( (unsigned)_hj_key[2] << 16 );  /* FALLTHROUGH */        \
     case 2:  _hj_i += ( (unsigned)_hj_key[1] << 8 );   /* FALLTHROUGH */        \
     case 1:  _hj_i += _hj_key[0];                                               \
  }                                                                              \
  HASH_JEN_MIX(_hj_i, _hj_j, hashv);                                             \
} while (0)

/* The Paul Hsieh hash function */
#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__)             \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)             \
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif
#define HASH_SFH(key,keylen,hashv)                                               \
do {                                                                             \
  unsigned const char *_sfh_key=(unsigned const char*)(key);                     \
  uint32_t _sfh_tmp, _sfh_len = (uint32_t)keylen;                                \
                                                                                 \
  unsigned _sfh_rem = _sfh_len & 3U;                                             \
  _sfh_len >>= 2;                                                                \
  hashv = 0xcafebabeu;                                                           \
                                                                                 \
  /* Main loop */                                                                \
  for (;_sfh_len > 0U; _sfh_len--) {                                             \
    hashv    += get16bits (_sfh_key);                                            \
    _sfh_tmp  = ((uint32_t)(get16bits (_sfh_key+2)) << 11) ^ hashv;              \
    hashv     = (hashv << 16) ^ _sfh_tmp;                                        \
    _sfh_key += 2U*sizeof (uint16_t);                                            \
    hashv    += hashv >> 11;                                                     \
  }                                                                              \
                                                                                 \
  /* Handle end cases */                                                         \
  switch (_sfh_rem) {                                                            \
    case 3: hashv += get16bits (_sfh_key);                                       \
            hashv ^= hashv << 16;                                                \
            hashv ^= (uint32_t)(_sfh_key[sizeof (uint16_t)]) << 18;              \
            hashv += hashv >> 11;                                                \
            break;                                                               \
    case 2: hashv += get16bits (_sfh_key);                                       \
            hashv ^= hashv << 11;                                                \
            hashv += hashv >> 17;                                                \
            break;                                                               \
    case 1: hashv += *_sfh_key;                                                  \
            hashv ^= hashv << 10;                                                \
            hashv += hashv >> 1;                                                 \
  }                                                                              \
                                                                                 \
    /* Force "avalanching" of final 127 bits */                                  \
    hashv ^= hashv << 3;                                                         \
    hashv += hashv >> 5;                                                         \
    hashv ^= hashv << 4;                                                         \
    hashv += hashv >> 17;                                                        \
    hashv ^= hashv << 25;                                                        \
    hashv += hashv >> 6;                                                         \
} while (0)

#ifdef HASH_USING_NO_STRICT_ALIASING
/* The MurmurHash exploits some CPU's (x86,x86_64) tolerance for unaligned reads.
 * For other types of CPU's (e.g. Sparc) an unaligned read causes a bus error.
 * MurmurHash uses the faster approach only on CPU's where we know it's safe.
 *
 * Note the preprocessor built-in defines can be emitted using:
 *
 *   gcc -m64 -dM -E - < /dev/null                  (on gcc)
 *   cc -## a.c (where a.c is a simple test file)   (Sun Studio)
 */
#if (defined(__i386__) || defined(__x86_64__)  || defined(_M_IX86))
#define MUR_GETBLOCK(p,i) p[i]
#else /* non intel */
#define MUR_PLUS0_ALIGNED(p) (((unsigned long)p & 3UL) == 0UL)
#define MUR_PLUS1_ALIGNED(p) (((unsigned long)p & 3UL) == 1UL)
#define MUR_PLUS2_ALIGNED(p) (((unsigned long)p & 3UL) == 2UL)
#define MUR_PLUS3_ALIGNED(p) (((unsigned long)p & 3UL) == 3UL)
#define WP(p) ((uint32_t*)((unsigned long)(p) & ~3UL))
#if (defined(__BIG_ENDIAN__) || defined(SPARC) || defined(__ppc__) || defined(__ppc64__))
#define MUR_THREE_ONE(p) ((((*WP(p))&0x00ffffff) << 8) | (((*(WP(p)+1))&0xff000000) >> 24))
#define MUR_TWO_TWO(p)   ((((*WP(p))&0x0000ffff) <<16) | (((*(WP(p)+1))&0xffff0000) >> 16))
#define MUR_ONE_THREE(p) ((((*WP(p))&0x000000ff) <<24) | (((*(WP(p)+1))&0xffffff00) >>  8))
#else /* assume little endian non-intel */
#define MUR_THREE_ONE(p) ((((*WP(p))&0xffffff00) >> 8) | (((*(WP(p)+1))&0x000000ff) << 24))
#define MUR_TWO_TWO(p)   ((((*WP(p))&0xffff0000) >>16) | (((*(WP(p)+1))&0x0000ffff) << 16))
#define MUR_ONE_THREE(p) ((((*WP(p))&0xff000000) >>24) | (((*(WP(p)+1))&0x00ffffff) <<  8))
#endif
#define MUR_GETBLOCK(p,i) (MUR_PLUS0_ALIGNED(p) ? ((p)[i]) :           \
                            (MUR_PLUS1_ALIGNED(p) ? MUR_THREE_ONE(p) : \
                             (MUR_PLUS2_ALIGNED(p) ? MUR_TWO_TWO(p) :  \
                                                      MUR_ONE_THREE(p))))
#endif
#define MUR_ROTL32(x,r) (((x) << (r)) | ((x) >> (32 - (r))))
#define MUR_FMIX(_h) \
do {                 \
  _h ^= _h >> 16;    \
  _h *= 0x85ebca6bu; \
  _h ^= _h >> 13;    \
  _h *= 0xc2b2ae35u; \
  _h ^= _h >> 16;    \
} while (0)

#define HASH_MUR(key,keylen,hashv)                                     \
do {                                                                   \
  const uint8_t *_mur_data = (const uint8_t*)(key);                    \
  const int _mur_nblocks = (int)(keylen) / 4;                          \
  uint32_t _mur_h1 = 0xf88D5353u;                                      \
  uint32_t _mur_c1 = 0xcc9e2d51u;                                      \
  uint32_t _mur_c2 = 0x1b873593u;                                      \
  uint32_t _mur_k1 = 0;                                                \
  const uint8_t *_mur_tail;                                            \
  const uint32_t *_mur_blocks = (const uint32_t*)(_mur_data+(_mur_nblocks*4)); \
  int _mur_i;                                                          \
  for(_mur_i = -_mur_nblocks; _mur_i!=0; _mur_i++) {                   \
    _mur_k1 = MUR_GETBLOCK(_mur_blocks,_mur_i);                        \
    _mur_k1 *= _mur_c1;                                                \
    _mur_k1 = MUR_ROTL32(_mur_k1,15);                                  \
    _mur_k1 *= _mur_c2;                                                \
                                                                       \
    _mur_h1 ^= _mur_k1;                                                \
    _mur_h1 = MUR_ROTL32(_mur_h1,13);                                  \
    _mur_h1 = (_mur_h1*5U) + 0xe6546b64u;                              \
  }                                                                    \
  _mur_tail = (const uint8_t*)(_mur_data + (_mur_nblocks*4));          \
  _mur_k1=0;                                                           \
  switch((keylen) & 3U) {                                              \
    case 3: _mur_k1 ^= (uint32_t)_mur_tail[2] << 16; /* FALLTHROUGH */ \
    case 2: _mur_k1 ^= (uint32_t)_mur_tail[1] << 8;  /* FALLTHROUGH */ \
    case 1: _mur_k1 ^= (uint32_t)_mur_tail[0];                         \
    _mur_k1 *= _mur_c1;                                                \
    _mur_k1 = MUR_ROTL32(_mur_k1,15);                                  \
    _mur_k1 *= _mur_c2;                                                \
    _mur_h1 ^= _mur_k1;                                                \
  }                                                                    \
  _mur_h1 ^= (uint32_t)(keylen);                                       \
  MUR_FMIX(_mur_h1);                                                   \
  hashv = _mur_h1;                                                     \
} while (0)
#endif  /* HASH_USING_NO_STRICT_ALIASING */

/* iterate over items in a known bucket to find desired item */
#define HASH_FIND_IN_BKT(tbl,hh,head,keyptr,keylen_in,hashval,out)               \
do {                                                                             \
  if ((head).hh_head != NULL) {                                                  \
    DECLTYPE_ASSIGN(out, ELMT_FROM_HH(tbl, (head).hh_head));                     \
  } else {                                                                       \
    (out) = NULL;                                                                \
  }                                                                              \
  while ((out) != NULL) {                                                        \
    if ((out)->hh.hashv == (hashval) && (out)->hh.keylen == (keylen_in)) {       \
      if (uthash_memcmp((out)->hh.key, keyptr, keylen_in) == 0) {                \
        break;                                                                   \
      }                                                                          \
    }                                                                            \
    if ((out)->hh.hh_next != NULL) {                                             \
      DECLTYPE_ASSIGN(out, ELMT_FROM_HH(tbl, (out)->hh.hh_next));                \
    } else {                                                                     \
      (out) = NULL;                                                              \
    }                                                                            \
  }                                                                              \
} while (0)

/* add an item to a bucket  */
#define HASH_ADD_TO_BKT(head,addhh)                                              \
do {                                                                             \
 head.count++;                                                                   \
 (addhh)->hh_next = head.hh_head;                                                \
 (addhh)->hh_prev = NULL;                                                        \
 if (head.hh_head != NULL) { (head).hh_head->hh_prev = (addhh); }                \
 (head).hh_head=addhh;                                                           \
 if ((head.count >= ((head.expand_mult+1U) * HASH_BKT_CAPACITY_THRESH))          \
     && ((addhh)->tbl->noexpand != 1U)) {                                        \
       HASH_EXPAND_BUCKETS((addhh)->tbl);                                        \
 }                                                                               \
} while (0)

/* remove an item from a given bucket */
#define HASH_DEL_IN_BKT(hh,head,hh_del)                                          \
    (head).count--;                                                              \
    if ((head).hh_head == hh_del) {                                              \
      (head).hh_head = hh_del->hh_next;                                          \
    }                                                                            \
    if (hh_del->hh_prev) {                                                       \
        hh_del->hh_prev->hh_next = hh_del->hh_next;                              \
    }                                                                            \
    if (hh_del->hh_next) {                                                       \
        hh_del->hh_next->hh_prev = hh_del->hh_prev;                              \
    }

/* Bucket expansion has the effect of doubling the number of buckets
 * and redistributing the items into the new buckets. Ideally the
 * items will distribute more or less evenly into the new buckets
 * (the extent to which this is true is a measure of the quality of
 * the hash function as it applies to the key domain).
 *
 * With the items distributed into more buckets, the chain length
 * (item count) in each bucket is reduced. Thus by expanding buckets
 * the hash keeps a bound on the chain length. This bounded chain
 * length is the essence of how a hash provides constant time lookup.
 *
 * The calculation of tbl->ideal_chain_maxlen below deserves some
 * explanation. First, keep in mind that we're calculating the ideal
 * maximum chain length based on the *new* (doubled) bucket count.
 * In fractions this is just n/b (n=number of items,b=new num buckets).
 * Since the ideal chain length is an integer, we want to calculate
 * ceil(n/b). We don't depend on floating point arithmetic in this
 * hash, so to calculate ceil(n/b) with integers we could write
 *
 *      ceil(n/b) = (n/b) + ((n%b)?1:0)
 *
 * and in fact a previous version of this hash did just that.
 * But now we have improved things a bit by recognizing that b is
 * always a power of two. We keep its base 2 log handy (call it lb),
 * so now we can write this with a bit shift and logical AND:
 *
 *      ceil(n/b) = (n>>lb) + ( (n & (b-1)) ? 1:0)
 *
 */
#define HASH_EXPAND_BUCKETS(tbl)                                                 \
do {                                                                             \
    unsigned _he_bkt;                                                            \
    unsigned _he_bkt_i;                                                          \
    struct UT_hash_handle *_he_thh, *_he_hh_nxt;                                 \
    UT_hash_bucket *_he_new_buckets, *_he_newbkt;                                \
    _he_new_buckets = (UT_hash_bucket*)uthash_malloc(                            \
             2UL * tbl->num_buckets * sizeof(struct UT_hash_bucket));            \
    if (!_he_new_buckets) { uthash_fatal( "out of memory"); }                    \
    memset(_he_new_buckets, 0,                                                   \
            2UL * tbl->num_buckets * sizeof(struct UT_hash_bucket));             \
    tbl->ideal_chain_maxlen =                                                    \
       (tbl->num_items >> (tbl->log2_num_buckets+1U)) +                          \
       (((tbl->num_items & ((tbl->num_buckets*2U)-1U)) != 0U) ? 1U : 0U);        \
    tbl->nonideal_items = 0;                                                     \
    for(_he_bkt_i = 0; _he_bkt_i < tbl->num_buckets; _he_bkt_i++)                \
    {                                                                            \
        _he_thh = tbl->buckets[ _he_bkt_i ].hh_head;                             \
        while (_he_thh != NULL) {                                                \
           _he_hh_nxt = _he_thh->hh_next;                                        \
           HASH_TO_BKT( _he_thh->hashv, tbl->num_buckets*2U, _he_bkt);           \
           _he_newbkt = &(_he_new_buckets[ _he_bkt ]);                           \
           if (++(_he_newbkt->count) > tbl->ideal_chain_maxlen) {                \
             tbl->nonideal_items++;                                              \
             _he_newbkt->expand_mult = _he_newbkt->count /                       \
                                        tbl->ideal_chain_maxlen;                 \
           }                                                                     \
           _he_thh->hh_prev = NULL;                                              \
           _he_thh->hh_next = _he_newbkt->hh_head;                               \
           if (_he_newbkt->hh_head != NULL) { _he_newbkt->hh_head->hh_prev =     \
                _he_thh; }                                                       \
           _he_newbkt->hh_head = _he_thh;                                        \
           _he_thh = _he_hh_nxt;                                                 \
        }                                                                        \
    }                                                                            \
    uthash_free( tbl->buckets, tbl->num_buckets*sizeof(struct UT_hash_bucket) ); \
    tbl->num_buckets *= 2U;                                                      \
    tbl->log2_num_buckets++;                                                     \
    tbl->buckets = _he_new_buckets;                                              \
    tbl->ineff_expands = (tbl->nonideal_items > (tbl->num_items >> 1)) ?         \
        (tbl->ineff_expands+1U) : 0U;                                            \
    if (tbl->ineff_expands > 1U) {                                               \
        tbl->noexpand=1;                                                         \
        uthash_noexpand_fyi(tbl);                                                \
    }                                                                            \
    uthash_expand_fyi(tbl);                                                      \
} while (0)


/* This is an adaptation of Simon Tatham's O(n log(n)) mergesort */
/* Note that HASH_SORT assumes the hash handle name to be hh.
 * HASH_SRT was added to allow the hash handle name to be passed in. */
#define HASH_SORT(head,cmpfcn) HASH_SRT(hh,head,cmpfcn)
#define HASH_SRT(hh,head,cmpfcn)                                                 \
do {                                                                             \
  unsigned _hs_i;                                                                \
  unsigned _hs_looping,_hs_nmerges,_hs_insize,_hs_psize,_hs_qsize;               \
  struct UT_hash_handle *_hs_p, *_hs_q, *_hs_e, *_hs_list, *_hs_tail;            \
  if (head != NULL) {                                                            \
      _hs_insize = 1;                                                            \
      _hs_looping = 1;                                                           \
      _hs_list = &((head)->hh);                                                  \
      while (_hs_looping != 0U) {                                                \
          _hs_p = _hs_list;                                                      \
          _hs_list = NULL;                                                       \
          _hs_tail = NULL;                                                       \
          _hs_nmerges = 0;                                                       \
          while (_hs_p != NULL) {                                                \
              _hs_nmerges++;                                                     \
              _hs_q = _hs_p;                                                     \
              _hs_psize = 0;                                                     \
              for ( _hs_i = 0; _hs_i  < _hs_insize; _hs_i++ ) {                  \
                  _hs_psize++;                                                   \
                  _hs_q = (UT_hash_handle*)((_hs_q->next != NULL) ?              \
                          ((void*)((char*)(_hs_q->next) +                        \
                          (head)->hh.tbl->hho)) : NULL);                         \
                  if (! (_hs_q) ) { break; }                                     \
              }                                                                  \
              _hs_qsize = _hs_insize;                                            \
              while ((_hs_psize > 0U) || ((_hs_qsize > 0U) && (_hs_q != NULL))) {\
                  if (_hs_psize == 0U) {                                         \
                      _hs_e = _hs_q;                                             \
                      _hs_q = (UT_hash_handle*)((_hs_q->next != NULL) ?          \
                              ((void*)((char*)(_hs_q->next) +                    \
                              (head)->hh.tbl->hho)) : NULL);                     \
                      _hs_qsize--;                                               \
                  } else if ( (_hs_qsize == 0U) || (_hs_q == NULL) ) {           \
                      _hs_e = _hs_p;                                             \
                      if (_hs_p != NULL){                                        \
                        _hs_p = (UT_hash_handle*)((_hs_p->next != NULL) ?        \
                                ((void*)((char*)(_hs_p->next) +                  \
                                (head)->hh.tbl->hho)) : NULL);                   \
                       }                                                         \
                      _hs_psize--;                                               \
                  } else if ((                                                   \
                      cmpfcn(DECLTYPE(head)(ELMT_FROM_HH((head)->hh.tbl,_hs_p)), \
                             DECLTYPE(head)(ELMT_FROM_HH((head)->hh.tbl,_hs_q))) \
                             ) <= 0) {                                           \
                      _hs_e = _hs_p;                                             \
                      if (_hs_p != NULL){                                        \
                        _hs_p = (UT_hash_handle*)((_hs_p->next != NULL) ?        \
                               ((void*)((char*)(_hs_p->next) +                   \
                               (head)->hh.tbl->hho)) : NULL);                    \
                       }                                                         \
                      _hs_psize--;                                               \
                  } else {                                                       \
                      _hs_e = _hs_q;                                             \
                      _hs_q = (UT_hash_handle*)((_hs_q->next != NULL) ?          \
                              ((void*)((char*)(_hs_q->next) +                    \
                              (head)->hh.tbl->hho)) : NULL);                     \
                      _hs_qsize--;                                               \
                  }                                                              \
                  if ( _hs_tail != NULL ) {                                      \
                      _hs_tail->next = ((_hs_e != NULL) ?                        \
                            ELMT_FROM_HH((head)->hh.tbl,_hs_e) : NULL);          \
                  } else {                                                       \
                      _hs_list = _hs_e;                                          \
                  }                                                              \
                  if (_hs_e != NULL) {                                           \
                  _hs_e->prev = ((_hs_tail != NULL) ?                            \
                     ELMT_FROM_HH((head)->hh.tbl,_hs_tail) : NULL);              \
                  }                                                              \
                  _hs_tail = _hs_e;                                              \
              }                                                                  \
              _hs_p = _hs_q;                                                     \
          }                                                                      \
          if (_hs_tail != NULL){                                                 \
            _hs_tail->next = NULL;                                               \
          }                                                                      \
          if ( _hs_nmerges <= 1U ) {                                             \
              _hs_looping=0;                                                     \
              (head)->hh.tbl->tail = _hs_tail;                                   \
              DECLTYPE_ASSIGN(head,ELMT_FROM_HH((head)->hh.tbl, _hs_list));      \
          }                                                                      \
          _hs_insize *= 2U;                                                      \
      }                                                                          \
      HASH_FSCK(hh,head);                                                        \
 }                                                                               \
} while (0)

/* This function selects items from one hash into another hash.
 * The end result is that the selected items have dual presence
 * in both hashes. There is no copy of the items made; rather
 * they are added into the new hash through a secondary hash
 * hash handle that must be present in the structure. */
#define HASH_SELECT(hh_dst, dst, hh_src, src, cond)                              \
do {                                                                             \
  unsigned _src_bkt, _dst_bkt;                                                   \
  void *_last_elt=NULL, *_elt;                                                   \
  UT_hash_handle *_src_hh, *_dst_hh, *_last_elt_hh=NULL;                         \
  ptrdiff_t _dst_hho = ((char*)(&(dst)->hh_dst) - (char*)(dst));                 \
  if (src != NULL) {                                                             \
    for(_src_bkt=0; _src_bkt < (src)->hh_src.tbl->num_buckets; _src_bkt++) {     \
      for(_src_hh = (src)->hh_src.tbl->buckets[_src_bkt].hh_head;                \
          _src_hh != NULL;                                                       \
          _src_hh = _src_hh->hh_next) {                                          \
          _elt = ELMT_FROM_HH((src)->hh_src.tbl, _src_hh);                       \
          if (cond(_elt)) {                                                      \
            _dst_hh = (UT_hash_handle*)(((char*)_elt) + _dst_hho);               \
            _dst_hh->key = _src_hh->key;                                         \
            _dst_hh->keylen = _src_hh->keylen;                                   \
            _dst_hh->hashv = _src_hh->hashv;                                     \
            _dst_hh->prev = _last_elt;                                           \
            _dst_hh->next = NULL;                                                \
            if (_last_elt_hh != NULL) { _last_elt_hh->next = _elt; }             \
            if (dst == NULL) {                                                   \
              DECLTYPE_ASSIGN(dst,_elt);                                         \
              HASH_MAKE_TABLE(hh_dst,dst);                                       \
            } else {                                                             \
              _dst_hh->tbl = (dst)->hh_dst.tbl;                                  \
            }                                                                    \
            HASH_TO_BKT(_dst_hh->hashv, _dst_hh->tbl->num_buckets, _dst_bkt);    \
            HASH_ADD_TO_BKT(_dst_hh->tbl->buckets[_dst_bkt],_dst_hh);            \
            (dst)->hh_dst.tbl->num_items++;                                      \
            _last_elt = _elt;                                                    \
            _last_elt_hh = _dst_hh;                                              \
          }                                                                      \
      }                                                                          \
    }                                                                            \
  }                                                                              \
  HASH_FSCK(hh_dst,dst);                                                         \
} while (0)

#define HASH_CLEAR(hh,head)                                                      \
do {                                                                             \
  if (head != NULL) {                                                            \
    uthash_free((head)->hh.tbl->buckets,                                         \
                (head)->hh.tbl->num_buckets*sizeof(struct UT_hash_bucket));      \
    HASH_BLOOM_FREE((head)->hh.tbl);                                             \
    uthash_free((head)->hh.tbl, sizeof(UT_hash_table));                          \
    (head)=NULL;                                                                 \
  }                                                                              \
} while (0)

#define HASH_OVERHEAD(hh,head)                                                   \
 ((head != NULL) ? (                                                             \
 (size_t)(((head)->hh.tbl->num_items   * sizeof(UT_hash_handle))   +             \
          ((head)->hh.tbl->num_buckets * sizeof(UT_hash_bucket))   +             \
           sizeof(UT_hash_table)                                   +             \
           (HASH_BLOOM_BYTELEN))) : 0U)

#ifdef NO_DECLTYPE
#define HASH_ITER(hh,head,el,tmp)                                                \
for(((el)=(head)), ((*(char**)(&(tmp)))=(char*)((head!=NULL)?(head)->hh.next:NULL)); \
  (el) != NULL; ((el)=(tmp)), ((*(char**)(&(tmp)))=(char*)((tmp!=NULL)?(tmp)->hh.next:NULL)))
#else
#define HASH_ITER(hh,head,el,tmp)                                                \
for(((el)=(head)), ((tmp)=DECLTYPE(el)((head!=NULL)?(head)->hh.next:NULL));      \
  (el) != NULL; ((el)=(tmp)), ((tmp)=DECLTYPE(el)((tmp!=NULL)?(tmp)->hh.next:NULL)))
#endif

/* obtain a count of items in the hash */
#define HASH_COUNT(head) HASH_CNT(hh,head)
#define HASH_CNT(hh,head) ((head != NULL)?((head)->hh.tbl->num_items):0U)

typedef struct UT_hash_bucket {
   struct UT_hash_handle *hh_head;
   unsigned count;

   /* expand_mult is normally set to 0. In this situation, the max chain length
    * threshold is enforced at its default value, HASH_BKT_CAPACITY_THRESH. (If
    * the bucket's chain exceeds this length, bucket expansion is triggered).
    * However, setting expand_mult to a non-zero value delays bucket expansion
    * (that would be triggered by additions to this particular bucket)
    * until its chain length reaches a *multiple* of HASH_BKT_CAPACITY_THRESH.
    * (The multiplier is simply expand_mult+1). The whole idea of this
    * multiplier is to reduce bucket expansions, since they are expensive, in
    * situations where we know that a particular bucket tends to be overused.
    * It is better to let its chain length grow to a longer yet-still-bounded
    * value, than to do an O(n) bucket expansion too often.
    */
   unsigned expand_mult;

} UT_hash_bucket;

/* random signature used only to find hash tables in external analysis */
#define HASH_SIGNATURE 0xa0111fe1u
#define HASH_BLOOM_SIGNATURE 0xb12220f2u

typedef struct UT_hash_table {
   UT_hash_bucket *buckets;
   unsigned num_buckets, log2_num_buckets;
   unsigned num_items;
   struct UT_hash_handle *tail; /* tail hh in app order, for fast append    */
   ptrdiff_t hho; /* hash handle offset (byte pos of hash handle in element */

   /* in an ideal situation (all buckets used equally), no bucket would have
    * more than ceil(#items/#buckets) items. that's the ideal chain length. */
   unsigned ideal_chain_maxlen;

   /* nonideal_items is the number of items in the hash whose chain position
    * exceeds the ideal chain maxlen. these items pay the penalty for an uneven
    * hash distribution; reaching them in a chain traversal takes >ideal steps */
   unsigned nonideal_items;

   /* ineffective expands occur when a bucket doubling was performed, but
    * afterward, more than half the items in the hash had nonideal chain
    * positions. If this happens on two consecutive expansions we inhibit any
    * further expansion, as it's not helping; this happens when the hash
    * function isn't a good fit for the key domain. When expansion is inhibited
    * the hash will still work, albeit no longer in constant time. */
   unsigned ineff_expands, noexpand;

   uint32_t signature; /* used only to find hash tables in external analysis */
#ifdef HASH_BLOOM
   uint32_t bloom_sig; /* used only to test bloom exists in external analysis */
   uint8_t *bloom_bv;
   uint8_t bloom_nbits;
#endif

} UT_hash_table;

typedef struct UT_hash_handle {
   struct UT_hash_table *tbl;
   void *prev;                       /* prev element in app order      */
   void *next;                       /* next element in app order      */
   struct UT_hash_handle *hh_prev;   /* previous hh in bucket order    */
   struct UT_hash_handle *hh_next;   /* next hh in bucket order        */
   void *key;                        /* ptr to enclosing struct's key  */
   unsigned keylen;                  /* enclosing struct's key len     */
   unsigned hashv;                   /* result of hash-fcn(key)        */
} UT_hash_handle;

#endif /* UTHASH_H */
/**
 * @file
 *
 * Implements hashtables
 *
 * This module is just a front-end to uthash
 *
 * Here's an example on how to use the hashtable. Of course you can exploit all the macros available in \b ut_hash to find/loop and so on
 * even here. Those macros are explained <a href="https://troydhanson.github.io/uthash/userguide.html#_hash_operations">here</a>
 *
 * ```
 * ct_hashtable_o* ht = ct_ht_init();
 * double* d = NULL;
 *
 * d = malloc(sizeof(double)); *d = 5;
 * ct_ht_put(&ht,1, d);
 * d = malloc(sizeof(double)); *d = 127;
 * ct_ht_put(&ht,5, d);
 * d = malloc(sizeof(double)); *d = -4.5;
 * ct_ht_put(&ht,7, d);
 *
 * ct_ht_destroy_with_elements(&ht, free);
 * ```
 *
 * @author koldar
 * @date Jan 24, 2017
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdbool.h>
#include <stdlib.h> /* For NULL macro */





/**
 * A structure representing an entry of the hash table
 *
 * see <a href="http://troydhanson.github.io/uthash/userguide.html">uthash</a> for further information
 */
struct ct_hashtable_entry;
typedef struct ct_hashtable_entry ct_hashtable_entry_o;

/**
 * Structure representing the facet of the hashtable data structure
 */
struct ct_hashtable;
typedef struct ct_hashtable ct_hashtable_o;

/**
 * Create a new hashtable in memory
 *
 * @return the new hashtable just created
 */
ct_hashtable_o* ct_ht_init();

/**
 * number of item in the hashtable
 *
 * \note
 * This operation is O(1)
 *
 * @param[in] ht the hash table involved
 * @return the number of elements inside the hash table
 */
int ct_ht_size(const ct_hashtable_o* ht);

/**
 * get an element in the hashtable, given a certain key
 *
 * @param[in] ht the hashtable whwere we want to look int
 * @param[in] key the key associated to a certain element
 * @return
 * 	\li the element whose key is \c key;
 * 	\li NULL if it isn't present in the hashtable;
 */
void* ct_ht_get(const ct_hashtable_o* ht, unsigned long key);

/**
 * Check if a value in the hashtable has a particular key
 *
 * @param[in] ht the ht involved
 * @param[in] key the key involved
 * @return
 * 	\li @true if there is a value within the hashtable whose key is \c key;
 * 	\li @false otheriwse
 */
bool ct_ht_contains(const ct_hashtable_o* ht, unsigned long key);

/**
 * Insert or alter an element inside the hash table
 *
 * If the hashtable doesn't have an element indexed with \c key, a new element will be created.
 * Otherwise we will update the existing element
 *
 * \attention
 * After this operation the old data will be totally overwritten! Be sure to still have a backup reference
 * of the previous object, otherwise memory leaks will likely happen!
 *
 * @param[inout] ht the hashtable to analyze
 * @param[in] key the key of the element to update
 * @param[in] data the new data to overwrite the old one
 * @return
 * 	\li @true if a new element is created;
 * 	\li @false if we overwrote the previous one
 */
bool ct_ht_put_or_update(ct_hashtable_o* ht, unsigned long key, const void* data);

/**
 * Updates the value indexed by \c key to a new value
 *
 * \note
 * If there is no element indexed with \c key in the hashtable, this function does nothing
 *
 * @param[inout] ht the hashtable to alter
 * @param[in] key the key referring to the element to update
 * @param[in] data the new data that will replace the old one
 * @return
 * 	\li @true if we have update with success the data;
 * 	\li @false if we couldn't find any cell indexed with \c key;
 */
bool ct_ht_update(ct_hashtable_o* ht, unsigned long key, const void* data);

/**
 * Insert a new value within the hashtable
 *
 * \note
 * Undefined behaviour if the key already exists in the hashtable
 *
 *
 * @param[inout] ht the hashtable to handle
 * @param[in] key the key of \c data
 * @param[in] data the actual value to store in the hastable
 */
void ct_ht_put(ct_hashtable_o* ht, unsigned long key, const void* data);

/**
 * Release the hashtable from the memory
 *
 * \note
 * If the values in the hastable are in memory as well, they won't be freed at all
 *
 * @param[inout] ht the hashtable to remove
 * @see ct_ht_destroy_with_elements
 */
void ct_ht_destroy(ct_hashtable_o* ht);

/**
 * Like ::ct_ht_destroy but it frees the items in the hashtable from memory as well
 *
 * @param[inout] ht the hashtable to remove
 * @param[in] d a function to use to free the elements in the hashtable from memory
 */
void ct_ht_destroy_with_elements(ct_hashtable_o* ht, ct_destroyer_c d);

/**
 * Remove an element from the hashtable
 *
 * \note
 * The element won't be removed from the memory at all
 *
 * @param[inout] ht the hashtable to handle
 * @param[in] key the key of the value to remove from the hashtable
 * @see ct_ht_remove_and_destroy
 */
bool ct_ht_remove(ct_hashtable_o* ht, unsigned long key);

/**
 * like ::ct_ht_remove but it frees the value from memory as well
 *
 * @param[inout] ht the hashtable to handle
 * @param[in] key the key of the value to remove from the hashtable
 * @param[in] d the function to use to dispose of the value removed from the hashtable
 */
bool ct_ht_remove_and_destroy(ct_hashtable_o* ht, unsigned long key, ct_destroyer_c d);

/**
 * checks if the hashtable contains no values
 *
 * @param[in] ht the hashtable to handle
 * @return @true if the hashtable contains no values; @false otherwise;
 */
bool ct_ht_is_empty(const ct_hashtable_o* ht);

/**
 * Fetch the first item the program can find within the hashtable.
 *
 * Nothing is said about what the software picks up: **don't assume it was the first one you have added in the hashtable!**
 *
 * @param[in] ht the hashtable involved
 * @return
 * 	\li an item inside the \c ht;
 * 	\li @null if the \c ht is empty;
 */
void* ct_ht_get_first(const ct_hashtable_o* ht);

/**
 * Swap the contents of the values indexed by \c key1 and \c keys
 *
 * If one key does not exist in the hashtable, after this operation it will exist whilst the
 * former one will be removed
 *
 * @param[inout] ht the ht to handle
 * @param[in] key1 the first key
 * @param[in] key2 the second key
 * @return
 * 	\li @true if a swap is performed;
 * 	\li @false if both keys are not present;
 */
bool ct_ht_swap(ct_hashtable_o* ht, unsigned long key1, unsigned long key2);

/**
 * Clear all the elements inside the given hashtable.
 *
 * The values inside the hashtable won't be touched; however make sure
 * you have other reference of them otherwise memory leak might happen
 *
 * @param[inout] ht the hashtable to clear
 */
void ct_ht_clear(ct_hashtable_o* ht);

/**
 * Clear all the elements inside the given hashtable
 *
 * Contrary to ::ct_ht_clear, this function will release the memory of the payloads inside the hashtable as well!
 *
 * \post
 * 	\li hashtable empty;
 * 	\li elements in hashtable released from memory
 *
 * @param[inout] ht the hashtable to clear
 * @param[in] d the function to use to destroy the paylaod of the hashtable
 */
void ct_ht_clear_and_destroy_elements(ct_hashtable_o* ht, ct_destroyer_c d);

/**
 * Returns the head entry of the hashtable.
 *
 * @param[inout] ht The hashtable on which to operate
 * @return The head entry of the hashtable
 */
ct_hashtable_entry_o* _ct_ht_head_entry(ct_hashtable_o* ht);

/**
 * Returns the value of the "next" field in the UT_hash_handle field of an entry.
 * \note
 * This function is purely instrumental towards hiding implementation details to the hashtable user
 *
 * @param[in] entry The hashtable entry on which the desired handle resides
 * @return A pointer to the next value contained in the hashtable, considered by insertion order
 */
void* _ct_ht_entry_next(ct_hashtable_entry_o* entry);

/**
 * Returns the payload of a given hashtable entry
 *
 * @param[in] entry The entry which contains the desired payload
 * @return The payload of the entry
 */
void* _ct_ht_entry_payload(ct_hashtable_entry_o* entry);

//Picked up from uthash HASH_ITER Definition
#ifdef CT_ITERATE_ON_HT
#	error "CrashC - CT_ITERATE_ON_HT macro already defined!"
#endif
#define CT_ITERATE_ON_HT(ht,el)   																								\
	ct_hashtable_entry_o* CT_UV(head) = _ct_ht_head_entry((ct_hashtable_o*) (ht));												\
	ct_hashtable_entry_o* el = NULL; 																						\
	ct_hashtable_entry_o* CT_UV(tmp) = NULL;																					\
	for(((el)=(CT_UV(head))), ((*(char**)(&(CT_UV(tmp))))=(char*)((CT_UV(head)!=NULL)?_ct_ht_entry_next(CT_UV(head)):NULL)); 			\
  (el) != NULL; ((el)=(CT_UV(tmp))), ((*(char**)(&(CT_UV(tmp))))=(char*)((CT_UV(tmp)!=NULL)?_ct_ht_entry_next(CT_UV(tmp)):NULL)))

//TODO replace this code with the correct code in Cutils!!!!
/**
 * Macro allowing you to go through every element of hashtable
 *
 * An example of usage might be:
 *
 * <pre><code>
 * ct_hashtable_o* ht;
 * CT_ITERATE_VALUES_ON_HT(&ht, el, int) {
 * 	printf("value %d\n", *el);
 * }
 * </code></pre>
 *
 * @param[in] ht Pointer to the hashtable on which to iterate
 * @param[in] _data the name of the variable that will contain a value in the iteration
 * @param[in] type the type of \c _data. So if you put \c int, data will have type <tt>int</tt>
 */
#ifdef CT_ITERATE_VALUES_ON_HT
#	error "CrashC - CT_ITERATE_VALUES_ON_HT macro already defined!"
#endif
#define CT_ITERATE_VALUES_ON_HT(ht,_data,type) 															\
	ct_hashtable_entry_o* CT_UV(head) = _ct_ht_head_entry((ct_hashtable_o*)(ht));						\
	ct_hashtable_entry_o* CT_UV(el) = NULL; 															\
	ct_hashtable_entry_o* CT_UV(tmp) = NULL;															\
	type _data = NULL; 																					\
	if (CT_UV(head) != NULL) { 																			\
		_data = _ct_ht_entry_payload(CT_UV(head)); 														\
	} 																									\
	for(																								\
		(CT_UV(el)=(CT_UV(head))), 																		\
		((*(char**)(&(CT_UV(tmp))))=(char*)((CT_UV(head) != NULL)?_ct_ht_entry_next(CT_UV(head)):NULL)) \
		; 																								\
		CT_UV(el) != NULL 																				\
		; 																								\
		(CT_UV(el)=(CT_UV(tmp))), 																		\
		_data=(CT_UV(el) != NULL) ? _ct_ht_entry_payload(CT_UV(el)) : NULL,								\
		((*(char**)(&(CT_UV(tmp))))=(char*)((CT_UV(tmp)!=NULL)?_ct_ht_entry_next(CT_UV(tmp)):NULL))		\
	)


#endif /* HASHTABLE_H_ */
/**
 * @file
 *
 * Basic linked-list implementation
 *
 * The list implemented is a simple one (**not** double-linked).
 * In the memory, it might resemble to:
 *
 * @dot
 * digraph {
 * 	rankdir="LR";
 * 	A -> B -> C;
 * }
 * @enddot
 *
 * The list is composed by "entries": each entry contains a pointer to the value in the list and a pointer to the next entry
 *
 * @author koldar
 * @date May 17, 2017
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdbool.h>



struct ct_list;
typedef struct ct_list ct_list_o;

struct ct_list_entry;
typedef struct ct_list_entry ct_list_entry_o;

/**
 * Initialize a new list inside the memory.
 *
 * Use ::ct_list_destroy or ::ct_list_destroy_with_elements to release the memory from the ct_list_o
 *
 * @return the requested list
 */
ct_list_o* ct_list_init();

/**
 * Destroy the list
 *
 * \note
 * The payload within it won't be release from the memory **at all**!
 *
 * @param[inout] l the list to handle
 * @see ct_list_destroy_with_elements
 */
void ct_list_destroy(const ct_list_o* l);

/**
 * like ::ct_list_destroy but it releases from the memory all the elements within the list as well
 *
 * @param[inout] l the list to handle
 * @param[in] d a function used to dispose all values within the list from memory
 */
void ct_list_destroy_with_elements(const ct_list_o* l, ct_destroyer_c d);

/**
 * Remove all the payloads from the list
 *
 * \note
 * List items won't be removed from memory at all
 *
 * @param[inout] l the list to handle
 */
void ct_list_clear(ct_list_o* l);

/**
 * Adds a new element at the beginning of the list
 *
 * @param[inout] l the list we need to prepend a new item in
 * @param[in] el the item to prepend
 */
void ct_list_add_head(ct_list_o* l, const void* el);

/**
 * Adds a new element at the end of the list
 *
 * @param[inout] l the list we need to append a new item in
 * @param[in] el the item to append
 */
void ct_list_add_tail(ct_list_o* l, const void* el);

/**
 * The size of the list
 *
 * @param[in] l the list to handle
 * @return the length of the list
 */
int ct_list_size(const ct_list_o* l);

/**
 * Adds all the elements of \c src into \c dst
 *
 * \note
 * After this function, \c src will be empty
 *
 * @param[inout] dst the list that will accept all the elements inside \c scc;
 * @param[inout] src the list whose elements will be tranferred to \c
 */
void ct_list_full_transfer(ct_list_o* restrict dst, ct_list_o* restrict src);

/**
 * Check if the given list is empty
 *
 * @param[in] l the list to analyze
 * @return
 * 	\li @true if \c l is empty;
 * 	\li @false otherwises
 */
bool ct_list_is_empty(const ct_list_o* l);

/**
 * Get and remove the head from the list \c l
 *
 * @param[inout] l the involved list
 * @return
 * 	\li The payload at the head of the list;
 * 	\li @null Wheter the list is empty;
 */
void* ct_list_pop(ct_list_o* l);

/**
 * Fetch the head of the list
 *
 * @param[in] l the list to handle
 * @return
 * 	\li the head of the list;
 * 	\li NULL if \c l is empty;
 */
void* ct_list_head(const ct_list_o* l);

/**
 * Fetch the tail of the list
 *
 * @param[in] l the list to analyze
 * @return
 * 	\li the tail of the list;
 * 	\li @null if \c l is empty
 */
void* ct_list_tail(const ct_list_o* l);

/**
 * Get the n-th element of the list
 *
 * \attention
 * This method is **highly** inefficient and it should be used only for debugging purposes, not in production!
 *
 * The operation take \f$O(n)\f$ time
 *
 * @param[in] list the list to analyze
 * @param[in] index the value in the \c index th position
 * @return
 * 	\li the payload
 * 	\li @null if the length of \c list is smaller than \c index itself;
 */
void* ct_list_get(const ct_list_o* list, int index);

/**
 *
 * \attention
 * You should use this function only inside ::CT_VARIABLE_ITERATE_ON_LIST. Other uses may (and often will) lead to **undefined behaviour**!
 *
 * This function will allow you to dynamically remove elements inside a list when you're iterating it.
 * A typical example might be:
 *
 * @code
 * CT_VARIABLE_ITERATE_ON_LIST(&list, previous, entry, payload) {
 * 	if (condition_to_payload()) {
 * 		ct_list_remove_entry_dynamic(&list, &previous, entry);
 * 	}
 * }
 * @endcode
 *
 * @param[in] l the list to change
 * @param[inout] previous_entry the entry that is positioned just before the one to remove. After this, \c previous_entry pointer will contain @null
 * @param[in] entry_to_remove the entry you want to remove
 *
 */
void ct_list_remove_entry_dynamic(ct_list_o* l, ct_list_entry_o** previous_entry, ct_list_entry_o* entry_to_remove);

/**
 * Get the next entry in the list.
 *
 * @param[in] entry the current element of the list
 * @return a entry containing the next element of the list or @null if we are currently on the tail of the list
 */
ct_list_entry_o* _ct_list_get_next_entry(const ct_list_entry_o* entry);

/**
 * Get the payload of the given list entry
 *
 * @param[in] entry the entry to handle
 * @return the payload within the entry. @null values doesn't necessary mean there is no attached value in the entry: for istance, a list of integer may
 * have such values representing a simple 0.
 */
void* _ct_list_get_entry_payload(const ct_list_entry_o* entry);

/**
 * Get the first entry of the list
 *
 * @param[in] l the list to handle
 * @return the first entry in the list
 */
ct_list_entry_o* _ct_list_head_entry(const ct_list_o* l);

/**
 * Allows you to transparently iterate through a list
 *
 * This macro is *slightly* faster than ::CT_VARIABLE_ITERATE_ON_LIST but it doesn't allow you to
 * safely remove entries while iterating
 *
 * @param[in] _l a double point to the list you want to iterate through;
 * @param[in] entry name of the variable that will represent the entry under analysis
 * @param[in] _payload name of the variable representing <tt>entry->payload</tt>
 * @param[in] type type of the variable _payload
 */
#ifdef CT_ITERATE_ON_LIST
#	error "CrashC - CT_ITERATE_ON_LIST macro already defined!"
#endif
#define CT_ITERATE_ON_LIST(_l, entry, _payload, type) 												\
		const ct_list_o* CT_UV(l) = (_l);															\
		ct_list_entry_o* entry = _ct_list_head_entry(CT_UV(l)); 									\
		ct_list_entry_o* CT_UV(next) = NULL;														\
		type _payload = NULL;																	\
		if (entry != NULL) {																	\
			_payload = _ct_list_get_entry_payload(entry);										\
			if (_ct_list_get_next_entry(entry) != NULL) {										\
				CT_UV(next) = _ct_list_get_next_entry(entry);										\
			}																					\
		}																						\
		for (																					\
				;																				\
				entry != NULL;																	\
				entry = CT_UV(next), 																\
				CT_UV(next) = (entry != NULL) ? _ct_list_get_next_entry(entry) : NULL, 			\
				_payload = (entry != NULL) ? _ct_list_get_entry_payload(entry) : NULL			\
		)

/**
 * Allows you to transparently iterate through a list
 *
 * This macro is *slightly* slower than ::CT_ITERATE_ON_LIST but allows you to
 * safely remove ct_list_entry_o while iterating.
 *
 * A typical example might be:
 *
 * @code
 * CT_VARIABLE_ITERATE_ON_LIST(&list, previous, entry, payload) {
 * 	if (conditionToPayload()) {
 * 		ct_list_remove_entry_dynamic(&list, &previous, entry);
 * 	}
 * }
 * @endcode
 *
 * @param[in] _l a double point to the ct_list_o you want to iterate through;
 * @param[in] previous_entry name of the variable that will represent the entry just analyzed.
 * @param[in] entry name of the variable that will represent the entry under analysis
 * @param[in] _payload name of the variable representing <tt>entry->payload</tt>
 * @param[in] type type of the variable _payload
 */
#ifdef CT_VARIABLE_ITERATE_ON_LIST
#	error "CrashC - CT_VARIABLE_ITERATE_ON_LIST macro already defined!"
#endif
#define CT_VARIABLE_ITERATE_ON_LIST(_l, previous_entry, entry, _payload, type) 						\
		ct_list_o* CT_UV(l) = (_l);																\
		ct_list_entry_o* CT_UV(next) = NULL;														\
		type _payload = NULL;																	\
		if (_ct_list_head_entry(CT_UV(l)) != NULL) {												\
			_payload = _ct_list_get_entry_payload(_ct_list_head_entry(CT_UV(l)));					\
			if (_ct_list_get_next_entry(_ct_list_head_entry(CT_UV(l))) != NULL) {					\
				CT_UV(next) = _ct_list_get_next_entry(_ct_list_head_entry(CT_UV(l)));					\
			}																					\
		}																						\
		for (																					\
				ct_list_entry_o 																\
				*previous_cell = NULL, 															\
				*CT_UV(previous_cell_tmp) = NULL, 													\
				*entry = _ct_list_head_entry(CT_UV(l)) 											\
				; 																				\
				entry != NULL 																	\
				; 																				\
				CT_UV(previous_cell_tmp) = previous_cell == NULL ? CT_UV(previous_cell_tmp) : entry, 	\
				previous_cell = CT_UV(previous_cell_tmp), 											\
				entry = CT_UV(next), 																\
				CT_UV(next) = (entry != NULL) ? _ct_list_get_next_entry(entry) : NULL, 			\
				_payload = (entry != NULL) ? _ct_list_get_entry_payload(entry) : NULL			\
		)


#endif /* LIST_H_ */
/**
 * @file
 *
 * Contains API for handling tags
 *
 * @date Feb 16, 2017
 * @author koldar
 */

#ifndef TAG_H_
#define TAG_H_

#include <stdbool.h>



/**
 * represents a single tag
 *
 * @definition Tag
 * It's simply a string label that can be attached to a @containablesection, like @testcase, @when or even @testsuite
 */
struct ct_tag {
	/**
	 * The name of the tag
	 */
	char* name;
};

/**
 * Alias to improve readability inside the code
 */
typedef ct_hashtable_o ct_tag_hashtable_o;

/**
 * Initialize a new tag
 *
 * @param[in] name the tag name
 * @return the instance of the new tag in the heap
 */
struct ct_tag* ct_tag_init(const char* name);

/**
 * Removed from memory a previously created tag
 *
 * @param[inout] tag the tag to destroy
 */
void ct_tag_destroy(struct ct_tag* tag);

/**
 * Compare 2 tags
 *
 * @param[in] tag1 the first tag
 * @param[in] tag2 the second tag
 * @return
 * 	\li \f$ ret < 0 \f$ if the name of the first tag is less than  the name of the second tag (with \c strcmp);
 * 	\li 0 if the 2 tags are the same (either by pointer or by tag names);
 * 	\li \f$ ret > 0 \f$ if the name of the first tag is greater than  the name of the second tag (with \c strcmp);
 */
int ct_tag_compare(const struct ct_tag* tag1, const struct ct_tag* tag2);

/**
 * Check if 2 tag sets have an intersection
 *
 * 2 tag hashtables have an intersection if at least one tag is present both in \c tag_set1 and in \c tag_set2
 *
 * @param[in] tag_set1 the first tagset to compute
 * @param[in] tag_set2 the second tagset to compute
 * @return
 * 	\li @true if the 2 tagsets have something in common;
 * 	\li @false otherwise;
 */
bool ct_have_tag_set_intersection(const ct_tag_hashtable_o* tag_set1, const ct_tag_hashtable_o* tag_set2);

/**
 * Adds a new tag inside a hashtable
 *
 * \post
 * 	\li after this operation, the hastable has increased its size by 1
 *
 * @param[inout] tag_hashable hashtable to populate
 * @param[in] name name of the tag to add
 */
void ct_tag_ht_put(ct_tag_hashtable_o* tag_hashable, const char* name);


//TODO maybe we should put this function in utils.h
/**
 * the hash of a string
 *
 * @param[in] str the string whose hash we want to compute
 * @return the hashcode of a string
 */
int ct_string_hash(const char* str);

/**
 * Fetch the next token in the stream of characters representing a tag
 *
 * @param[in] str the string where we need to fetch the next token
 * @param[in] separator a character representing when a token ends and when it starts
 * @param[in] characters_to_ignore a string containing a list of characters we can safely ignore in \c str
 * @param[inout] output a buffer that will contain the token just read
 * @return the first character of the next new token or 0 if we reached the end of the string
 */
const char* ct_next_tag_in_string(const char* const str, char separator, char* characters_to_ignore, char* output);

/**
 * adds in \c output all the tags inside the stream of tags
 *
 * \post
 * 	\li \c output size increased
 *
 * @param[inout] output the hashtable where to add every tag found in \c tags
 * @param[in] tags a string containing tags, each of them separated by \c separator
 * @param[in] separator a character separating 2 tags. No double separators allowed
 */
void ct_tag_ht_populate(ct_tag_hashtable_o* output, const char* const tags, char separator);

#endif /* TAG_H_ */
/**
 * @file
 *
 * Contains generic utilities used throughout all the project
 *
 * @author noodles
 * @date Oct 31, 2017
 */

#ifndef CT_UTILS_H_
#define CT_UTILS_H_

#include <time.h>

//TODO: Maybe improve this function
/**
 * Returns the basename of a file, given its absolute path.
 *
 * \note
 * This function is written specifically to fit @crashc needs: we get the file name
 * from the \c __FILE__ macro, which always expand to an absolute filename. Due to the fact that
 * the filename is included compile-time in the binary we also return a pointer to a specific
 * offset in the binary in order not to need any dynamic allocation
 *
 * TODO is this example correct?
 * For example, if the input is <tt>/home/koldar/crashc/mytest.c</tt> the return value will be <tt>mytest.c</tt>
 *
 * @param[in] filename the absolute filename returned by \c __FILE__
 * @return the basename of the absolute file
 */
char* const ct_file_basename(char* const filename);

/**
 * Get the time *now*
 *
 * @return struct holding information about the time when this function has been called
 */
struct timespec ct_get_time();
/**
 * Get a time interval
 *
 * @param[in] start the start of the time interval
 * @param[in] end the end of the time interval
 * @param[in] format_str either one of the following options:
 * 	\li "s" if you want to the time interval in seconds;
 *  \li "m" if you want to the time interval in milliseconds;
 *  \li "u" if you want to the time interval in microseconds;
 *  \li "n" if you want to the time interval in nanoseconds;
 * @return the time elapsed between \c start and \c end
 * @see ct_get_time
 */
long ct_compute_time_gap(struct timespec start, struct timespec end, const char* format_str);



#endif /* CT_UTILS_H_ */
/**
 * @file
 *
 * Provides struct ct_section, which is used to represents @containablesection
 *
 * @author koldar
 * @date Feb 16, 2017
 */

#ifndef SECTION_H_
#define SECTION_H_

#include <stdbool.h>






/**
 * Represents the type of a ::ct_section
 *
 * section type is used to uniquely determine the behaviour of the section itself.
 */
enum ct_section_type {
	/**
	 * The section is the root of the tree.
	 */
	CT_ROOT_SECTION,
	/**
	 * The section is a test suite
	 */
	CT_TESTSUITE_SECTION,
	/**
	 * The section is a testcase
	 */
	CT_TESTCASE_SECTION,
	/**
	 * The section is a when
	 */
	CT_WHEN_SECTION,
	/**
	 * The section is a then
	 */
	CT_THEN_SECTION,
};

/**
 * Represents each state a ::ct_section can have
 *
 * ::ct_section state is used to check whether or not @crashc runtime should access to the related @containablesection code
 * or not. This answer greatly depends on the type of the given ::ct_section as well.
 */
enum ct_section_status {
	/**
	 * Represents a section @crashc knows exists, but it didn't visited it at all
	 */
	CT_SECTION_UNVISITED,

	/**
	 * Represents a section @crashc has visited, but it didn't fully visited
	 *
	 * @definition Partially visited section
	 * It's a section where at least one **direct** or **indirect** child section has not been visited
	 */
	CT_SECTION_PARTIALLY_VISITED,

	/**
	 * Represents a section CrashC has fully explored
	 *
	 * @definition Fully visited section
	 * It's a section where all its direct and indirect children sections have been fully visited
	 */
	CT_SECTION_FULLY_VISITED,

	/**
	 * A section has this state when inside its body the program encountered a signal
	 *
	 * The status is set only when the code **directed** owned by the section generates a signal. So for example in
	 * the figure below the signal was raised during the source code of section "B", after executing section "C" but before section "D"
	 *
	 * @dotfile signalDetectedTree.dot
	 *
	 * Sections with this status should not be visited anymore (so even if there are undirect children with status ::CT_SECTION_UNVISITED,
	 * those section will never be run at all.
	 */
	CT_SECTION_SIGNAL_DETECTED,

	/**
	 * A section has this state when we encountered it but we didn't explore its body because the tags were uncompliant with the context
	 *
	 * Suppose the user chose to exclude sections whose tags contains "SKIP". If we encounter any section containing the tag "SKIP"
	 * we need to mark it somehow. This status represents this very concept.
	 */
	CT_SECTION_SKIPPED_BY_TAG,
};


/**
 * Defines the possible statuses in which a snapshot can be.
 *
 * This indicates the "exit status" of the section in the particular test
 */
enum ct_snapshot_status {
	/**
	 * A snapshot contains all **passed assertions**
	 */
	CT_SNAPSHOT_OK,

	/**
	 * A snapshot run a code which produced an unhandled signal
	 */
	CT_SNAPSHOT_SIGNALED,

	/**
	 * A snapshot contained a **failed assertions**
	 *
	 */
	CT_SNAPSHOT_FAILED,
};

/**
 * Contains relevant test results information contained in a ::ct_section at a precise moment of the program execution
 *
 * @definition struct ct_section Snapshot
 * Represents the in-test information a given ::ct_section has. Note that one ::ct_section may actually have several, different, section shapshot, since
 * a @containablesection represented by a ::ct_section can be run multiple times in a @testcase. A section snapshot is tree because it is a run
 * of a single cycle of a ::CT_LOOPER, like @testcase: in it, the section are organized as a **tree**, not just as a list. For example a @then may have multiple
 * but different subsection @then.
 *
 * Such information are then used by the test reports to yield to the user a more detailed set of informations about a given test.
 * Note that this structure contains only the fields of a ::ct_section which are subject to changes during the execution
 * of the tests. Furthermore, note that this structure represents an implicit tree, used by @crashc to track the relationships between the
 * sections involved in a precise test
 */
struct ct_snapshot {
	/**
	 * The description of the ::ct_section represented by the struct
	 *
	 * @notnull
	 */
	char* description;

	/**
	 * The tags associated to the ::ct_section represented by the struct
	 *
	 * @notnull
	 */
	ct_tag_hashtable_o*  tags;

	/**
	 * The type of the ::ct_section represented by the struct
	 */
	enum ct_section_type type;

	/**
	 * The status of the snapshot.
	 *
	 * This pretty much just mimic the status the section to which this snapshot
	 * refers to, but without some section status which makes no sense in the
	 * context of the snapshot: for example CT_SECTION_UNVISITED, CT_SECTION_PARTIALLY_VISITED are one of them
	 */
	enum ct_snapshot_status status;
	/**
	 * The amount of time, in microseconds, the section ran during one specific test.
	 *
	 * This field has no meaning until the associated ::ct_section has been completely exeuted during the specific test
	 */
	long elapsed_time;

	/**
	 * The list of reports of the assertions executed in the ::ct_section represented.
	 *
	 * @notnull
	 */
	ct_list_o* assertion_reports;

	/**
	 * The pointer to the parent snapshot in the snapshot tree.
	 *
	 * If @null, the snapshot is actually the root of the tree
	 */
	struct ct_snapshot* parent;

	/**
	 * The pointer to the next sibling of this snapshot in the snapshot tree
	 *
	 * If @null, the snapshot is actually the last child of the parent in the snaphshot tree
	 */
	struct ct_snapshot* next_sibling;

	/**
	 * The pointer to the first child of this snapshot in the snapshot tree
	 *
	 * If @null, the snapshot has no children, therefore it's a leaf in the snapshot tree
	 */
	struct ct_snapshot* first_child;
};

/**
 * Main structure representing a piece of testable code
 *
 * @definition section
 * It's data structure representing, in @crashc framework, a given @containablesection.
 * ::ct_section is one of the most important data structure in @crashc. section are hierarchuically organized in a tree, called **section tree**
 *
 * Examples of sections may be @testcase, @when, @then and so on. Whilst this structure **does not contain**
 * any of the code inside the section, it represents the metadata of such code. An example of metadata is the number of
 * subsections (aka children) a section has. We need them in order to generate useful statistics.
 *
 * For example the following code is structure as exposed in the graph:
 *
 * @include exampleOfTestCase.dox
 *
 * Sections can be organized as in a tree, like this:
 *
 * @dotfile exampleOfTestCase.dot
 *
 * Note that the code inside "when 2" is all the code between the thens plus the code within each @then (a total of 9 lines).
 */
struct ct_section {

	/**
	 * An id that uniquely identifies a particular section.
	 *
	 * Like the pointer of the section, but more easy to read. Used internally
	 */
	int id;

	/**
	 * Represents the type of this section.
	 *
	 * For example you can use this field to get whether the section is a @when, a @then or something else
	 */
	enum ct_section_type type;

	/**
	 * Description of the section
	 *
	 * @notnull
	 */
	const char* description;

	/**
	 * List of tags associated to the section
	 *
	 * @notnull
	 */
	ct_tag_hashtable_o* tags;

	/**
	 * determine if ::ct_section::children_number has a meaning
	 *
	 * The field is @true if we have already scanned the section content at least once. This means
	 * that we explore the associated @containablesection from the beginning till the end. This has 2 effects:
	 * 	\li we know whether there are subsections inside the @containablesection or not;
	 *	\li we know the actual number of children the section has;
	 *
	 * The second effects allows us to safely use ct_section::children_number and ct_section::first_child
	 */
	bool children_number_known;

	/**
	 * the number of subsection this section has
	 *
	 * This field has only a meaning when ::ct_section::children_number_known is set to @true.
	 * For example, given the code:
	 *
	 * @include exampleOfTestCase.dox
	 *
	 * section "when 2" has 2 children, namely "then 1" and "then 2"
	 */
	int children_number;

	/**
	 * Holds information about the current state of the section.
	 *
	 */
	enum ct_section_status status;

	/**
	 * The number of the child we're currently analyzing
	 *
	 * Given the test code:
	 *
	 * @include exampleOfTestCase.dox
	 *
	 * Suppose you are in "when 3" for the first time (this also applies, although for different reasons, even if you're exploring the section
	 * for the n-th time). Since when you just entered in "when 3" you can't possibly know the exact children of the section, since the program still needs to read
	 * the code, you can't immediately create the relevant section tree.
	 *
	 * After you've done analyzing a subsection (i.e "then 3"), you need to increase a counter representing the number of children you've encoutered
	 * so far when analyzing "when 3" and keep analyzing "when 3" by executing the code, eventually arriving to the next section child (i.e. "then 4").
	 * This field does exactly represent such counter. When the program arrives at the end of "when 3", this counter will also represents the actual number of children
	 * "when 3" has.
	 */
	int current_child;

	/**
	 * The number of times the @crashc encountered this section instance
	 *
	 * \note
	 * "encounters" means that we reach the macro definition. It doesn't matter if we actually enter or not, the counter will increase nonetheless
	 */
	int times_encountered;

	/**
	 * The variable used to loop only once inside parent switcher in ::CT_CONTAINABLE_SECTION
	 *
	 * \attention this variable is for internal usage only
	 *
	 * The for loop is an excellent way to have, after a macro, "{ }" and to execute code after the actual "{ }" code content.
	 * However, we need to call the for content only once. Since section are nested, we cannot define a counter to loop over otherwise
	 * 2 nested section will generate a <c>variable redefinition</c> compile error. Hence we store the loop variable inside the section itself.
	 */
	bool loop1;

	/**
	 * like ::ct_section::loop1, but for the access cycle inside ::CT_CONTAINABLE_SECTION
	 */
	bool loop2;

	/**
	 * @true if the software has given us the access to execute the code inside the struct ct_section, @false otherwise
	 */
	bool access_granted;

	/**
	 * @true if the software has given us the access to execute the code tag-wise
	 *
	 * The software can deny the access to the underlying section for 2 reasons:
	 * \li the section has incompatible tags with the tag context of the run test;
	 * \li the condition of the section is not satisfied;
	 *
	 *
	 * The first check is computed before the second one. Together with  ::ct_section::access_granted, this field
	 * allow you to understand why a particular section has been denied. In particular this field is @true
	 * if the section tags are compatible with the tag context; @false otherwise
	 */
	bool tag_access_granted;

	/**
	 * Used to ensure that sibling @when sections are executed in the proper order.
	 *
	 * Since only a single @when needs to be executed during a given @testcase loop, we use
	 * this field in the parent section to know if during this cycle we already found and
	 * executed a @when section.
	 *
	 * This implies that this field needs to be reset at every @testcase cycle.
	 */
	bool already_found_when;

	/**
	 * The signal detected when running this section
	 *
	 * The value is meaningful only when ::ct_section::status is set to ::CT_SECTION_SIGNAL_DETECTED
	 */
	int signal_detected;

	///the parent of this section in the tree. May be @null
	struct ct_section* parent;

	///the first child this section has in the tree. May be @null
	struct ct_section* first_child;

	///the next sibling this section has in the tree. May be @null
	struct ct_section* next_sibling;
};

/**
 * Adds a ::ct_section inside the children list of a parent section
 *
 * @param[in] to_add the new child \c parent has;
 * @param[inout] parent the parent \c toAdd have from this point on;
 * @return \c toAdd
 */
struct ct_section* ct_section_add_child(struct ct_section* restrict to_add, struct ct_section* restrict parent);
/**
 * get the n-th child of a given ::ct_section
 *
 * @param[in] parent the parent involved
 * @param[in] n the number of the child we want to fetch
 * @return
 * 	\li the n-th child of parent;
 * 	\li NULL if such child does not exist
 */
struct ct_section* ct_section_get_child(const struct ct_section* parent, int n);

/**
 * Get the level of the given section in the hierarchy.
 *
 * @param[in] section The involved section
 * @return The level of the section in the hierarchy
 */
int ct_section_get_level(const struct ct_section* section);

/**
 * Creates a new section
 *
 * \attention
 * The function allocates data in the heap. To remove it, call ::ct_section_destroy
 *
 * @param[in] type the type of this section
 * @param[in] description a text describing briefly the section
 * @param[in] tags a single string containing all the tags associated to the section. See \ref tags
 * @return the new ::ct_section instance just created
 */
struct ct_section* ct_section_init(enum ct_section_type type, const char* description, const char* tags);
/**
 * Destroy a ::ct_section inside the heap
 *
 * \note
 * The function will destory all the sections which have parent (directly or indirectly) \c section.
 * Formally, the function will free from the memory the subtree generate by \c section (\c section included).
 *
 * There is only a ::ct_section that won't be freed by this function: all the sections with struct ct_section::levelId set to 0 won't be touched.
 * Normally this isn't a problem because there is only one ::ct_section with such an id: struct ct_section::rootSection. The rationale is that such variable is global one
 * allocated in the \b stack, so freeing such a variable is illegal.
 *
 * @param[inout] section the section to free
 */
void ct_section_destroy(struct ct_section* section);

/**
 * Check if we're still retrieving the number of children a given section has
 *
 * @param[in] section the section to analyze
 * @return
 * 	\li @true if we're still computing the number of children of \c section,
 * 	\li @false otheriwse
 */
bool ct_section_still_discovering_children(const struct ct_section* section);

/**
 * Marks the given section as a section where the related @containablesection raised a signal
 *
 * @param[inout] section the section to alter
 */
void ct_section_set_signaled(struct ct_section* section);

/**
 * Check if the related @containablesection has raise a signal
 *
 * @param[in] section the section to check
 * @return true if the section was marked as signal detected; false otherwise
 */
bool ct_section_is_signaled(const struct ct_section* section);

/**
 * Mark the section as **partially executed**
 *
 * @param[inout] section the section to mark
 */
void ct_section_set_executed(struct ct_section* section);

/**
 * Mark the section as **fully visited**
 *
 * @param[inout] section the section to mark
 */
void ct_section_set_done(struct ct_section* section);

/**
 * Mark section as a one which we didn't explore because of the tags
 *
 * @param[inout] section the section whose status we need to update
 */
void ct_section_set_skipped(struct ct_section* section);

/**
 * Tells if a given section will be executed again depending on its status.
 *
 * @param[in] section the section to manage
 * @return
 *  \li @true if the status is either ::CT_SECTION_UNVISITED or ::CT_SECTION_PARTIALLY_VISITED;
 *  \li @false iff the status is either ::CT_SECTION_SKIPPED_BY_TAG, ::CT_SECTION_FULLY_VISITED or ::CT_SECTION_SIGNAL_DETECTED;
 */
bool ct_section_still_needs_execution(struct ct_section* section);

/*
 * We use this function to determine whether we can set a section as fully visited.
 *
 * If so, we don't need to execute it anymore.
 * A section is considered fully executed in two cases:
 * -# When it has no child
 * -# When every **direct** children of the section is fully visited itself
 *
 * Note that we first check if the section has been executed at least once, as
 * if a given section has never been executed it surely can not be fully visited.
 *
 * @param[in] section the section to manage
 * @return
 * 	\li @true if we don't need to visit the section anymore;
 * 	\li @false otherwise
 */
bool ct_section_is_fully_visited(struct ct_section* section);

/**
 * Generates an image of the section tree useful for debugging purposes
 *
 * \pre
 * 	\li graphviz installed on your system and available in your PATH;
 * \post
 * 	\li a png file named like \c format will be present. The path is relative to the CWD
 *
 * @param[in] section the section tree where you want to start drawing
 * @param[in] format a `printf` like format string with all the formatter strings
 * @param[in] ... variadic components of `printf`
 */
void ct_section_draw_tree(const struct ct_section* section, const char* format, ...);

#endif /* SECTION_H_ */
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
/**
 * @file
 *
 * Module collecting all the error macros used in the project
 *
 * You can compile time customize the code when a specific error occurs.
 * Normally it simply call \c exit function with a specific error code
 *
 * @author koldar
 * @date Feb 15, 2017
 */

#ifndef ERRORS_H_
#define ERRORS_H_

/**
 * C code to execute when a \c malloc returns NULL
 *
 */
#ifndef CT_MALLOC_ERROR_CALLBACK
#	define CT_MALLOC_ERROR_CALLBACK() exit(1)
#endif



#endif /* ERRORS_H_ */
/**
 * @file
 *
 * Represents the main file of the framework
 *
 * The file contains all the main macro definition used to provide crashC features to the developer
 *
 * TEST CASE
 * ---------
 *
 * Test cases are particularly important sections: they serve as entry points for all our tests. They need:
 * 	\li a cycle: if there multiple WHEN clauses, we need to rerun their body until all the when clauses are run. This requires a cycle;
 * 	\li a function definition: test cases are not defined inside the main, so the only alternative is that they are functions. Why they can't be inside the main?
 * 		Because otherwise we would need to have "}" at the end of all the test cases. But with macro programming you can't simply do that (we may tell the user
 * 		to write the "}" by himself, but that would add boilerplate code to the test case, code that we do not want to add.
 *
 * Ok, how can we add a function definition that compiles? Remember: you can't add anything after you put the "{ ... }" of the test case.
 * The idea is simple: the test is a function and the "{ ... }" is its body. However in this way we can't have the cycle, cycle that we need to deal
 * with the when clauses. The solution is to have for every test, <b>2 functions</b>: "{...}" is  the body of the second function whilst the first function
 * is defined before the second one, like this:
 *
 * @code
 *
 * TESTCASE ("tc1", "important") {
 * 	//our beatiful tests
 * }
 *
 * //this becomes
 *
 * void test_1a() {
 * 	CT_LOOPER(...) {
 * 		test_1b();
 * 	}
 * }
 *
 * void test_1b() {
 * 	//our beatiful tests
 * }
 *
 * @endcode
 */

#ifndef CRASHC_H_
#define CRASHC_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>












/**
 * Callback type of a function representing a general condition that determine if we can access to a particular @containablesection source code
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[in] section the section representing the @containablesection you want to access
 * @return
 * 	\li @true whenever the testing framework decided that the code can enter in the @containablesection source code;
 * 	\li @false otherwise.
 */
typedef bool (*ct_access_c)(struct ct_model* model, struct ct_section*);

/**
 * Callback signature representing a callback to execute as soon as you **exit** from the @containablesection source code
 *
 * For example, suppose the following. Suppose you have exited from \c WHEN. The struct ct_model::current_section variable needs to be
 * reset from \c WHEN to its parent, namely \c TESTCASE. The function prototype allows you to code that
 *
 * @code
 * 	TESTCASE("","") {
 * 		WHEN("","") {
 * 		}
 * 	}
 * @endcode
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[inout] parent_position a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you'have just exit from.
 */
typedef void (*ct_exit_c)(struct ct_model* model, struct ct_section** parent_position, struct ct_section* child);

/**
 * Callback prototype for any function executing addtional code before entering in a @containablesection
 *
 * For example, asume you are in \c TESTCASE and assume the framework gave you access to \c WHEN @containablesection.
 * Before actually enter in the section, you might want to perform additional code. The prototype has been specified for this very reason
 * @code
 * 	TESTCASE("","") {
 * 		WHEN("","") {
 * 		}
 * 	}
 * @endcode
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access (hence the child).
 */
typedef void (*ct_enter_c)(struct ct_model* model, struct ct_section* section);

/**
 * Register a new @testsuite in the @crashc ecosystem
 *
 * C language doesn't have anything allowing you to fetch all defined functions within a file or a group of file (something like
 * Java's annotation or Python introspection). So, in order to detect all the suite the tests defined, one need to manually
 * register them in a data structure (like an array).
 *
 * This function goal is precisely this: it allows you to register a @testsuite within @crashc framework.
 *
 * TODO: Add control on duplicates testsuites
 *
 * \post
 * 	\li \c func added in the register
 *
 * @param[inout] model the model where we operate on
 * @param[in] func the function to register
 */
void ct_update_test_array(struct ct_model* model, ct_test_c func);

/**
 * Function to concretely perform the **access cycle**
 *
 * @param[inout] model the model containing all the data representing the automatic testing
 * @param[in] section the section we want to access in
 * @param[in] cs the condition we need to satisfy in order to access the section
 * @param[in] callback the code to execute if the system grant us access to the section. Note that in this way the code is called \b before entering in the section
 * @param[in] run_tags an hash table containing all the tags allowed. If a section does not have a tag inside this set, it won't be run
 * @param[in] exclude_tags an hash table containing all the tags prohibited. If a section has at least one tag inside this set, it won't be run
 * @return
 * 	\li @true if the section has to be visited;
 * 	\li @false otherwise;
 */
bool ct_run_once_check_access(struct ct_model* model, struct ct_section* section, ct_access_c cs, ct_enter_c callback, const ct_tag_hashtable_o* restrict run_tags, const ct_tag_hashtable_o* restrict exclude_tags);
/**
 * Function supposed to run in the **parent switcher** cycle
 *
 * @param[inout] model the model containing all the data representing the automatic testing
 * @param[in] section the section we want to fetch the associated parent from
 * @param[in] pointer_to_set_as_parent a pointer that we has to set to <tt>section->parent</tt> in this call. <b>This has to be done by exactly one of the callbacks</b>
 * @param[in] callback function **always called after we have check the access** to the section: if we have access, this function is called **after** we entered inside the section source code
 * @param[in] access_granted_callback function called \b afrer \c callback if the software has entered in the section source code
 * @param[in] access_denied_callback function called \b afrer \c callback if the software hasn't entered in the section source code
 * @return
 *	\li @true if this is the first iteration of the **parent switcher** cycle.
 *	\li @false if this si the second iteration of the  **parent switcher cycle.
 */
bool ct_run_once_final_work(struct ct_model* model, struct ct_section* section, struct ct_section** pointer_to_set_as_parent, ct_exit_c callback, ct_exit_c access_granted_callback, ct_exit_c access_denied_callback);

/**
 * Fetches the section representing a particular @containablesection. If not present, it creates it.
 *
 * @code
 * 	TESTCASE("","") {
 * 		WHEN("a","") {
 * 		}
 * 		WHEN("b","") {
 * 		}
 * 	}
 * @endcode
 * For example, this function creates the section representing the @containablesection @when "a" if there is no section inside the section tree
 * of @testcase representing it; otherwise, it just fetches such struct ct_section.
 *
 * \post
 * 	\li a ::ct_section representing the given @containablesection now exists within the section tree.
 *
 * @param[in] parent the section containing the one we're creating. For example if we're in the test code of @testcase and we see a @when clause
 * 				this attribute is set to the metadata representing @testcase.
 * @param[in] type the kind of section to fetch
 * @param[in] description a brief string explaining what this section is and does
 * @param[in] tags a list of tags. See \ref tags for further information
 * @return
 * 	\li a newly created section if we're still computing the children of \c parent
 * 	\li the struct ct_section::current_child -th child of \c parent otherwise
 */
struct ct_section* ct_fetch_section(struct ct_section* parent, enum ct_section_type type, const char* description, const char* tags);

/**
 * Reset the struct ct_model::current_section global variable to the given one after we have detected a signal
 *
 * Sometimes it happens that we need to abruptly break the flow of struct ct_model::current_section.
 * For example when we detect an unhandled signal in one of the sections, we don't need to return to the parent of the section involved,
 * but we need to immediately go to the @testcase.
 *
 * This function allows you to set all the metadata to ensure that such "unorthodox" flow is valid.
 *
 * \post
 * 	\li struct ct_model::current_section is valid and refers to \c testcase_section
 *
 * @param[inout] model the model containing all the data representing the automatic testing
 * @param[in] jump_source_section the section where a signal has been raised
 * @param[in] testcase_section the section representing a test case we need to reset the struct ct_model:current_section to
 */
void ct_reset_section_after_jump(struct ct_model* model, struct ct_section* const jump_source_section, struct ct_section* const testcase_section);

///@defgroup accessConditions Access Condition Functions
///@brief Functions that can be used as ::ct_access_c concrete values.
///Use these functions to develop new @containablesection. These group of functions defines the pool of ::ct_access_c you can use
///to create new @containablesection s.
///@{


/**
 * We give access to a @when only if it's the first @when we're giving access to in the current level of the section tree
 *
 * We use this function to check whether or not we need to enter in the given WHEN section.
 * You gain the access to a @when if:
 *
 * \li the @when has not been fully visited;
 * \li given the parent @containablesection, it's the first @when you can access to;
 *
 * To clarify, consider the following situation:
 *
 * @dotfile accessWhen
 *
 * -# At first you enter in @when 1, then you enter in @when 1,1. After that you avoid both @when 1,2 and @when 1,3 because in the current
 * @testcase cycle, you've already accessed to a @when within the @containablesection called @when 1. The same can be said for @when 2:
 * in the current @testcase cycle, you've already accessed to a @when within the @containablesection called @testcase.
 * -# In the second @testcase cycle, you still enter in @when 1 but, since @when 1,1 has already been visited, you gain the access to @when 1,2.
 * Again, you avoid entering in @when 1,3 since you've already gained access to a @when in the same level (namely @when 1,2). @when 2 follows the
 * same principle, since you've entered in @when 1.
 * -# In the third loop, you gain access, as per usual, to @when 1 (since it's not fully visited yet). Within it, you access to @when 1,3. After it, @when 1
 * is finally fully visited. Again, you don't access to @when 2 since you've visited @when 1 in this @testcase cycle loop.
 * -# Finally you avoid entering in @when 1 since it's fully visited but you access to @when 2.
 *
 * @param[in] model the model involved
 * @param[in] section the section we're trying to access
 * @return
 * 	\li \true if we can access to section \c section;
 * 	\li \false otherwise
 */
bool ct_get_access_when(struct ct_model* model, struct ct_section* section);

/**
 * Grants always access
 *
 * This condition allows you to **always** gain the access to the @containablesection
 *
 * @param[in] model the model involved
 * @param[in] section the section we're trying to access
 * @return
 * 	\li true if we can access to section \c section;
 * 	\li false otherwise
 */
bool ct_always_enter(struct ct_model* model, struct ct_section* section);


///@}

///@defgroup beforeEntering Before Entering Containable struct ct_section Functions
///@brief Functions that can be used as ::ct_enter_c concrete values.
///Use these functions to develop new @containablesection. These group of functions defines the pool of ::ct_enter_c you can use
///to create new @containablesection s.
///@{

/**
 * Perform setup operations before entering in a @when
 *
 * This callback should be executed when we get access granted to a WHEN section, before executing its code.
 * It sets ::ct_section::already_found_when of struct ct_model::current_section and takes a snapshot of the WHEN section for the test report
 * and adds it to the test report snapshots tree.
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access.
 */
void ct_callback_entering_when(struct ct_model* model, struct ct_section* section);

/**
 * Perform setup operations before entering in a @then
 *
 * This callback should be executed when we get access granted to a THEN section, before executing its code.
 * It takes a snapshot of the section and adds it to the current_section's test report snapshots tree.
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access.
 */
void ct_callback_entering_then(struct ct_model* model, struct ct_section* section);

///@}

/**
 * Updates the current_snapshot, creating a new one if we started a new test
 * or adding a snapshot to the current test snapshots tree
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access.
 */
void ct_update_current_snapshot(struct ct_model* model, struct ct_section* section);

///@defgroup afterEntering After Entering Containable struct ct_section Functions
///@brief Callbacks that can be used in ::ct_run_once_final_work as callbacks. Set of candidate callbacks to be called after a struct ct_section **access cycle**, **regardless** of its outcome.
///These callbacks have the main task to repair struct ct_model::current_section in order to ensure that the @containablesection
///we're actually in when the code is executing is the same pointed by the section tree.
///Furthermore, these callbacks fetches new data useful for other @crashc task (i.e. test reports).
///@{

/**
 * updates struct ct_model::current_section and increases the children of the parent by 1
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you have just exit from (hence it's the child section).
 */
void ct_exit_callback_next_sibling(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child);
/**
 * Update section tree metadata whenever you have successfully entered in a child @containablesection
 *
 * If you have just exited from a @containablesection, this means you know the number of **direct** children it contains.
 * This function then performs the following operations:
 *  \li Mark the \c section as "now we know the exact number of children you have";
 *  \li Updates struct ct_model::current_snapshot;
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you have just exit from (hence it's the child section).
 */
void ct_exit_callback_children_number_computed(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child);

//TODO find a new name. It's quite similar to ct_exit_callback_children_number_computed, so maybe we should have 2 similar names?
/**
 * like ::ct_exit_callback_children_number_computed but it doesn't update the struct ct_model::current_snapshot
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you have just exit from (hence it's the child section).
 */
void ct_exit_callback_reset_container(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child);

/**
 * Do nothing
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] child the section representing the @containablesection you have just exit from (hence it's the child section).
 */
void ct_exit_callback_do_nothing(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child);

/**
 * Finalize operations when a @testcase is finished.
 *
 * it simply updates the test report outcome depending on the status of the last snapshot of the test tree
 * and resets the struct ct_model::current_snapshot pointer back to NULL to indicate that the current test is over
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[inout] pointer_to_set_as_parent a pointer **you** need to set to the parent of the section representing the @containablesection
 * @param[in] section the section representing the @containablesection you'have just exit from.
 */
void ct_exit_callback_access_granted_testcase(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* section);

///@}

///\defgroup accessGrantedCallBack Access Granted Callbacks
///@brief Callbacks that can be used in ::ct_run_once_check_access as \c access_granted_callBack. These callbacks are useful to
///create new @containablesection implementations
///@{

/**
 * Does nothing
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access (hence the child).
 */
void ct_callback_do_nothing(struct ct_model* model, struct ct_section* section);

/**
 * Setup function for @testcase sections
 *
 * It initializes the test report associated to the particular looper iteration,
 * adds it to the test reports list and takes the snapshot of the testcase.
 *
 * @param[inout] model the global struct ct_model crashC model you manage
 * @param[in] section the section representing the @containablesection you're going to access (hence the child).
 */
void ct_callback_entering_testcase(struct ct_model* model, struct ct_section* section);

///@}

/**
 * Main macro of CrashC
 *
 * @definition
 * A containable section is defined as a piece of developer code you might want to test. To delimit it, containable section
 * are often wrap in curly brackets. Containable sections may be hierarchical organized: that is, one containable section X may contain
 * multiple "children" containable sections, which have one and only parent containable section, namely X.
 * Access to containable section may be granted depending on **specific conditions**.
 * The main goal of containable sections is to forcefully change the flow of the developer code,
 * regardless on how the code may appear from the outside. For example, code in \c WHEN sections seems to be executable just after the code
 * shown before it. Actually code in \c WHEN section is executed depending on a much more complex condition.
 * Concretely a @containablesection is 2 nested for...loop cycle that, at the end, are just 2 masked \c if conditions.
 *
 *
 * @definition
 * ::CT_CONTAINABLE_SECTION is 2 nested for...loop cycles. The outermost is the **parent switcher**, allowing you to keep synchronized the
 * @containablesection the code is actually in with the metadata representing @containablesection, namely the tree formed by ::ct_section.
 * This syncronization is a two-pass system: first it change the struct ct_model::current_section from the parent @containablesection to the child one,
 * **regardless** if you have the access to the child. After fully execute the **access cycle**, it repairs struct ct_model::current_section by updating
 * it from the child to the parent. Code-wise, just like ::ct_run_once_check_access, we model the if with a for loop. Since the for loop condition is run twice, we need to ensure the
 * cycle condition is in the first call @true whilst in the second one @false (this ensure the "if" behaviour"). Moreover we want to perform additional
 * code. Such operation is done by ::ct_run_once_final_work.
 *
 *
 * @definition
 * ::CT_CONTAINABLE_SECTION is composed by 2 loops: the most inner one is the **access cycle** that checks if we can **actually enter** in the @containablesection
 * represented by a  struct ct_section data structure. If the condition is not surpassed, the access cycle immediately release the control to the **parent switcher**. Otherwise
 * the @containablesection source code is executed. The for is implementing an \c if condition with the added feature to execute call code at the end. This function has to be called inside
 * the condition check of the for loop and ensures the loop behaves like an \c if: this is necessary because for loops condition has to be checked twice: one
 * to check if we can access to the section and one (if we have entered inside the loop) to exit from the loop itself
 *
 * @param[inout] model variable of type pointer of struct ct_model containing all the data to manage
 * @param[in] parent the ::ct_section this @containablesection is contained
 * @param[in] section_type (whose type is ::ct_section_type) the type of the ::ct_section representing this @containablesection
 * @param[in] description (whose type is <tt>char*</tt>) a brief description of this @containablesection
 * @param[in] tags a value (whose type is <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 * @param[in] condition the condition (whose type is ::ct_access_c)you need to clear in order to gain access to the internal test code of the @containablesection
 * @param[in] access_granted_callback a callback (whose type is ::ct_enter_c) representing a set of instructions to execute if you gain access to the internal test code. This function will be execute before executing the actual test code
 * @param[in] back_to_parent_callback a callback (whose type is ::ct_exit_c) executed when you're surpassing this @containablesection. Note that this function will be called regardless if you actually enter inside the code or not.
 * @param[in] exit_access_granted_callback a callback (whose type is ::ct_exit_c)to execute if you entered inside the test code of the @containablesection. Called after \c back_to_parent_callback;
 * @param[in] exit_access_denied_callback a callback (whose type is ::ct_exit_c)to execute if you didnt' enter inside the test code of the @containablesection. Called after \c back_to_parent_callback;
 * @param[in] setup_code a piece of C code to paste in the source code before starting all the reasoning for the @containablesection. struct ct_model::current_section for this @containablesection is already populated though.
 */
#ifdef CT_CONTAINABLE_SECTION
#	error "CrashC - CT_CONTAINABLE_SECTION macro already defined!"
#endif
#define CT_CONTAINABLE_SECTION(model, parent, section_type, description, tags, condition, access_granted_callback, back_to_parent_callback, exit_access_granted_callback, exit_access_denied_callback, setup_code)	\
		/**
		 * Every time we enter inside a section (WHEN, THEN, TESTCASE) we
		 * create a new metadata data representing such section (if not created yet)
		 * and then we enter in such section. At the end of the execution,
		 * we return to the parent section
		 */																																								\
		(model)->current_section = ct_fetch_section(parent, section_type, description, tags);																			\
		(model)->current_section->times_encountered += 1;																															\
		setup_code																																						\
		for (																																							\
				(model)->current_section->loop1 = true																													\
				;																																						\
				ct_run_once_final_work((model), (model)->current_section, &((model)->current_section), 																	\
						back_to_parent_callback, exit_access_granted_callback, exit_access_denied_callback																\
				)																																						\
				;																																						\
				/**
				 *  This code is execute when we have already executed the code
				 *  inside the container. We assume every post condition of
				 *  CT_CONTAINABLE_SECTION is satisfied for its children
				 *  CT_CONTAINABLE_SECTION. Here current_section has not been repaired yet!
				 */																																						\
				 (model)->current_section->loop1 = false																												\
		)																																								\
		for (																																							\
				(model)->current_section->loop2 = true																													\
				;																																						\
				ct_run_once_check_access((model), (model)->current_section, condition, access_granted_callback, (model)->run_only_if_tags, (model)->exclude_tags)		\
				;																																						\
				(model)->current_section->loop2 = false,																												\
				ct_section_set_executed((model)->current_section)																											\
		)

/**
 * Convenience macro for a NOOP
 */
#ifdef CT_NO_CODE
#	error "CrashC - CT_NO_CODE macro already defined!"
#endif
#define CT_NO_CODE

/**
 * A @containablesection which embeds a loop.
 *
 * The loop keeps going until the @containablesection is fully visited. This means a ::CT_LOOPER does not
 * stop until every single @containablesection within it has been correctly scanned.
 *
 * @param[inout] model a variable of type ::ct_model containing all the data needed by crashc
 * @param[in] parent a variable of type ::ct_section representing the parent section of this @containablesection
 * @param[in] section_type a value of type ::ct_section_type representing the type of this @containablesection
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#ifdef CT_LOOPER
#	error "CrashC - CT_LOOPER macro already defined!"
#endif
#define CT_LOOPER(model, parent, section_type, description, tags)																									\
		CT_CONTAINABLE_SECTION(																																		\
				(model),																																			\
				parent, section_type, description, tags,																											\
				ct_always_enter, ct_callback_entering_testcase, 																									\
				ct_exit_callback_reset_container, ct_exit_callback_access_granted_testcase,  ct_exit_callback_do_nothing, 											\
																																									\
				(model)->jump_source_testcase = (model)->current_section;																							\
				bool CT_UV(jump_occurred) = false;																													\
				if (sigsetjmp((model)->jump_point, 1)) {                                                                          					        		\
					/* We have caught a SIGNAL or an ASSERTION HAS FAILED: here current_section is the section where the signal was raised							\
					 * If the code enters in this if remember that current_section still refer to the containablesection where the signal has happended				\
					 * or the assertion have failed. If this is the case, you need to reset current_section to the containable section of the test case.			\
					 * */																																			\
					CT_UV(jump_occurred) = true; 																													\
					/*we reset the current_section to the test case*/																								\
					ct_reset_section_after_jump((model), (model)->current_section, (model)->jump_source_testcase);													\
				}																																					\
				for (    																																			\
						;																																			\
						!CT_UV(jump_occurred) && ct_section_still_needs_execution((model)->current_section)                                                   		\
						;																																			\
				)																																					\
		)


//TODO the parent of the test case is not the root section, but the "suite section". We need to include them in the section tree as well!
/**
 * Represents a @containablesection which keeps being executed until every direct and undirect @containablesection within it has been executed
 *
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#ifdef TESTCASE
#	error "CrashC - TESTCASE macro already defined!"
#endif
#define TESTCASE(description, tags) CT_LOOPER(ct_model, ((ct_model)->root_section), CT_TESTCASE_SECTION, description, tags)
/**
 * like ::TESTCASE but with the default \c tags value of ""
 *
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 */
#ifdef EZ_TESTCASE
#	error "CrashC - EZ_TESTCASE macro already defined!"
#endif
#define EZ_TESTCASE(description) TESTCASE(description, "")

/**
 * A @containablesection where you always gain access to
 *
 * @param[inout] model variable of type pointer of struct ct_model containing all the data to manage
 * @param[in] section_type (whose type is ::ct_section_type) the type of the ::ct_section representing this @containablesection
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#ifdef CT_ALWAYS_ENTER
#	error "CrashC - CT_ALWAYS_ENTER macro already defined!"
#endif
#define CT_ALWAYS_ENTER(model, section_type, description, tags) CT_CONTAINABLE_SECTION(															\
		(model), 																														\
		(model)->current_section, section_type, description, tags,																		\
		ct_always_enter, ct_callback_entering_then,																						\
		ct_exit_callback_next_sibling,	ct_exit_callback_children_number_computed, ct_exit_callback_do_nothing,							\
		CT_NO_CODE																															\
)

/**
 * A @containablesection where you gain access to only if has
 * not been fully visited and you didn't access to a similar section yet in the same parent.
 *
 * @param[inout] model variable of type pointer of struct ct_model containing all the data to manage
 * @param[in] section_type (whose type is ::ct_section_type) the type of the ::ct_section representing this @containablesection
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#ifdef CT_ENTER_ONCE_PER_LOOP
#	error "CrashC - CT_ENTER_ONCE_PER_LOOP macro already defined!"
#endif
#define CT_ENTER_ONCE_PER_LOOP(model, section_type, description, tags) CT_CONTAINABLE_SECTION(													\
		(model), 																														\
		(model)->current_section, section_type, description, tags,																		\
		ct_get_access_when, ct_callback_entering_when, 																					\
		ct_exit_callback_next_sibling, ct_exit_callback_children_number_computed, ct_exit_callback_do_nothing,								\
		CT_NO_CODE 																															\
)

/**
 * Represents a @containablesection with 2 access conditions:
 *
 * \li it has not been fully visited yet;
 * \li you still have not accessed to a @when in the same container
 *
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#ifdef WHEN
#	error "CrashC - WHEN macro already defined!"
#endif
#define WHEN(description, tags) CT_ENTER_ONCE_PER_LOOP((ct_model), CT_WHEN_SECTION, description, tags)

/**
 * like ::WHEN but with the default \c tags value of ""
 */
#ifdef EZ_WHEN
#	error "CrashC - EZ_WHEN macro already defined!"
#endif
#define EZ_WHEN(description) WHEN(description, "")

/**
 * Represents a @containablesection you need always to access to
 *
 * @param[in] description a value of type <tt>char*</tt> representing a brief description of the section
 * @param[in] tags a value of type <tt>char*</tt> representing all the tags within the section. See \ref tags for further information.
 */
#ifdef THEN
#	error "CrashC - THEN macro already defined!"
#endif
#define THEN(description, tags) CT_ALWAYS_ENTER((ct_model), CT_THEN_SECTION, description, tags)

/**
 * like ::THEN but with the default \c tags value of ""
 */
#ifdef EZ_THEN
#	error "CrashC - EZ_THEN macro already defined!"
#endif
#define EZ_THEN(description) THEN(description, "")

//TODO all those functions should be included in the only one global models
/**
 * Represents the default entry point for @crashc main executable
 *
 * The macro is used to contain all test declarations and to generate the main function for the
 * execution of the various tests.
 *
 * The macro is actually masking a \c main function
 *
 */
#ifdef TESTS_START
#	error "CrashC - TESTS_START macro already defined!"
#endif
#define TESTS_START int main(const int argc, char* const args[]) { 																\
		ct_model = ct_setup_default_model();																					\
		ct_parse_args(argc, args, CT_TAGS_SEPARATOR, (ct_model)->run_only_if_tags, (ct_model)->exclude_tags); 		\
		ct_register_signal_handlers();

///@defgroup hooks CrashC developer hooks
///@brief Represents a list of utility APIs you can use to fully customize @crashc testing execution
///@{

/**
 * Used to register a teardown function
 *
 * Use this function to perform additional operations **just before** struct ct_model model is fully disposed.
 * Usually you can use this function to release from memory any additional resources you've created.
 *
 * @param[in] f a function of type <tt>void (*f)(void)</tt> to be executed before tearing down the whole model
 * @see struct ct_model::ct_teardown
 */
void ct_set_crashc_teardown(ct_teardown_c f);

///@}

/**
 * Complete the \c main function created with ::TESTS_START
 *
 * Furthermore, it also start the execution of the registered testsuites
 */
#ifdef TESTS_END
#	error "CrashC - TESTS_END macro already defined!"
#endif
#define TESTS_END 																	\
    for (int i = 0; i < (ct_model)->suites_array_index; i++) { 						\
    	(ct_model)->tests_array[i](); 												\
    } 																				\
	(ct_model)->report_producer_implementation->report_producer(ct_model);			\
	if ((ct_model)->ct_teardown != NULL) {											\
		(ct_model)->ct_teardown();													\
	}																				\
	ct_teardown_default_model(ct_model);											\
} //main function closing bracket

/**
 * Specifies a function prototype representing a test suite.
 *
 * @testsuite can contain an arbitrary number
 * of testcases. Every ::TESTSUITE must be given a unique ID, namely either a valid C identifier
 * or a non negative integer number. Such ID uniquely represents a @testsuite.
 * Progressive numerical IDs are strongly recomended as the framework provides a few
 * really useful macros to easily register their testsuites.
 * Note that ID coherency is left as a user responsability.
 *
 * @param[in] id a sequence of id, either a valid C identifier or a non negative integer number or a valid C identifier prefixed with a non negative integer number
 */
#ifdef TESTSUITE
#	error "CrashC - TESTSUITE macro already defined!"
#endif
#define TESTSUITE(id) void suite_ ## id()

/**
 * Macro registering a single test suite given its ID
 *
 * \pre
 * 	\li test suite generated with ::TESTSUITE macro with the same \c id needs to have been declared
 * \post
 * 	\li the @testsuite with id set to \c id has been register in @crashc framework
 *
 * @param[in] id a sequence of id, either a valid C identifier or a non negative integer number or a valid C identifier prefixed with a non negative integer number
 */
#ifdef REG_SUITE
#	error "CrashC - REG_SUITE macro already defined!"
#endif
#define REG_SUITE(id) 									\
     void suite_ ## id(); 								\
     ct_update_test_array((ct_model), suite_ ## id)

/**
 * Register a batch of test suites all in one
 *
 * This macro accepts a variadic arguments. Each argument represents the \c id of a @testsuite to register.
 *
 * \pre
 * 	\li every \c id within the variadic argument needs to have a corresponding ::TESTSUITE call with the same \c id
 *
 * Note that for a single use of the macro the maximum number of specified IDs is 64
 * due to CPP limitations. This problem is easily solved by the use of two or more
 * consequent macro uses.
 *
 * @param[in] ... a sequence of id, either a valid C identifier or a non negative integer number or a valid C identifier prefixed with a non negative integer number
 */
#ifdef REG_SUITES
#	error "CrashC - REG_SUITES macro already defined!"
#endif
#define REG_SUITES(...) CT_VARIADIC_MACRO(REG_SUITE, ## __VA_ARGS__)


#endif
/**
 * @file
 *
 * Contains all the global variables inside the project
 *
 * @date Oct 11, 2017
 * @author koldar
 */

#ifndef MAIN_MODEL_H_
#define MAIN_MODEL_H_



//TODO rename the file in global_variables.h

/**
 * @defgroup globalVariables Global Variables
 * @brief a collection of all the global variables used in the project
 * @{
 */

/**
 * A structure containing the whole model of application
 *
 * This global variable represents the struct ct_model the framework use to store all its data.
 *
 */
extern struct ct_model* ct_model;

///@}

#endif /* MAIN_MODEL_H_ */
/**
 * @file
 *
 * Represents the model of @crashc.
 *
 * All the data needed by a **single instance of the framework** should be defined in the model.
 * With this design, one can potentially run several, different, instances of @crashc within the same process,
 * since they would use different struct ct_model instances.
 *
 * @author koldar
 * @date Oct 11, 2017
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <signal.h>
#include <setjmp.h>






/**
 * The maximum number of registrable suites
 *
 * This macro is used to set the initial size of the internal array used to contain the tests functions pointers
 */
#ifndef MAX_TESTS
#   define MAX_TESTS 256
#endif

/**
 * A collection of required variables used by a run of @crashc to soundly operate
 *
 * @definition @crashc model
 * It's a collection of all the needed variables a single thread of @crashc needs
 */
struct ct_model {
	/**
	 * Array containing the pointers to the testsuites functions
	 */
	ct_test_c tests_array[MAX_TESTS];
	/**
	 * The pointer to the global teardown function
	 *
	 * This is @null by default, and gets populated with a function pointer whenever the user uses the AFTER_TESTS macro.
	 *
	 * @definition
	 * The global teardown function is the last piece of code to execute before the struct ct_model model used by the framework is tore down.
	 * It's basically the last place where you can put custom code
	 */
	ct_teardown_c ct_teardown;
	/**
	 * Variablee used to keep track of the @testsuite registered
	 *
	 * Whilst registering @testsuite, the field represents the number of @testsuite you've registered so far.
	 * After registered all the @testsuite, the field represents the numbr of @testsuite registered in the run:
	 * you can use the values to understand where the field ct_model::tests_array ends to have meanings
	 *
	 * @see REG_SUITE
	 */
	int suites_array_index;
	/**
	 * The list of the test reports generated by this instance of crash c test
	 *
	 * The payloads within the list are of type ::ct_test_report.
	 * Each of them represent the outcome of the test of a run within a @testcase loop.
	 */
	ct_list_o* test_reports_list;
	/**
	 * field representing the root of the section tree
	 *
	 * @ref section_tree
	 */
	struct ct_section* root_section;
	/**
	 * Represents the @containablesection we're analyzing right now in a given time when we're running test code.
	 *
	 * Suppose you have the following code:
	 *
	 * @include exampleOfTestCase.dox
	 *
	 * which translate to the following section tree:
	 *
	 * @dotfile exampleOfTestCase.dot
	 *
	 * Suppose the program is inside "when 2": you need something that allows you to synchronize:
	 * \li where @crashc thinks the code is running;
	 * \li where the code is actually running;
	 *
	 * This means you need something that points which @containablesection you're actually in. This variable is that pointer.
	 *
	 * When you're in "when 2" this variable is set to the node in the tree representing "when 2". When you enter inside the code of "then1",
	 * this variable is reset to point "then 1" ::ct_section. Then, when you return to "when 2" to execute the code between "then 1" and "then 2", this variable
	 * is set again in a way to point "when2".
	 */
	struct ct_section* current_section;
	/**
	 * Pointer to the current section snapshot of ct_model_ct::current_section
	 *
	 * This is used by test reports to store the flow of execution followed by the tests
	 */
	struct ct_snapshot* current_snapshot;
	/**
	 * The ::TESTCASE @crashc is handling right now
	 *
	 * @crashc can handle at most 1 test case per time
	 */
	struct ct_section* jump_source_testcase;
	/**
	 * Represents the tags the user has specified as the only ones that should be consider
	 *
	 * A test is run only if it declares at least one tag inside this container.
	 * If the test is in conflict with ct_model::excludeTags , ct_model::excludeTags has the precedence.
	 *
	 * If this hashtable is empty, then we consider as if the check does't need to happen
	 *
	 * @see struct ct_model::excludeTags
	 */
	ct_tag_hashtable_o* run_only_if_tags;
	/**
	 * Represents the tags the user has specified as the ones that excludes tests
	 *
	 * A test is skipped if it declares at least one tag inside this container.
	 * If the test is in conflict with ct_model::run_only_if_tags, ct_model::excludeTags has the precedence
	 *
	 * If this hashtable is empty, then we consider as if the check does't need to happen
	 *
	 * @see struct ct_model::run_only_if_tags
	 */
	ct_tag_hashtable_o* exclude_tags;
	/**
	 * Used to store the execution state to be restored thanks to setjmp and longjmp when needed during faulty test execution.
	 *
	 * By faulty test we mean tests which generate the following scenarios:
	 * \li some signal is detected (i.e. SIGSEGV or SIGFPE);
	 * \li an assertion failed;
	 */
	jmp_buf jump_point;
	/**
	 * Represents the sigaction flag required for intercepting signals
	 *
	 * @see struct ct_model::jump_point
	 */
	struct sigaction _crashc_sigaction;
	/**
	 * contains severla  statistical informations about the tests.
	 *
	 * Example may be:
	 * 	\li the number of total tests;
	 * 	\li the number of failed tests;
	 * 	\li and so on...;
	 */
	struct ct_test_stats* statistics;
	/**
	 * The implementation of the report producer used by the model.
	 *
	 * This contains the functions used to create test reports for the user to overview
	 */
	struct ct_report_producer* report_producer_implementation;
	/**
	 * The file where to write the report on
	 */
	FILE* output_file;
};

/**
 * Create a model correctly initialized
 *
 * @return a setupped model
 */
struct ct_model* ct_setup_default_model();

/**
 * Destroy every memory allocated by the model
 *
 * @param[in] ccm the model to destroy
 */
void ct_teardown_default_model(struct ct_model* ccm);

#endif /* MODEL_H_ */
/**
 * @file
 *
 * Module for handling command line arguments
 *
 *
 * @author koldar
 * @date Oct 9, 2017
 */

#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include <stdio.h>



/**
 * Analyze the command line arguments and populates all the variables involved
 *
 * @param[in] argc \c argc from main
 * @param[in] args \c args from main
 * @param[in] tag_separator the character used to separate tags in the command line parsing (eg. -I or -E). See \ref tags for further information
 * @param[inout] run_tags hashtable from crashc model. Hashtable must be already initialized. The table will be populated at the end of the function
 * @param[inout] exclude_tags hashtable from crashc model. Hashtable must be already initialized. The table will be populated at the end of the function
 */
void ct_parse_args(const int argc, char* const* args, char tag_separator, ct_tag_hashtable_o* run_tags, ct_tag_hashtable_o* exclude_tags);

/**
 * Print the help of the command line
 *
 * @param[inout] fout the file where to write the help on
 */
void ct_print_help(FILE* fout);

#endif /* COMMAND_LINE_H_ */
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
/**
 * @file
 *
 * Contains the definitions used by @crashc to handle the test reports
 *
 * @author koldar
 * @date Feb 16, 2017
 *
 */

#ifndef TEST_REPORT_H_
#define TEST_REPORT_H_

#include <string.h>
#include <stdlib.h>






/**
 * Represents the possible outcomes of a single test:
 */
enum ct_test_outcome {
	/**
	 * A test passed
	 *
	 * @definition Passed Test
	 * It's a **test** containing no failed assertions
	 */
	CT_TEST_SUCCESS,
	/**
	 * A test didn't pass
	 *
	 * @definition Failed Test
	 * It's a **test** which is not a **passed test**.
	 */
	CT_TEST_FAILURE,
};

/**
 * Represents the actual test report.
 *
 * It contains all the informations regarding the status of an executed test.
 * Note that @crashc considers a test to be a single flow of execution of a ::CT_LOOPER (i.e. A single iteration of a @testcase)
 * E.g:
 * Let's suppose we have this @testcase:
 *
 * @code
 * TESTCASE("...", "...") {
 *     WHEN("outer when", "") {
 *         WHEN("inner when 1", "") {
 *             ...some code...
 *         }
 *         WHEN("inner when 2", "") {
 *             ...other code...
 *         }
 *     THEN("then", "") {
 *         ...then code...
 *     }
 * }
 * @endcode
 *
 * The flow of execution of this @testcase would be:
 * -# inner when 1 -> then
 * -# inner when 2 -> then
 *
 * These are what @crashc considers to be tests.
 * This implies that the informations stored in the ct_test_report struct are related to the flow of execution, not to a given section.
 * This is the reason why the ct_test_report struct needs an auxiliary struct, ::ct_snapshot, to hold the information on the status of
 * the sections involved in the test at the moment they were executed.
 */
struct ct_test_report {
	/**
	 * The name of the file that contained this test
	 *
	 * @notnull
	 */
	char* filename;
	/**
	 * The snapshot of the testcase that contained this test.
	 * This is actually the root of a tree which contains the section path followed by the test
	 *
	 *
	 * @notnull
	 */
	 struct ct_snapshot* testcase_snapshot;
	 /**
	  * The outcome of the test.
	  */
	 enum ct_test_outcome outcome;
	/**
	 * The time that it took to complete the test
	 *
	 * Note that execution times might be higher than expected due to the necessary
	 * overhead introduced by the internal code created by @crashc to properly
	 * guide the tests' execution flow
	 */
	long execution_time;
};

/**
 * create in memory a new test report
 *
 * \note
 * the test report created is assumed to have state ::CT_TEST_SUCCESS
 *
 * @param[in] tc_snapshot the snapshot tree associated to the test report
 * @return the test report desired;
 */
struct ct_test_report* ct_init_test_report(struct ct_snapshot* tc_snapshot);
/**
 * release from the memory a struct ct_test_report
 *
 * @param[inout] report the report to dispose of. @notnull
 */
void ct_destroy_test_report(struct ct_test_report* report);

//TODO why the init section snapshot is defined here while the struct is defined in section.h?
/**
 * Initialize in the heap a new section snapshot
 *
 * @param[in] section the section represented by the ::ct_snapshot you want to build
 * @return the requested ::ct_snapshot
 */
struct ct_snapshot* ct_init_section_snapshot(struct ct_section* section);
/**
 * Release from memory a section snapshot
 *
 * @param[inout] snapshot the snapshot to remove from the memory
 */
void ct_destroy_snapshot_tree(struct ct_snapshot* snapshot);

/**
 * Adds the given section to the section tree
 *
 * \note
 * It also automatically reorganize the tree's children
 *
 * @param[in] to_add snapshot to add
 * @param[inout] tree root of the snapshot tree to rearrange
 * @return \c to_add value
 */
struct ct_snapshot* ct_add_snapshot_to_tree(struct ct_snapshot* to_add, struct ct_snapshot* tree);

/**
 * Checks the status of the section associated to a given snapshot in order to change coherently the status of the snapshot.
 *
 * This is needed due to the fact
 * that every snapshot is created when entering at the beginning of the section and by default
 * is set as ::CT_SNAPSHOT_OK, so it discovers if any error occourred only at the end, by checking the section status.
 *
 * @param[in] section to status to check
 * @param[inout] snapshot whose status we need to alter
 */
void ct_update_snapshot_status(struct ct_section* section, struct ct_snapshot* snapshot);

/**
 * Checks the status of the last executed snapshot in the test tree and sets the status of the test as needed
 *
 * @param[inout] report the report to update
 * @param[in] last_snapshot the snapshot whose result we need to integrate to \c report
 */
void ct_update_test_outcome(struct ct_test_report* report, struct ct_snapshot* last_snapshot);

#endif /* TEST_REPORT_H_ */
/**
 * @file
 *
 * Represents a entity which produces a report summarizing the test outcomes for the user
 *
 * An example of a report producer is the entity which prints the outcomes of the tests in a file
 *
 * @date Oct 27, 2017
 * @author Lorenzo Nodari <lorenzo.nodari.it@ieee.org>
 */

#ifndef REPORT_PRODUCER_H_
#define REPORT_PRODUCER_H_

#include <stdio.h>







/**
 * Struct used to contain the statistics used by @crashc to give the user additional info on the run tests
 */
struct ct_test_stats {
	/**
	 * The number of total tests @crashc have detected
	 *
	 * @definition Test
	 * It's is a single loop of a @testcase
	 */
	unsigned int total_tests;
	/**
	 * The number of passed tests
	 *
	 */
	unsigned int successful_tests;
	/**
	 * The number of  failed tests
	 *
	 */
	unsigned int failed_tests;
};

/**
 * Contains function pointers to the implementations of the function used to produce the report of the particular parts of the tests.
 *
 * @definition Report Producer
 * It's an entity which, given a in-memory representation of the outcome of the tests just performed by @crashc,
 * generates an output (e.g. json, xml, sql db) which nicely summarizes the outcomes of the tests.
 *
 * This allows for easy customization and high code maintainability. Use this struct to generate your custom test reporter.
 * For example, one may want to create a test reporter which save its data ina mysqlite database.
 */
struct ct_report_producer {

	ct_test_reporter_c test_reporter;

	ct_snapshot_tree_reporter_c snapshot_tree_reporter;

	ct_summary_reporter_c summary_producer;

	ct_assert_reporter_c assert_reporter;

	ct_reporter_c report_producer;

};

/**
 * Prints the string representation of a ::ct_snapshot_status
 *
 * @param[in] status the status to compute the string representation of
 * @return a string representation of \c status
 */
char* ct_snapshot_status_to_string(enum ct_snapshot_status status);

/**
 * Prints the string representation of a ::ct_section_type
 *
 * @param[in] type the type to compute the string representation of
 * @return a string representation of \c type
 */
char* ct_section_type_to_string(enum ct_section_type type);

//TODO in my opinion these functiosn should be placed in another header called like "default_report_producer.h"

/**
 * @addtogroup defaultReportProducer Default Report Producer
 * @brief a struct ct_report_producer which print the summary of @crashc test outcomes in a file by writing the test correctly indented
 * @{
 */

/**
 * Prints the report of the executed tests in a default format
 *
 * \note
 * The report will be printed in the file specified by struct ct_model::output_file
 *
 * @param[inout] model the model to handle
 */
void ct_default_report(struct ct_model* model);
/**
 * Prints the tests summary in a default format
 *
 * \note
 * The report will be printed in the file specified by struct ct_model::output_file
 *
 * @param[inout] model the model to handle
 */
void ct_default_report_summary(struct ct_model* model);

/**
 * Prints a the report of a single test with a default format
 *
 * \note
 * The report will be printed in the file specified by struct ct_model::output_file
 *
 * @param[inout] model the model to handle
 * @param[inout] report the test report to put in the file
 */
void ct_default_test_report(struct ct_model* model, struct ct_test_report* report);

/**
 * Prints a representation of a snapshot tree in a default format
 *
 * \note
 * The report will be printed in the file specified by struct ct_model::output_file
 *
 * @param[inout] model the model to manage
 * @param[inout] snapshot the snapshot to write into the file
 * @param[in] level the depth level \c snapshot is in the snapshot tree
 */
void ct_default_snapshot_tree_report(struct ct_model* model, struct ct_snapshot* snapshot, int level);

/**
 * Prints a default report for the assertions related to a specific snapshot
 *
 * \note
 * The report will be printed in the file specified by struct ct_model::output_file
 *
 * @param[inout] model the model to manage
 * @param[inout] snapshot the snapshot to write into the file
 * @param[in] level the depth level \c snapshot is in the snapshot tree
 *
 */
void ct_default_assertions_report(struct ct_model* model, struct ct_snapshot* snapshot, int level);

///@}

/**
 * Creates a new initialized structure in the heap
 *
 * @return a structure containing the statistics data related to the tests
 */
struct ct_test_stats* ct_init_stats();

/**
 * Creates and initializes in memory a new default report producer structure
 *
 * @return a structure representing a naive report producer
 */
struct ct_report_producer* ct_init_default_report_producer();

/**
 * Frees the memory occupied by a struct ct_test_stats
 *
 * @param[inout] stats the structure to dispose from the memory
 */
void ct_destroy_stats(struct ct_test_stats* stats);

/**
 * Frees the memory allocated for the default report producer
 *
 * @param[inout] producer the structure to dispose from the mermory
 */
void ct_destroy_default_report_producer(struct ct_report_producer* producer);

#endif /* REPORT_PRODUCER_H_ */
/**
 * @file
 *
 * The module where @crashc assertions API are defined
 *
 * Asserts are the base foundation of every unit testing
 *
 * @author noodles
 * @date Nov 15, 2017
 */

#ifndef CT_ASSERT_H_
#define CT_ASSERT_H_

#include <stdbool.h>
#include <stdlib.h>




//TODO create an enum for these codes. It's better to centralized similar magic numbers into enums imho
/**
 * Magic number to pass to \c siglongjmp code when jumping back to the start of a testcase due to a failed assertion
 *
 * This macro represents the integer code passed to the siglongjmp code when jumping back to the start of
 * a testcase due to a failed assertion. This lets us distinguish when we do this due to a signal or due to
 * an assertion ad thus let us handle the two cases differently.
 */
#ifdef CT_ASSERT_JUMP_CODE
#	error "CrashC - CT_ASSERT_JUMP_CODE already defined!"
#endif
#define CT_ASSERT_JUMP_CODE 2

/**
 * This struct contains the informations related to a specific assertion
 */
struct ct_assert_report{

	/**
	 *  The outcome of the assertion. True if succeded
	 */
	bool passed;

	/**
	 * Whenever this assertion is mandatory or not
	 *
	 * Mandatory assertions will cause a test abortion on failure, non-mandatory assertions
	 * will simply register the failure and continue the test
	 */
	bool is_mandatory;

	/**
	 * String containing the text of the assertion, useful for reporting
	 */
	char* asserted;

	/**
	 * Pointer to the value expected to have from the assertion.
	 *
	 * This is a void* in order to be able to handle any type of data, even custom ones
	 */
	void* expected_value;

	/**
	 * A string containing the textual representation of the expected_value.
	 */
	char* expected_str;

	/**
	 * Pointer to the value obtained from the function to test.
	 *
	 * This is a void* in order to be able to handle any type of data, even custom ones
	 */
	void* actual_value;

	/**
	 * String representation of the actual_value assumed by the assertion
	 */
	char* actual_str;

	/**
	 * Pointer to a string containing the file name in which the assertion was
	 */
	char* file_name;

	/**
	 * The line number on which the assertion was located in the source code
	 */
	unsigned int line_number;

};

/**
 * function prototype for function comparing elements
 *
 * This type represents a comparator function, used by sophisticated versions of assertions to compare arbitraty types, like user defined structures
 *
 * @param[in] elem1 the first element to compare
 * @param[in] elem2 the second element to compare
 * @return
 * 	\li -1 when \c elem1 is "lesser" than \c elem2
 *  \li 0 when \c elem1 is equal to \c elem2
 *  \li +1 when \c elem1 is "greater" to \c elem2
 *
 * UNUSED AT THE MOMENT
 */
//typedef int (*ct_comparator_c)(void* elem1, void* elem2);

/**
 * function prototype for functions computing string representation of elements
 *
 * This type represents a stringer function, used by sophisticated versions of assertions to create strings representing the type of data on which the assertion
 * is being made, for example user-defined structures.
 * Note that the strings returned by the function are expected to be malloc'd, as he test report destructor will try to free them
 * It is possible that in future versions of the framework, the destructor will be parametrized
 *
 * @param[in] elem the elements which we want to obtain a string representation of
 *
 * UNUSED AT THE MOMENT
 */
//TODO I think the best prototype for this function pointer would be int(void* element, int buffer size, char* buffer) where the return value is the number of byte you used in the buffer.
//typedef void (*ct_stringer_c)(void* elem);

/**
 * Represents the function pointers used as callbacks to handle the behaviour of an assertion implementation.
 * The 3 parameters are:
 * @param[in] model the @crashc model being used
 */
/*
 * - type_size: the size, in bytes, of the type on which the assertion is being made. This is needed as once the original pointer is cast to void *, its real type cannot be
 * 				recovered anymore, so we need this to do some boundaries check
 * - asserted: pointer to the asserted variable/value
 * - stringer: the stringer to be used to convert raw bytes into a string
 */
typedef void (*ct_assert_c)(struct ct_model* model);


/**
 * The general CrashC assertion macro
 *
 * This macro is not used directly, but it is masked by other macros which actually implement
 * a specific assertion type.
 *
 * @param[in] model a pointer to struct ct_model used
 * @param[in] is_mandatory @true if the assertion needs to be surpassed; @false if the assertion is actually optional
 * @param[in] asserted C code representing the whole content of the assertion. This is likely to be something like <tt>someStuff == someOtherStuff</tt>.
 * 	The code is **required** to be a boolean expression.
 * @param[in] passed_callback function of type ct_assert_c called if the assertion passes;
 * @param[in] failed_callback function of type ct_assert_c called if the assertion doesn't pass;
 */
#ifdef CT_ASSERTION
#	error "CrashC - CT_ASSERTION already defined!"
#endif
#define CT_ASSERTION(model, is_mandatory, asserted, passed_callback, failed_callback)														\
	ct_list_add_tail((model)->current_snapshot->assertion_reports, ct_init_assert_report(is_mandatory, #asserted, __FILE__, __LINE__));		\
	if ((asserted) != true) {																												\
		failed_callback((model));																											\
	}																																		\
	else {																																	\
		passed_callback((model));																											\
	}																																		\

//TODO complete the documentation here
/**
 * This macro is the basic form of a CrashC assertion.
 *
 * @param[in] assertion a C code reprsenting the assertion to express. The code is **required** to be a boolean expression.
 */
#ifdef ASSERT
#	error "CrashC - ASSERT macro already defined!"
#endif
#define ASSERT(assertion) CT_ASSERTION(ct_model, true, assertion, ct_assert_do_nothing, ct_general_assert_failed)

/**
 * Initializes an assertion report and returns a pointer to it.
 *
 * \note
 * \c expected_value, \c actual_value, \c expected_stringed and \c actual_stringed all default to @null: their initialization is demanded to the caller in a later moment
 *
 * @param[in] is_mandatory @true if the assertion needs to be surpassed; @false if the assertion is actually optional
 * @param[in] asserted_text a string representing the actual C code of assertion
 * @param[in] file a string representing the file where the assertion is positioned
 * @param[in] line the line number where the assertion is located in the file \c file
 * @return a structure representing the performance of a given assertion
 */
struct ct_assert_report* ct_init_assert_report(bool is_mandatory, char* asserted_text, char* file, unsigned int line);

/**
 * Frees the memory allocated for a particular assertion report
 *
 * @param[in] report the report to release from the memory
 */
void ct_destroy_assert_report(struct ct_assert_report* report);

/**
 * @defgroup assertCallbacks Assert Outcome Callbacks
 * @brief represents a group of callbacks designed to be passed to ::CT_ASSERTION macro
 * @{
 */

/**
 * Function used as a callback when an assertion doesn't need to do anything
 *
 * @param[in] model the model to handle
 */
void ct_assert_do_nothing(struct ct_model* model);

/**
 * Function used by the general ASSERT macro to handle its failure
 *
 * @param[in] model the model to handle
 */
void ct_general_assert_failed(struct ct_model* model);

///@}

#endif /* CT_ASSERT_H_ */
/*
 * ct_assert.c
 *
 *  Created on: Nov 15, 2017
 *      Author: noodles
 */

#include <string.h>
#include <setjmp.h>






struct ct_assert_report* ct_init_assert_report(bool is_mandatory, char* asserted_text, char* file, unsigned int line) {
	struct ct_assert_report* ret_val = malloc(sizeof(struct ct_assert_report));

	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->passed = true;
	ret_val->is_mandatory = is_mandatory;
	ret_val->asserted = asserted_text;
	ret_val->expected_value = NULL;
	ret_val->expected_str = NULL;
	ret_val->actual_value = NULL;
	ret_val->actual_str = NULL;
	ret_val->file_name = file;
	ret_val->line_number = line;

	return ret_val;
}

//TODO: We might add a field to the assert_report struct to point at a specific destructor in order to be able to
//		precisely control how to destroy a specific report, which might have been generated by a different type of assertion
//		which, for example, needs to malloc the memory for its strings
void ct_destroy_assert_report(struct ct_assert_report* report) {
	free(report);
}

void ct_assert_do_nothing(struct ct_model* model) {

}

void ct_general_assert_failed(struct ct_model* model) {

	struct ct_snapshot* snapshot = model->current_snapshot;
	struct ct_assert_report* report = ct_list_tail(snapshot->assertion_reports);
	struct ct_test_report* test_report = ct_list_tail(model->test_reports_list);

	//Update the assertion report
	report->passed = false;
	report->expected_str = "true";
	report->actual_str = "false";

	//Update the status of the snapshot which contained this assertion and of the test
	snapshot->status = CT_SNAPSHOT_FAILED;
	ct_update_test_outcome(test_report, snapshot);

	//We then need to reset the current snapshot to start a new snapshot tree
	model->current_snapshot = NULL;

	//We jump out of this flow of execution to go back at the beginning of the test case and go on with the execution of the other tests
	siglongjmp(model->jump_point, CT_ASSERT_JUMP_CODE);
}


/**
 * @file
 *
 * @author koldar
 * @date Oct 9, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>





static struct option long_options[] = {
	{"include_tag",		required_argument,	0,	'i'},
	{"include_tags",	required_argument,	0,	'I'},
	{"exclude_tag",		required_argument,	0,	'e'},
	{"exclude_tags",	required_argument,	0,	'E'},
	{"help",			no_argument,		0,	'h'},
	{0,					0,					0,	0}
};

void ct_print_help(FILE* fout) {
	int i = 0;
	struct option o;

	while (true) {
		o = long_options[i];
		if (o.name == NULL) {
			return;
		}

		fprintf(fout, "-%c, --%s: ", o.val, o.name);

		switch (o.val) {
		case 'h': {
			fprintf(fout, "Show this help");
			break;
		}
		case 'i': {
			fprintf(fout,
					"Adds a tagname in the \"Run Only\" set. "
					"A section is run only if it contains at least a tagname within such set. "
					"Can be defined multiple times.  If a tag is inside both 'e' and 'i', 'e' has the precedence."
			);
			break;
		}
		case 'e': {
			fprintf(fout,
					"Adds a tagname in the \"Exclude Test\" set. "
					"A section isn't run if it contains even one tagname within such set. "
					"Can be defined multiple times. If a tag is inside both 'e' and 'i', this set has the precedence."
			);
			break;
		}
		case 'I': {
			fprintf(fout,
					"Like \"i\", but allows to specify multiple tags. Tags are separated via \"%c\". "
					"Can be defined multiple times.",
					CT_TAGS_SEPARATOR
			);
			break;
		}
		case 'E': {
			fprintf(fout,
					"Like \"e\", but allows to specify multiple tags. Tags are separated via \"%c\". "
					"Can be defined multiple times.",
					CT_TAGS_SEPARATOR
			);
			break;
		}
		}

		fprintf(fout, "\n");

		i++;
	}
}

void ct_parse_args(const int argc, char* const* args, char tag_separator, ct_tag_hashtable_o* run_tags, ct_tag_hashtable_o* exclude_tags) {

	while (true) {
		/* getopt_long stores the option index here. */
		int option_index = 0;

		int optionId = getopt_long (argc, args, "i:I:e:E:", long_options, &option_index);

		/* Detect the end of the options. */
		if (optionId == -1)
			break;

		switch (optionId) {
		case 0: {
			//Only flags have the id set to 0.
			break;
		}
		case 'h': {
			ct_print_help(stdout);
			//TODO create an exit macro like CORRECT_EXIT_MACRO
			exit(0);
			break;
		}
		case 'i': {
			ct_tag_ht_put(run_tags, optarg);
			break;
		}
		case 'e': {
			ct_tag_ht_put(exclude_tags, optarg);
			break;
		}
		case 'I': {
			ct_tag_ht_populate(run_tags, optarg, tag_separator);
			break;
		}
		case 'E': {
			ct_tag_ht_populate(exclude_tags, optarg, tag_separator);
			break;
		}
		case '?': {
			/* getopt_long already printed an error message. */
			break;
		}
		default: {
			abort();
		}
		}
	}

//  ACTIVATE IF YOU WANT TO SEE WHAT TAGS HAVE BEEN STORED
//	CT_ITERATE_VALUES_ON_HT(runIfTags, t, struct ct_tag*) {
//		printf("run if tag: %s\n", t->name);
//	}
//	CT_ITERATE_VALUES_ON_HT(excludeTags, t2, struct ct_tag*) {
//		printf("exclude tag: %s\n", t2->name);
//	}

}





void ct_update_test_array(struct ct_model* model, ct_test_c func) {
	model->tests_array[model->suites_array_index] = func;
	model->suites_array_index++;
}

bool ct_run_once_check_access(struct ct_model* model, struct ct_section* section, ct_access_c cs, ct_enter_c callback, const ct_tag_hashtable_o* restrict run_tags, const ct_tag_hashtable_o* restrict exclude_tags) {
	if (!section->loop2) {
		return false;
	}

	//TODO here we need  to replace the parameter runOnlyWithTags and excludeIfTags with a pointer of the global model
	//check if the section we're dealing with is compliant with the context tags
	if (!ct_ht_is_empty(exclude_tags)) {
		if (ct_have_tag_set_intersection(section->tags, exclude_tags)) {
			section->tag_access_granted = false;
			section->access_granted = false;
			ct_section_set_skipped(section);
			return false;
		}
	}

	if (!ct_ht_is_empty(run_tags)) {
		if (!ct_have_tag_set_intersection(section->tags, run_tags)) {
			section->tag_access_granted = false;
			section->access_granted = false;
			ct_section_set_skipped(section);
			return false;
		}
	}
	section->tag_access_granted = true;

	section->access_granted = cs(model, section);
	if (section->access_granted) {
		callback(model, section);
	}
	return section->access_granted;
}

bool ct_run_once_final_work(struct ct_model* model, struct ct_section* section, struct ct_section** pointer_to_set_as_parent, ct_exit_c callback, ct_exit_c access_granted_callback, ct_exit_c access_denied_callback) {
	if (section->loop1) {
		return true;
	}
	//callback is always executed and it can (and often will) change pointerToSetAsParent and child pointers (since they point to the same structure).
	//in order to use the previously pointed structure we copy the child pointer. As for pointerToSetAsParent, we can do nothing since it will be changed nonetheless
	struct ct_section* _child = section;
	callback(model, pointer_to_set_as_parent, section);
	if (section->access_granted) {
		//ct_section_set_executed(section);
		//If we executed the section we check if this execution made the section
		//fully visited and update its status consequently
		if (ct_section_is_fully_visited(section)) {
			ct_section_set_done(section);
		}

		//We reset the WHEN found tag
		section->already_found_when = false;

		access_granted_callback(model, pointer_to_set_as_parent, _child);
	} else {
		access_denied_callback(model, pointer_to_set_as_parent, _child);
	}
	return false;
}

/** If we are still computing the children of the given parent section the function
 *  creates a new section with the given description, tags and adds it to its
 *  parent section and returns a pointer to the new section.
 *  Else, the function returns a pointer to the current_child of the given section.
 *
 *  @param[in] type the kind of section we're getting.
 *  @see ::ct_section_type
 */
struct ct_section* ct_fetch_section(struct ct_section* parent, enum ct_section_type type, const char* description, const char* tags) {
	if (ct_section_still_discovering_children(parent)) {
		parent->children_number += 1;
		return ct_section_add_child(ct_section_init(type, description, tags), parent);
	}
	return ct_section_get_child(parent, parent->current_child);
}

void ct_reset_section_after_jump(struct ct_model* model, struct ct_section* const jump_source_section, struct ct_section* const testcase_section) {
	model->current_section = testcase_section;
}

bool ct_always_enter(struct ct_model* model, struct ct_section* section) {
	return true;
}

void ct_exit_callback_next_sibling(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* section) {
	//we finish a section. we return to the parent
	*pointer_to_set_as_parent = section->parent;
	//we go to the next sibling of child
	(*pointer_to_set_as_parent)->current_child += 1;
}

void ct_exit_callback_children_number_computed(struct ct_model* model, struct ct_section** pointerToSetAsParent, struct ct_section* section) {
	//we executed the section. Hence we can safely say we know the child number of such section
	if (!section->children_number_known) {
		section->children_number_known = true;
	}
	section->current_child = 0;

	ct_update_snapshot_status(model->current_section, model->current_snapshot);

	model->current_snapshot = model->current_snapshot->parent;
}

void ct_exit_callback_reset_container(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* child) {
	//we finished a section. Hence now we know the number of children that section have
	child->children_number_known = true;
	child->current_child = 0;
}

void ct_exit_callback_do_nothing(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* section) {

}

bool ct_get_access_when(struct ct_model* model, struct ct_section* section) {
	//section is the WHEN we're considering right now

	//we don't enter in this WHEN if we have already entered in another WHEN of the parent
	if (section->parent->already_found_when) {
		return false;
	}

	//we don't access this WHEN if we have already completed it or if it generated a SECTION_SIGNAL
	if (section->status == CT_SECTION_FULLY_VISITED || section->status == CT_SECTION_SIGNAL_DETECTED) {
		return false;
	}

	return true;
}

void ct_callback_do_nothing(struct ct_model* model, struct ct_section* section) {

}

void ct_callback_entering_testcase(struct ct_model* model, struct ct_section* section) {
	ct_update_current_snapshot(model, model->current_section);
	struct ct_test_report* report = ct_init_test_report(model->current_snapshot);
	ct_list_add_tail(model->test_reports_list, report);
}

void ct_callback_entering_then(struct ct_model* model, struct ct_section* section) {
	ct_update_current_snapshot(model, section);
}

void ct_callback_entering_when(struct ct_model* model, struct ct_section* section) {
	section->parent->already_found_when = true;

	ct_update_current_snapshot(model, section);
}

void ct_exit_callback_access_granted_testcase(struct ct_model* model, struct ct_section** pointer_to_set_as_parent, struct ct_section* section) {
	struct ct_test_report* report = ct_list_tail(model->test_reports_list);
	struct ct_snapshot* last_snapshot = model->current_snapshot;

	ct_update_snapshot_status(section, model->current_snapshot);
	ct_update_test_outcome(report, last_snapshot);

	//Resets the current_snapshot pointer to NULL to indicate the end of the test
	model->current_snapshot = NULL;
}

/**
 * This functions updates the current_snapshot. More precisely:
 *  - if current_snapshot is NULL it means we just started a new test, so it simply takes a section snapshot and sets it
 *  	as the current_snapshot
 *  - if current_snapshot is not NULL it means we are not in a new test, so we take a snapshot and adds it to the test report tree
 */

void ct_update_current_snapshot(struct ct_model* model, struct ct_section* section) {
	struct ct_snapshot* snapshot = ct_init_section_snapshot(section);

	if (model->current_snapshot == NULL) {
		model->current_snapshot = snapshot;
	}
	else {
		model->current_snapshot = ct_add_snapshot_to_tree(snapshot, model->current_snapshot);
	}

}

void ct_signal_callback_do_nothing(int signal, struct ct_section* signaled_section, struct ct_section* section, struct ct_section* target_section) {

}

void ct_set_crashc_teardown(ct_teardown_c f) {
	ct_model->ct_teardown = f;
}
/*
 * hashtable.c
 *
 *  Created on: Jan 24, 2017
 *      Author: koldar
 */




// File-local functions declarations
static ct_hashtable_entry_o* ht_init_entry(const void* data, unsigned long key);
static void ht_destroy_entry(const ct_hashtable_entry_o* entry);

struct ct_hashtable_entry {
	/**
	 * the key of a particular value
	 */
	unsigned long id;
	/**
	 * the value in the hashtable
	 */
	void* data;
	/**
	 * A field required by \c uthash project to correctly work
	 */
	UT_hash_handle hh;

};

struct ct_hashtable {
	/**
	 * **head** of the hashtable
	 *
	 * \c uthash hashtables are retrieved by a particular ::ct_hashtable_entry_o called **head**. This field is exactly that
	 */
	ct_hashtable_entry_o* head;
};

ct_hashtable_o* ct_ht_init() {
	ct_hashtable_o* ret_val = malloc(sizeof(ct_hashtable_o));
	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->head = NULL;

	return ret_val;
}

int ct_ht_size(const ct_hashtable_o* ht) {
	int ret_val;
	ret_val = HASH_COUNT(ht->head);
	return ret_val;
}

void* ct_ht_get(const ct_hashtable_o* ht, unsigned long key) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return NULL;
	}
	return tmp->data;
}

bool ct_ht_contains(const ct_hashtable_o* ht, unsigned long key) {
	ct_hashtable_entry_o* tmp;
	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	return (tmp != NULL);
}

bool ct_ht_put_or_update(ct_hashtable_o* ht, unsigned long key, const void* data) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		ct_ht_put(ht, key, data);
		return true;
	} else {
		tmp->data = (void*) data;
		return false;
	}
}

bool ct_ht_update(ct_hashtable_o* ht, unsigned long key, const void* data) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	tmp->data = (void*) data;
	return true;
}

void ct_ht_put(ct_hashtable_o* ht, unsigned long key, const void* data) {
	ct_hashtable_entry_o* add = ht_init_entry(data, key);
	HASH_ADD(hh,ht->head,id, sizeof(unsigned long), add);
}

void ct_ht_destroy(ct_hashtable_o* ht) {
	ct_hashtable_entry_o* s;
	ct_hashtable_entry_o* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		ht_destroy_entry(s);
	}
}

void ct_ht_destroy_with_elements(ct_hashtable_o* ht, ct_destroyer_c d) {
	ct_hashtable_entry_o* s;
	ct_hashtable_entry_o* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		d(s->data);
		ht_destroy_entry(s);
	}
}

bool ct_ht_remove(ct_hashtable_o* ht, unsigned long key) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	HASH_DEL(ht->head, tmp);
	ht_destroy_entry(tmp);
	return true;
}

bool ct_ht_remove_and_destroy(ct_hashtable_o* ht, unsigned long key, ct_destroyer_c d) {
	ct_hashtable_entry_o* tmp;

	HASH_FIND(hh, ht->head, &key, sizeof(unsigned long), tmp);
	if (tmp == NULL) {
		return false;
	}
	HASH_DEL(ht->head, tmp);
	d(tmp->data);
	ht_destroy_entry(tmp);
	return true;
}

bool ct_ht_is_empty(const ct_hashtable_o* ht) {
	return ht->head == NULL;
}

void* ct_ht_get_first(const ct_hashtable_o* ht) {
	CT_ITERATE_VALUES_ON_HT(ht, data, void*) {
		return data;
	}
	return NULL;
}

bool ct_ht_swap(ct_hashtable_o* ht, unsigned long key1, unsigned long key2) {
	ct_hashtable_entry_o* tmp1;
	ct_hashtable_entry_o* tmp2;

	HASH_FIND(hh, ht->head, &key1, sizeof(unsigned long), tmp1);
	HASH_FIND(hh, ht->head, &key2, sizeof(unsigned long), tmp2);

	if (tmp1 != NULL && tmp2 != NULL) {
		//they are both inside the hashtable
		void* p1 = tmp1->data;
		void* p2 = tmp2->data;
		HASH_DEL(ht->head, tmp1);
		HASH_DEL(ht->head, tmp2);

		ct_ht_put(ht, key1, p2);
		ct_ht_put(ht, key2, p1);

		return true;

	} else if (tmp1 != NULL) {
		//key2 is not inside the hashtable
		ct_ht_put(ht, key2, tmp1->data);
		HASH_DEL(ht->head, tmp1);

		return true;

	} else if (tmp2 != NULL) {
		//key1 is not inside the hashtable
		ct_ht_put(ht, key1, tmp2->data);
		HASH_DEL(ht->head, tmp2);

		return true;
	}

	return false;
}

void ct_ht_clear(ct_hashtable_o* ht) {
	ct_hashtable_entry_o* s;
		ct_hashtable_entry_o* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		ht_destroy_entry(s);
	}
	ht->head = NULL;
}

void ct_ht_clear_and_destroy_elements(ct_hashtable_o* ht, ct_destroyer_c d) {
	ct_hashtable_entry_o* s;
	ct_hashtable_entry_o* tmp;

	HASH_ITER(hh, ht->head, s, tmp) {
		HASH_DEL(ht->head, s);
		d(s->data);
		ht_destroy_entry(s);
	}
	ht->head = NULL;
}

ct_hashtable_entry_o* _ct_ht_head_entry(ct_hashtable_o* ht) {
	return ht->head;
}

void* _ct_ht_entry_next(ct_hashtable_entry_o* entry) {
	return entry->hh.next;
}

void* _ct_ht_entry_payload(ct_hashtable_entry_o* entry) {
	return entry->data;
}

/**
 *
 * \note
 * The data is added in the cell <b>by reference</b>
 *
 * @param[in] data the data to put inside the cell
 * @param[in] f a function with one parameter <tt>void*</tt> returning the hash of the value, aka an int
 * @return a cell of an hash table. You still need to manually add it in the hash table though
 */
static ct_hashtable_entry_o* ht_init_entry(const void* data, unsigned long key) {
	ct_hashtable_entry_o* ret_val = malloc(sizeof(ct_hashtable_entry_o));
	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->id = key;
	ret_val->data = (void*) data;

	return ret_val;
}

/**
 * Frees from the memory the structure \c ct_hashtable_entry_o
 *
 * \attention
 * the function <b>won't free the pointer</b> ::ct_hashtable_entry_o::data
 *
 * @param[in] entry the cell to remove from the memory
 */
static void ht_destroy_entry(const ct_hashtable_entry_o* entry) {
	free((void*) entry);
}
/*
 * list.c
 *
 *  Created on: May 17, 2017
 *      Author: koldar
 */

#include <stdio.h>
#include <string.h>






struct ct_list_entry {
	///represents the payload inside this cell of the list
	void* payload;
	///a pointer to the next element of the list. Can be NULL
	struct ct_list_entry* next;
};

struct ct_list {
	///number of elements inside the list. Allows count operation to be O(1)
	int size;
	///pointer to the first element of the list. Can be NULL
	ct_list_entry_o* head;
	///pointer to the last element of the list. Can be NULL
	ct_list_entry_o* tail;
};

ct_list_entry_o* _ct_list_get_next_entry(const ct_list_entry_o* entry) {
	return entry->next;
}

void* _ct_list_get_entry_payload(const ct_list_entry_o* entry) {
	return entry->payload;
}

ct_list_entry_o* _ct_list_head_entry(const ct_list_o* l) {
	return l->head;
}

ct_list_o* ct_list_init() {
	ct_list_o* ret_val = malloc(sizeof(ct_list_o));
	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->head = NULL;
	ret_val->size = 0;
	ret_val->tail = NULL;

	return ret_val;
}

void ct_list_destroy(const ct_list_o* lst) {
	CT_ITERATE_ON_LIST(lst, entry, value, void*) {
		free(entry);
	}
	free((void*) lst);
}

void ct_list_destroy_with_elements(const ct_list_o* lst, ct_destroyer_c d) {
	CT_ITERATE_ON_LIST(lst, entry, value, void*) {
		d(entry->payload);
		free(entry);
	}
	free((void*) lst);
}

void ct_list_clear(ct_list_o* l) {
	CT_ITERATE_ON_LIST(l, entry, value, void*) {
		free(entry);
	}
	l->head = NULL;
	l->size = 0;
	l->tail = NULL;
}

void ct_list_add_head(ct_list_o* l, const void* el) {
	ct_list_entry_o* new_entry = malloc(sizeof(ct_list_entry_o));
	if (new_entry == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	new_entry->payload = (void*)el;
	new_entry->next = l->head;

	l->size++;
	l->head = new_entry;
	if (l->tail == NULL) {
		l->tail = new_entry;
	}
}

void ct_list_add_tail(ct_list_o* l, const void* el) {
	ct_list_entry_o* new_entry = malloc(sizeof(ct_list_entry_o));
	if (new_entry == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	new_entry->payload = (void*)el;
	new_entry->next = NULL;
	if (l->tail != NULL) {
		l->tail->next = new_entry;
	}

	l->size++;
	if (l->head == NULL) {
		l->head = new_entry;
	}
	l->tail = new_entry;

}

int ct_list_size(const ct_list_o* l) {
	return l->size;
}

void ct_list_full_transfer(ct_list_o* restrict dst, ct_list_o* restrict src) {
	//*********** DST **********
	dst->size += ct_list_size(src);
	if (dst->head == NULL) {
		dst->head = src->head;
	} else {
		dst->tail->next = src->head;
	}
	dst->tail = src->tail;

	//************ SRC **************
	src->size = 0;
	src->head = NULL;
	src->tail = NULL;
}

bool ct_list_is_empty(const ct_list_o* l) {
	return l->size == 0;
}

void* ct_list_pop(ct_list_o* l) {
	if (ct_list_is_empty(l)) {
		return NULL;
	}

	ct_list_entry_o* cell = l->head;
	void* ret_val = cell->payload;
	l->head = l->head->next;
	l->size--;
	if (ct_list_is_empty(l)) {
		l->tail = NULL;
	}

	free(cell);
	return ret_val;
}

void* ct_list_head(const ct_list_o* l) {
	if (ct_list_is_empty(l)) {
		return NULL;
	}
	return l->head->payload;
}

void* ct_list_tail(const ct_list_o* l) {
	if (ct_list_is_empty(l)) {
		return NULL;
	}
	return l->tail->payload;
}

void* ct_list_get(const ct_list_o* l, int index) {
	CT_ITERATE_ON_LIST(l, entry, payload, void*) {
		if (index == 0) {
			return payload;
		}
		index--;
	}
	return NULL;
}

void ct_list_remove_entry_dynamic(ct_list_o* l, ct_list_entry_o** previous_entry, ct_list_entry_o* entry_to_remove) {
	ct_list_o* lst = l;
	ct_list_entry_o* previous = *previous_entry;

	if (previous == NULL) {
		//we're removing the head
		ct_list_pop(l);
	} else if (entry_to_remove->next == NULL){
		//we're removing the tail
		previous->next = NULL;
		lst->size--;
		lst->tail = previous;
		free(entry_to_remove);
	} else {
		//we're removing an element inside the list
		previous->next = entry_to_remove->next;
		lst->size--;
		free(entry_to_remove);
	}

	*previous_entry = NULL;
}
/*
 * main_model.c
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */




struct ct_model* ct_model;
/*
 * model.c
 *
 *  Created on: Oct 11, 2017
 *      Author: koldar
 */

#include <stdlib.h>






struct ct_model* ct_setup_default_model() {
	struct ct_model* ret_val = malloc(sizeof(struct ct_model));

	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->current_section = NULL;
	ret_val->current_snapshot = NULL;
	ret_val->ct_teardown = NULL;
	ret_val->test_reports_list = ct_list_init();
	ret_val->jump_source_testcase = NULL;
	ret_val->suites_array_index = 0;
	ret_val->run_only_if_tags = ct_ht_init();
	ret_val->exclude_tags = ct_ht_init();
	ret_val->_crashc_sigaction = (struct sigaction) { 0 };
	ret_val->root_section = ct_section_init(CT_ROOT_SECTION, "root", "");
	ret_val->statistics = ct_init_stats();
	ret_val->report_producer_implementation = ct_init_default_report_producer();
	ret_val->output_file = stdout;

	return ret_val;
}

void ct_teardown_default_model(struct ct_model* ccm) {
	ct_section_destroy(ccm->root_section);
	ct_ht_destroy_with_elements(ccm->exclude_tags, (ct_destroyer_c)ct_tag_destroy);
	ct_ht_destroy_with_elements(ccm->run_only_if_tags, (ct_destroyer_c)ct_tag_destroy);
	ct_list_destroy_with_elements(ccm->test_reports_list, (ct_destroyer_c)ct_destroy_test_report);
	ct_destroy_stats(ccm->statistics);
	ct_destroy_default_report_producer(ccm->report_producer_implementation);
	fclose(ccm->output_file);
	free(ccm);
}
/*
 * report_producer.c
 *
 *  Created on: Oct 27, 2017
 *      Author: Lorenzo Nodari <lorenzo.nodari.it@ieee.org>
 */

#include <stdlib.h>





char* ct_snapshot_status_to_string(enum ct_snapshot_status s) {

	switch (s) {
		case CT_SNAPSHOT_OK: return "OK";
		case CT_SNAPSHOT_SIGNALED: return "SIGNALED";
		case CT_SNAPSHOT_FAILED: return "FAILED";
		default: 	printf("\nERROR: Unrecognized snapshot status, exiting.\n");
					exit(1); //TODO: Fix error exit
	}

}

char* ct_section_type_to_string(enum ct_section_type t) {

	switch (t) {
		case CT_WHEN_SECTION: return "WHEN";
		case CT_THEN_SECTION: return "THEN";
		case CT_TESTCASE_SECTION: return "TESTCASE";
		case CT_ROOT_SECTION: return "ROOT";
		case CT_TESTSUITE_SECTION: return "SUITE";
		default: 	printf("\nERROR: Unrecognized section type, exiting.\n");
					exit(1); //TODO: Fix error exit
	}

}


//TODO: Check for errors due to FILE null pointer etc...
void ct_default_snapshot_tree_report(struct ct_model* model, struct ct_snapshot* snapshot, int level) {

	FILE* file = model->output_file;

	char* type_str = ct_section_type_to_string(snapshot->type);
	char* status_str = ct_snapshot_status_to_string(snapshot->status);
	fprintf(file, "%s : %s -> %s\n", type_str, snapshot->description, status_str);
	ct_default_assertions_report(model, snapshot, level);

	struct ct_snapshot* child = snapshot->first_child;
	while (child != NULL) {
		for (int i = 0; i < level; i++) {
			putchar('\t');
		}
		ct_default_snapshot_tree_report(model, child, level + 1);
		child = child->next_sibling;
	}

}

void ct_default_test_report(struct ct_model* model, struct ct_test_report* report) {

	FILE* file = model->output_file;

	fprintf(file, " ---------- TEST REPORT ----------\n\n");
	//fprintf(file, "File: %s\n\n", report->filename);
	ct_default_snapshot_tree_report(model, report->testcase_snapshot, 1);
	fprintf(file, "\nOutcome: %s\n", (report->outcome == CT_TEST_SUCCESS) ? "SUCCESS" : "FAILURE");
	fprintf(file, "\n --------------------------------\n");
	fprintf(file, "\n\n");

}

void ct_default_report_summary(struct ct_model* model) {

	FILE* file = model->output_file;
	struct ct_test_stats * stats = model->statistics;

	fprintf(file, "Total tests: %d\n", stats->total_tests);
	fprintf(file, "Successful tests: %d\n", stats->successful_tests);
	fprintf(file, "Failed tests: %d\n", stats->failed_tests);
	fprintf(file, "Percentage of successful tests: %.2f%%\n", ((double) stats->successful_tests / stats->total_tests) * 100);

}

void ct_default_assertions_report(struct ct_model* model, struct ct_snapshot* snapshot, int level) {

	FILE* file = model->output_file;
	ct_list_o* assertion_reports = snapshot->assertion_reports;

	CT_ITERATE_ON_LIST(assertion_reports, report_cell, report, struct ct_assert_report*) {
		for (int i = 0; i < level; i++) {
			putchar('\t');
		}

		if (report->passed) {
			fprintf(file, "Assertion \"%s\" - OK\n", report->asserted);
		}
		else {
			fprintf(file, "Assertion \"%s\" - FAILED - Expected: %s, Actual: %s\n", report->asserted, report->expected_str, report->actual_str);
		}
	}

}

void ct_default_report(struct ct_model* model) {

	ct_list_o* report_list = model->test_reports_list;

	//TODO: Move the stats-acquiring code away from the report producer
	CT_ITERATE_ON_LIST(report_list, report_cell, report, struct ct_test_report*) {
		if (report->outcome == CT_TEST_SUCCESS) {
			model->statistics->successful_tests++;
		}
		else {
			model->statistics->failed_tests++;
		}
		ct_default_test_report(model, report);
	}

	model->statistics->total_tests = ct_list_size(report_list);
	ct_default_report_summary(model);

}

struct ct_test_stats* ct_init_stats() {

	struct ct_test_stats* ret_val = malloc(sizeof(struct ct_test_stats));

	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->total_tests = 0;
	ret_val->successful_tests = 0;
	ret_val->failed_tests = 0;

	return ret_val;
}

struct ct_report_producer* ct_init_default_report_producer() {

	struct ct_report_producer* ret_val = malloc(sizeof(struct ct_report_producer));

	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->test_reporter = ct_default_test_report;
	ret_val->snapshot_tree_reporter = ct_default_snapshot_tree_report;
	ret_val->summary_producer = ct_default_report_summary;
	ret_val->assert_reporter = ct_default_assertions_report;
	ret_val->report_producer = ct_default_report;

	return ret_val;

}

void ct_destroy_stats(struct ct_test_stats* stats) {

	free(stats);

}

void ct_destroy_default_report_producer(struct ct_report_producer* producer) {

	free(producer);

}


/*
 * section.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */

#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>




/**
 * The id the next section created with ::ct_section_init will have.
 *
 * It starts from 1 and not from 0 because 0 is **manually** set to the root section, created **statically**.
 * See ::rootSection
 */
static int next_section_id = 1;

static int populate_section_status_buffer_string(const int ss, int space_left, char* buffer);
static void compute_section_tree_dot_file(FILE* fout, const struct ct_section* section);
static void update_section_tree_dot_file_with_section_info(FILE* fout, const struct ct_section* section);
static void update_section_tree_dot_file_with_section_edges(FILE* fout, const struct ct_section* section);

struct ct_section* ct_section_add_child(struct ct_section* restrict to_add, struct ct_section* restrict parent) {
	struct ct_section* list = NULL;

	to_add->parent = parent;

	list = parent->first_child;
	if (list == NULL) {
		parent->first_child = to_add;
		return to_add;
	}
	while (true) {
		if (list->next_sibling != NULL) {
			list = list->next_sibling;
		} else {
			list->next_sibling = to_add;
			return to_add;
		}
	}
}

struct ct_section* ct_section_get_child(const struct ct_section* parent, int n) {
	struct ct_section* list = parent->first_child;
	while(true) {
		if ((n == 0) || (list == NULL)) {
			return list;
		} else {
			list = list->next_sibling;
			n--;
		}
	}
}

struct ct_section* ct_section_init(enum ct_section_type type, const char* description, const char* tags) {
	struct ct_section* ret_val = malloc(sizeof(struct ct_section));
	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->id = next_section_id;
	next_section_id += 1;
	ret_val->access_granted = false;
	ret_val->already_found_when = false;
	ret_val->children_number = 0;
	ret_val->children_number_known = false;
	ret_val->status = CT_SECTION_UNVISITED;
	ret_val->current_child = 0;
	ret_val->description = strdup(description);
	ret_val->first_child = NULL;
	ret_val->type = type;
	ret_val->times_encountered = 0;
	ret_val->loop1 = false;
	ret_val->loop2 = false;
	ret_val->next_sibling = NULL;
	ret_val->parent = NULL;
	ret_val->tags = ct_ht_init();

	ct_tag_ht_populate(ret_val->tags, tags, CT_TAGS_SEPARATOR);

	return ret_val;
}

void ct_section_destroy(struct ct_section* section) {
	if (section->first_child != NULL) {
		ct_section_destroy(section->first_child);
	}
	if (section->next_sibling != NULL) {
		ct_section_destroy(section->next_sibling);
	}
	if (section->parent == NULL) {
		//root section shouldn't be removed at all
		return;
	}

	ct_ht_destroy_with_elements(section->tags, (ct_destroyer_c)(ct_tag_destroy));
	free((void*) section->description);
	free((void*) section);
}

bool ct_section_still_discovering_children(const struct ct_section* section) {
	return !section->children_number_known;
}

void ct_section_set_signaled(struct ct_section* section) {
	section->status = CT_SECTION_SIGNAL_DETECTED;
}

bool ct_section_is_signaled(const struct ct_section* section) {
	return section->status == CT_SECTION_SIGNAL_DETECTED;
}

void ct_section_set_executed(struct ct_section* section) {
	section->status = CT_SECTION_PARTIALLY_VISITED;
}

void ct_section_set_done(struct ct_section* section) {
	section->status = CT_SECTION_FULLY_VISITED;
}

void ct_section_set_skipped(struct ct_section* section) {
	section->status = CT_SECTION_SKIPPED_BY_TAG;
}

bool ct_section_still_needs_execution(struct ct_section* section) {
	if (section->status == CT_SECTION_UNVISITED || section->status == CT_SECTION_PARTIALLY_VISITED) {
		return true;
	}
	else {
		return false;
	}
}

int ct_section_get_level(const struct ct_section* section) {
	int level = 0;
	const struct ct_section* tmp = section;

	while (tmp->parent != NULL) {
		level++;
		tmp = tmp->parent;
	}

	return level;
}

bool ct_section_is_fully_visited(struct ct_section* section) {
	if (section->children_number == 0) {
		return true;
	}
	else {
		struct ct_section* next_child = section->first_child;

		while (next_child != NULL) {
			if (ct_section_still_needs_execution(next_child)) {
				return false;
			}
			next_child = next_child->next_sibling;
		}
		return true;
	}
}

void ct_section_draw_tree(const struct ct_section* section, const char* format, ...) {
	//create image name
	va_list ap;
	char image_template[CT_BUFFER_SIZE];
	va_start(ap, format);
	vsnprintf(image_template, CT_BUFFER_SIZE, format, ap);
	va_end(ap);

	char dot_filename[CT_BUFFER_SIZE];
	strncpy(dot_filename, image_template, CT_BUFFER_SIZE);
	//TODO here we need to make sure ".dot" can be put within the buffer
	strcat(dot_filename, ".dot");

	FILE* dot_file = fopen(dot_filename, "w");
	if (dot_file == NULL) {
		fprintf(stderr, "Can't create dot_file %s! Exiting...", dot_filename);
		//TODO create a new error like FILE_ERROR_CALLBACK
		exit(1);
	}
	compute_section_tree_dot_file(dot_file, section);
	fclose(dot_file);

	char png_filename[CT_BUFFER_SIZE];
	strncpy(png_filename, image_template, CT_BUFFER_SIZE);
	//TODO here we need to make sure ".dot" can be put within the buffer
	strcat(png_filename, ".png");

	char command[CT_BUFFER_SIZE];
	snprintf(command, CT_BUFFER_SIZE, "dot -Tpng -o%s %s", png_filename, dot_filename);
	system(command);
	unlink(dot_filename);
}

static int populate_section_status_buffer_string(const int ss, int space_left, char* buffer) {
	int i = 0;
	//TODO renamed DONE, EXECUTED and UNEXECUTED to fit section status renaming
	switch (ss) {
	case CT_SECTION_FULLY_VISITED: { i += snprintf(&buffer[i], space_left - i, "DONE"); break; }
	case CT_SECTION_PARTIALLY_VISITED: { i += snprintf(&buffer[i], space_left - i, "EXECUTED"); break; }
	case CT_SECTION_UNVISITED: { i += snprintf(&buffer[i], space_left - i, "UNEXECUTED"); break; }
	case CT_SECTION_SIGNAL_DETECTED: { i += snprintf(&buffer[i], space_left - i, "SIGNAL DETECTED"); break; }
	default: {
		//TODO create a new error type, like SWITCH_DEFAULT_ERROR_CALLBACK
		fprintf(stderr, "invalid section status %d\n", ss);
		exit(1);
	}
	}

	return i;
}

static void compute_section_tree_dot_file(FILE* fout, const struct ct_section* section) {
	fprintf(fout, "digraph {\n");

	// ******************* define nodes **********************
	update_section_tree_dot_file_with_section_info(fout, section);
	// ******************* create edges **********************
	update_section_tree_dot_file_with_section_edges(fout, section);

	fprintf(fout, "}\n");
}

/**
 * Adds inside a *.dot file the lists of all the nodes inside the section tree starting from a given one
 *
 * \pre
 * 	\li \c fout open in "w" mode;
 * \post
 * 	\li \c fout contains node whose names patterns are "SECTION%5d" ("%05d" refers to a given section id)
 *
 * @param[inout] fout the file to write on
 * @param[in] section the section where we start writing information from
 */
static void update_section_tree_dot_file_with_section_info(FILE* fout, const struct ct_section* section) {
	char buffer[CT_BUFFER_SIZE];
	populate_section_status_buffer_string(section->status, CT_BUFFER_SIZE, buffer);
	fprintf(fout, "\tSECTION%05d [label=\"%s\\nlevel=%d;\\nstatus=%s\", shape=\"box\"];\n", section->id, section->description, ct_section_get_level(section), buffer);

	if (section->first_child == NULL) {
		return;
	}

	update_section_tree_dot_file_with_section_info(fout, section->first_child);

	struct ct_section* tmp = section->first_child->next_sibling;
	while (tmp != NULL) {
		update_section_tree_dot_file_with_section_info(fout, tmp);
		tmp = tmp->next_sibling;
	}

}

/**
 * Adds inside a "*.dot" file all the edges between the nodes
 *
 * \pre
 * 	\li \c fout open in "w" mode;
 * 	\li ::update_section_tree_dot_file_with_section_info called;
 * \post
 * 	\li \c fout contains all the edges;
 *
 * @param[inout] fout the file to write on
 * @param[in] section the section where we need to start adding edges from
 */
static void update_section_tree_dot_file_with_section_edges(FILE* fout, const struct ct_section* section) {
	if (section->first_child == NULL) {
		return;
	}

	update_section_tree_dot_file_with_section_edges(fout, section->first_child);

	fprintf(fout, "\tSECTION%05d -> SECTION%05d;\n", section->id, section->first_child->id);
	struct ct_section* tmp = section->first_child->next_sibling;
	while (tmp != NULL) {
		fprintf(fout, "SECTION%05d -> SECTION%05d;\n", section->id, tmp->id);
		tmp = tmp->next_sibling;
	}

}
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




static void ct_failsig_handler(int signum);

void ct_register_signal_handlers() {
	(ct_model)->_crashc_sigaction.sa_handler = ct_failsig_handler;
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
//		perror("Error: cannot handle SIGINT"); //should not happen
//	}
	//TODO do not register signal if it  is already registers by the program under test itself
	if (sigaction(SIGFPE, &((ct_model)->_crashc_sigaction), NULL) == -1) {
		perror("Error: cannot handle SIGFPE"); //should not happen
	}
}

/**
 * Code to be run when a signal was not handled by the program under test itself
 *
 * This handler is used for every signal whose delivery is considered by CrashC to
 * imply a failure in the running test. This handler is used only if the programmer
 * does not register any other signal handler for the particular signal being considered.
 * What we need to do is mark the current running test as failed and update its status
 * in order that it is not run again on the next CT_LOOPER iteration.
 *
 * @param signum an ID representing the signal detected
 *
 */
static void ct_failsig_handler(int signum) {

	//printf("marking section \"%s\" as signal detected!\n", (ct_model)->current_section->description);
    //Mark test as failed code
	ct_section_set_signaled((ct_model)->current_section);
	(ct_model)->current_section->signal_detected = signum;

	(ct_model)->current_snapshot->status = CT_SNAPSHOT_SIGNALED;
	struct ct_test_report* report = ct_list_tail((ct_model)->test_reports_list);
	ct_update_test_outcome(report, (ct_model)->current_snapshot);
	(ct_model)->current_snapshot = NULL;

	//after handling the signal we return to sigsetjmp function (we will enter in the "if" where sigsetjmp is located)
    siglongjmp((ct_model)->jump_point, CT_SIGNAL_JUMP_CODE);
}
/*
 * tag.c
 *
 *  Created on: Feb 16, 2017
 *      Author: koldar
 */





struct ct_tag* ct_tag_init(const char* name) {
	struct ct_tag* ret_val = malloc(sizeof(struct ct_tag));
	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->name = strdup(name);

	return ret_val;
}

void ct_tag_destroy(struct ct_tag* tag) {
	free(tag->name);
	free(tag);
}

int ct_tag_compare(const struct ct_tag* tag1, const struct ct_tag* tag2) {
	if (tag1 == tag2) {
		return 0;
	}
	//TODO perform comparation(if the user wants it) with case insensitive as well!
	return strcmp(tag1->name, tag2->name);
}

bool ct_have_tag_set_intersection(const ct_tag_hashtable_o* tag_set1, const ct_tag_hashtable_o* tag_set2) {
	//TODO optimization: pick up the set with the least number of elements first. In this way the outer loop will be executed less

	CT_ITERATE_VALUES_ON_HT(tag_set1, tag1, struct ct_tag*) {
		CT_ITERATE_VALUES_ON_HT(tag_set2, tag2, struct ct_tag*) {
			if (ct_tag_compare(tag1, tag2) == 0) {
				return true;
			}
		}
	}

	return false;
}

void ct_tag_ht_put(ct_tag_hashtable_o* tag_hashtable, const char* name) {
	//TODO to improve performances you don't need to create a tag every time you see a name, There should be a tag pool containing
	//all the tags in the project. In this way, tag with the same name are created only once
	ct_ht_put(tag_hashtable, ct_string_hash(name), ct_tag_init(name));
}

int ct_string_hash(const char* str) {
	unsigned int hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

const char* ct_next_tag_in_string(const char* const str, char separator, char* characters_to_ignore, char* output) {
	char* output_index = NULL;
	char ch = '\0';
	const char* input = NULL;
	bool to_skip = false;

	//I don't use strtok because I don't want to use something with side effects
	input = str;
	output_index = &output[0];
	while (true) {
		ch = *input;

		//check if the character has to be skipped
		to_skip = false;
		for (int i=0; (characters_to_ignore[i])!='\0'; i++) {
			if (ch == characters_to_ignore[i]) {
				to_skip = true;
			}
		}

		if (!to_skip) {
			//add the character to the buffer
			if (ch == separator || ch == '\0') {
				*output_index = '\0';
				goto exit;
			} else {
				*output_index = ch;
			}
		}

		//go to the next character
		output_index += 1;
		input += 1;
	}

	exit:;
	if (*input != '\0') {
		input++;
	}
	return input;
}

void ct_tag_ht_populate(ct_tag_hashtable_o* output, const char* const tags, char separator) {
	char token[CT_BUFFER_SIZE];
	char* position_in_buffer = NULL;
	int token_id;

	const char* token_string = tags;

	while (*token_string != '\0') {
		token_string = ct_next_tag_in_string(token_string, separator, "", token);

		if (strlen(token) == 0) {
			return;
		}

		//add the fetched tag inside the section
		token_id = ct_string_hash(token);
		struct ct_tag* tag_with_token_id = ct_ht_get(output, token_id);
		if (tag_with_token_id == NULL) {
			tag_with_token_id = ct_tag_init(token);
			ct_ht_put(output, token_id, tag_with_token_id);
		}
	}
}
/**
 * This file contains the function used by CrashC to generate and handle the test reports
 * TODO: Fix strdup unsafe call, add dimension limit to avoid overflows
 *
 * Author: Lorenzo Nodari
 *
 */

#include <string.h>





struct ct_test_report* ct_init_test_report(struct ct_snapshot* tc_snapshot) {
	struct ct_test_report* ret_val = malloc(sizeof(struct ct_test_report));
	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->filename = NULL;
	ret_val->execution_time = 0;
	ret_val->outcome = CT_TEST_SUCCESS;
	ret_val->testcase_snapshot = tc_snapshot;

	return ret_val;
}

void ct_destroy_test_report(struct ct_test_report* report) {
	free(report->filename);
	ct_destroy_snapshot_tree(report->testcase_snapshot);
	free(report);

}

struct ct_snapshot* ct_init_section_snapshot(struct ct_section* section) {
	struct ct_snapshot* ret_val = malloc(sizeof(struct ct_snapshot));
	if (ret_val == NULL) {
		CT_MALLOC_ERROR_CALLBACK();
	}

	ret_val->description   = strdup(section->description);
	ret_val->tags          = section->tags;
	ret_val->type          = section->type;
	ret_val->status        = CT_SNAPSHOT_OK;
	ret_val->elapsed_time  = 0;
	ret_val->assertion_reports = ct_list_init();
	ret_val->parent = NULL;
	ret_val->next_sibling = NULL;
	ret_val->first_child = NULL;

	return ret_val;
}

void ct_destroy_snapshot_tree(struct ct_snapshot* snapshot) {
	free(snapshot->description);
	ct_list_destroy_with_elements(snapshot->assertion_reports, (ct_destroyer_c) ct_destroy_assert_report);

	struct ct_snapshot* next_child = snapshot->first_child;
	while (next_child != NULL) {
		struct ct_snapshot* tmp = next_child->next_sibling;
		ct_destroy_snapshot_tree(next_child);
		next_child = tmp;
	}

	free(snapshot);
}

struct ct_snapshot* ct_add_snapshot_to_tree(struct ct_snapshot* to_add, struct ct_snapshot* tree) {
	to_add->parent = tree;

	struct ct_snapshot* tmp = tree->first_child;
	if (tmp == NULL) {
			tree->first_child = to_add;
			return to_add;
	}
	while (true) {
		if (tmp->next_sibling != NULL) {
			tmp = tmp->next_sibling;
		}
		else {
			tmp->next_sibling = to_add;
			return to_add;
		}
	}
}

void ct_update_snapshot_status(struct ct_section* section, struct ct_snapshot* snapshot) {
	if (section->status == CT_SECTION_SIGNAL_DETECTED) {
		snapshot->status = CT_SNAPSHOT_SIGNALED;
	}
}

void ct_update_test_outcome(struct ct_test_report* report, struct ct_snapshot* last_snapshot) {
	if (last_snapshot->status != CT_SNAPSHOT_OK) {
		report->testcase_snapshot->status = last_snapshot->status;
		report->outcome = CT_TEST_FAILURE;
	}
}


/*
 * ct_utils.c
 *
 *  Created on: Oct 31, 2017
 *      Author: noodles
 */



enum time_format {
	TF_NANO    = 0,
	TF_MICRO   = 1,
	TF_MILLI   = 2,
	TF_SECONDS = 3
};

static enum time_format parse_time_format(const char* str);

char* const ct_file_basename(char* const filename) {
	int i = 0;
	int last_slash_index = 0;
	char c = filename[0];

	while (c != '\0') {
		if (c == '/') {
			last_slash_index = i;
		}

		i++;
		c = filename[i];
	}

	return filename + (last_slash_index + 1);
}

struct timespec ct_get_time() {
	struct timespec ret_val;
	clock_gettime(CLOCK_MONOTONIC, &ret_val);
	return ret_val;
}

static enum time_format parse_time_format(const char* str) {
	switch (*str) {
	case 's': return TF_SECONDS;
	case 'm': return TF_MILLI;
	case 'u': return TF_MICRO;
	case 'n': return TF_NANO;
	}
}

long ct_compute_time_gap(struct timespec start, struct timespec end, const char* format_str)  {
	long sec, nano_sec, ret_val;
	enum time_format format = parse_time_format(format_str);

	sec = end.tv_sec - start.tv_sec;
	nano_sec = end.tv_nsec - start.tv_nsec;

	ret_val = ((sec * 1E9 + nano_sec) + nano_sec);
	for (int i = 0; i < format; i++) {
		ret_val /= 1000;
	}

	return ret_val;
}

#endif /* CRASHC_ALL_IN_ONE_H_ */
