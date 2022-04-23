// Nome: Alexandre Carneiro e Lucas Sidnei dos Santos
// GRR: 20166359 e 20163040

#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

#include "utils.h"
#include <matheval.h>
#include "Rosenbrock.h"

void ResolveProblems(infos *in);
uint EncontrarMax(double **x, int n);
double *Retrosub(double *mF, double **mFD, int n);
void CopySolution(int type, infos *in, int i, double *x);
void NewtonResolveMethod(double *mF, double **mFD, int n);
double *NewtonModifyResolveMethod(double **A, double b[], int n);
void TrocaLinhas(double **mFD, double *mF, int i, uint iPivo, int n);
double *ResolveLinearSistem(double *mF, double **mFD, int n, int type);
double *NewtonGaussSeidelResolveMethod(double **A, double b[], int n);

#endif
