// Nome: Alexandre Carneiro e Lucas Sidnei dos Santos
// GRR: 20166359 e 20163040

#include "functions.h"

void TrocaLinhas(double **A, double *b, int l, uint iPivo, int n)
{
    double aux = 0.00;

    // Troca elementos das linhas na matriz: a
    for (int i = 0; i < n; i++)
    {
        aux = A[l][i];
        A[l][i] = A[iPivo][i];
        A[iPivo][i] = aux;
    }

    // Troca os elementos do vetor: b
    aux = b[l];
    b[l] = b[iPivo];
    b[iPivo] = aux;
}

void NewtonResolveMethod(double *mF, double **mFD, int n)
{
    for (int i = 0; i < n; ++i)
    {

        for (int k = i + 1; k < n; ++k)
        {
            uint iPivo = EncontrarMax(mFD, n);
            if (fabs(mFD[k][0]) < fabs(mFD[i][0]) && fabs(mFD[k][0]) != 0 && fabs(mFD[i][0]) != 0)
                TrocaLinhas(mFD, mF, i, iPivo, n);

            double m = mFD[k][i] / mFD[i][i];
            mFD[k][i] = 0.0;
            for (int j = i + 1; j < n; ++j)
                mFD[k][j] -= mFD[i][j] * m;

            mF[k] -= mF[i] * m;
        }
    }
}

uint EncontrarMax(double **x, int n)
{
    int lineIndex = 0;
    double max = fabs(x[0][0]);
    for (int i = 0; i < n; ++i)
        if (fabs(x[i][0]) > max)
        {
            max = fabs(x[i][0]);
            lineIndex = i;
        }

    return lineIndex;
}

double dr(double *x0, double *x1, unsigned int n)
{

    double dr, x_max;
    unsigned int i;

    dr = 0;
    x_max = 0;

    for (i = 0; i < n; i++)
    {
        if (fabs(x1[i] - x0[i]) > dr)
            dr = fabs(x1[i] - x0[i]);
        if (fabs(x1[i]) > x_max)
            x_max = fabs(x1[i]);
    }
    dr = dr / x_max;
    return dr;
}

double *NewtonGaussSeidelResolveMethod(double **A, double b[], int n)
{

    double e = 0.000001;
    int i, j, k;
    double sum;
    double x0[n];
    double *x1;

    x1 = malloc(sizeof(double) * n);

    for (i = 0; i < n; i++)
        x0[i] = x1[i] = 0;

    do
    {
        k++;

        for (i = 0; i < n; i++)
            x0[i] = x1[i];

        for (i = 0; i < n; i++)
        {
            sum = 0;

            for (j = 0; j < i; j++)
                sum += A[i][j] * x1[j];

            for (j = i + 1; j < n; j++)
                sum += A[i][j] * x0[j];

            x1[i] = (b[i] - sum) / A[i][i];
        }
    } while (dr(x0, x1, n) > e && k < 50);

    return x1;
}

void CopySolution(int type, infos *in, int i, double *x)
{
    if (type == 0)
    {
        in->newton.it = i + 1;
        in->newton.solution[i] = (double *)malloc(sizeof(double));
        in->newton.solution[i][0] = rosenbrock(x, in->n);
    }

    if (type == 1)
    {
        in->newtonInaccurate.it = i + 1;
        in->newtonInaccurate.solution[i][0] = rosenbrock(x, in->n);
    }
}

void ResolveProblems(infos *in)
{
    in->solution = malloc(sizeof(double) * in->n);
    double initValuesAux;
    double *x_ant = (double *)malloc(sizeof(double) * in->n);
    double *x = (double *)malloc(sizeof(double) * in->n);
    double *delta = (double *)malloc(sizeof(double) * in->n);

    for (int i = 0; i < in->n; ++i)
    {
        initValuesAux = in->initialsApproaches[i];
        x_ant[i] = initValuesAux;
    }

    for (int type = 0; type < 2; ++type)
    {
        double **mFD = GetMatrix(in, x_ant, type);
        double *mF = in->solution;
        double timeTotalInicial = timestamp();

        if (mF == NULL)
            return;

        for (int i = 0; i < in->itMax; ++i)
        {
            double big = GetBiggestValue(mF, in->n);
            if (big <= in->epsilon)
            {
                // printf("type = %d ; i = %d ; saindo 1\n", type, i);
                CopySolution(type, in, i, x_ant);
                break;
            }

            double timeSL = timestamp();
            delta = ResolveLinearSistem(mF, mFD, in->n, type);
            CalculateTimeSL(type, in, timeSL);
            for (int j = 0; j < in->n; ++j)
                x[j] = x_ant[j] + delta[j];

            big = GetBiggestValue(delta, in->n);
            if (big <= in->epsilon)
            {
                CopySolution(type, in, i, x_ant);
                break;
            }

            CopySolution(type, in, i, x_ant);
            for (int j = 0; j < in->n; ++j)
            {
                double auxX = x[j];
                x_ant[j] = auxX;
            }

            mFD = GetMatrix(in, x_ant, type);
            mF = in->solution;
        }

        CalculateTimeTotal(type, in, timeTotalInicial);
        for (int j = 0; j < in->n; ++j)
        {
            initValuesAux = in->initialsApproaches[j];
            x_ant[j] = initValuesAux;
        }

        mFD = GetMatrix(in, x_ant, type);
        mF = in->solution;
    }

    free(x_ant);
    free(x);
    free(delta);
}

double *Retrosub(double *mF, double **mFD, int n)
{
    double *x = (double *)malloc(sizeof(double) * n);
    for (int i = n - 1; i >= 0; --i)
    {
        x[i] = mF[i];
        for (int j = i + 1; j < n; ++j)
            x[i] -= mFD[i][j] * x[j];

        x[i] /= mFD[i][i];
    }

    return x;
}

double *ResolveLinearSistem(double *mF, double **mFD, int n, int type)
{

    if (type == 0)
    {
        NewtonResolveMethod(mF, mFD, n);
        return Retrosub(mF, mFD, n);
    }

    if (type == 1)
        return NewtonGaussSeidelResolveMethod(mFD, mF, n);

    return NULL;
}

double **GetMatrix(infos *in, double *x, int type)
{
    double **mF = (double **)malloc(sizeof(double *) * in->n);

    for (int i = 0; i < in->n; ++i)
        mF[i] = (double *)malloc(sizeof(double) * in->n);

    double timeDerivate = timestamp();
    for (int i = 0; i < in->n; ++i)
    {
        in->solution[i] = rosenbrock_dx(i, x, in->n) * -1;

        for (int j = 0; j < in->n; ++j)
            mF[i][j] = rosenbrock_dxdy(i, j, x, in->n);
    }

    CalculateTimeDerivate(type, in, timeDerivate);

    return mF;
}
