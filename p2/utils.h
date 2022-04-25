// Nome: Alexandre Carneiro e Lucas Sidnei dos Santos
// GRR: 20166359 e 20163040

#ifndef __UTILS_H__
#define __UTILS_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <matheval.h>
#include <time.h>
#include <sys/time.h>

typedef double real_t;
typedef double rtime_t;
typedef char * string_t;

// SIMD alignment macros
#define SIZE_BLOCK 10
#define BUFFER_SIZE 10000000
#define ALIGN_64 __attribute__((aligned(64)))
#define ALIGN_32 __attribute__((aligned(32)))
#define ALIGN_16 __attribute__((aligned(16)))
#define numDigits(n)  6  // ( (int) log10(n) + 1 )

typedef struct
{
     int it;
     double timeTotal;
     double timeDerivate;
     double timeSL;
     double **solution;
} response;

typedef struct
{
     int n;
     char f[BUFFER_SIZE];
     double *initialsApproaches;
     double *solution;
     response newton;
     response newtonInaccurate;
     double epsilon;
     int itMax;

} infos;

double *GetMatrix(infos *in, double *x, int type);
infos *GetInfos(int *countProblems);
char *validateArgs(int argc, char *argv[]);
void PrintResult(infos *in, char *arqName);
void CalculateTimeDerivate(int type, infos *in, double timeDerivate);
void CalculateTimeSL(int type, infos *in, double timeSL);
void CalculateTimeTotal(int type, infos *in, double timeTotal);
void PrintSolution(infos *in, double **x, int it, int n);
void PrintVector(double *x, int n);
void PrintMatrix(double **x, int n);
double GetBiggestValue(double *x, int n);
double timestamp(void);
string_t markerName(string_t baseName, int n);

#endif
