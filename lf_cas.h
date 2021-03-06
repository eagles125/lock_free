/*
 * File     : lf_cas.h
 * Author   : Jérémy Zurcher  <jeremy@asynk.ch>
 * Date     : 01/11/09 
 * License  :
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _LF_CAS_H_
#define _LF_CAS_H_

# ifdef __cplusplus
extern "C" {
# endif /* __cplusplus */

struct lf_pointer;

typedef struct lf_pointer {
    volatile struct lf_pointer *ptr;
    volatile unsigned int count;
} lf_pointer_t;

union ptr_u {
    lf_pointer_t ptr;
    volatile long long concat;
};

#define lf_eql(ptra,ptrb) (((union ptr_u)(ptra)).concat == ((union ptr_u)(ptrb)).concat)

/* CMPXCHG8B m64   Compare EDX:EAX with m64. If equal, set ZF and load ECX:EBX into m64. Else, clear ZF and load m64 into EDX:EAX. */
static inline unsigned int cas( volatile lf_pointer_t *mem,
                                volatile lf_pointer_t old,
                                volatile lf_pointer_t new ) {
    char result;
    __asm__ __volatile__("lock; cmpxchg8b %0; setz %1;"
            : "=m"(*mem), "=q"(result)
            : "m"(*mem), "d" (old.count), "a" (old.ptr),
            "c" (new.count), "b" (new.ptr)
            : "memory");
    return (int)result;
}

# ifdef __cplusplus
}
# endif /* __cplusplus */

# endif /* _LF_CAS_H_ */

