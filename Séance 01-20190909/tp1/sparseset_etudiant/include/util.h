#ifndef __UTIL__H__
#define __UTIL__H__ 

// ============================================================================
// UTILITY
#define OK 0
#define KO 1

#define FOREACH_SPARSEINTSET(I, N, S) for(N = 0, I = (S)->dense ? (S)->dense[N] : 0; N < (S)->population; N++, I = (S)->dense[N])
#define FOREACH_ARRAY(T, N, A, S) for(N = 0, T = A; N < S; N++, T++)


#endif