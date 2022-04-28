// Nome: Alexandre Carneiro e Lucas Sidnei dos Santos
// GRR: 20166359 e 20163040

#include "functions.h"
#include <likwid.h>

void TrocaLinhas(double *A, double *b, int l, uint iPivo, int n)
{
    double aux = 0.00;

    // Troca elementos das linhas na matriz: a
    for (int i = 0; i < n; i++)
    {
        aux = A[(l * n) + i];
        A[(l * n) + i] = A[(iPivo * n) + i];
        A[(iPivo * n) + i] = aux;
    }

    // Troca os elementos do vetor: b
    aux = b[l];
    b[l] = b[iPivo];
    b[iPivo] = aux;
}

void NewtonResolveMethod(double *mF, double *mFD, int n)
{
    for (int i = 0; i < n; ++i)
    {

        for (int k = i + 1; k < n; ++k)
        {
            uint iPivo = EncontrarMax(mFD, n);
            if (fabs(mFD[(k * n)]) < fabs(mFD[i * n]) && fabs(mFD[k * n]) != 0 && fabs(mFD[i * n]) != 0)
                TrocaLinhas(mFD, mF, i, iPivo, n);

            double m = mFD[(k * n) + i] / mFD[(i * n) + i];
            mFD[(k * n) + i] = 0.0;
            for (int j = i + 1; j < n; ++j)
                mFD[(k * n) + j] -= mFD[(i * n) + j] * m;

            mF[k] -= mF[i] * m;
        }
    }
}

uint EncontrarMax(double *x, int n)
{
    int lineIndex = 0;
    double max = fabs(x[0]);
    for (int i = 0; i < n; ++i)
        if (fabs(x[i * n]) > max)
        {
            max = fabs(x[i * n]);
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

double *NewtonGaussSeidelResolveMethod(double *A, double b[], int n)
{

    double e = 0.000001;
    int i, j, k = 0;
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
                sum += A[(i * n) + j] * x1[j];

            for (j = i + 1; j < n; j++)
                sum += A[(i * n) + j] * x0[j];

            x1[i] = (b[i] - sum) / A[(i * n) + i];
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
    LIKWID_MARKER_INIT;

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
        LIKWID_MARKER_START(markerName(type==0?"newton_padrao_total":"newton_inexato_total",10));

        double *mFD = GetMatrix(in, x_ant, type);
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
    
        LIKWID_MARKER_STOP(markerName(type==0?"newton_padrao_total":"newton_inexato_total",10));

    }

    LIKWID_MARKER_CLOSE;    
}

double *Retrosub(double *mF, double *mFD, int n)
{
    double *x = (double *)malloc(sizeof(double) * n);
    for (int i = n - 1; i >= 0; --i)
    {
        x[i] = mF[i];
        for (int j = i + 1; j < n; ++j)
            x[i] -= mFD[(i * n) + j] * x[j];

        x[i] /= mFD[(i * n) + i];
    }

    return x;
}

double *ResolveLinearSistem(double *mF, double *mFD, int n, int type)
{

    if (type == 0)
    {
        LIKWID_MARKER_START(markerName("resolucao_sl_newton_padrao",10));
        NewtonResolveMethod(mF, mFD, n);
        double *x = Retrosub(mF, mFD, n);
        LIKWID_MARKER_STOP(markerName("resolucao_sl_newton_padrao",10));

        return x;
    }

    if (type == 1)
    {
        LIKWID_MARKER_START(markerName("resolucao_sl_newton_inexato",10));
        double *x = NewtonGaussSeidelResolveMethod(mFD, mF, n); 
        LIKWID_MARKER_STOP(markerName("resolucao_sl_newton_inexato",10));

        return x;

    }

    return NULL;
}

double *GetMatrix(infos *in, double *x, int type)
{

    double *mF = malloc(sizeof(double) * in->n * in->n);
    double timeDerivate = timestamp();

    for (int ii = 0; ii < in->n / SIZE_BLOCK; ++ii)
    {
        int istart = ii * SIZE_BLOCK;
        int iend = istart + SIZE_BLOCK;

        for (int jj = 0; jj < in->n / SIZE_BLOCK; ++jj)
        {
            int jstart = jj * SIZE_BLOCK;
            int jend = jstart + SIZE_BLOCK;

            for (int i = istart; i < iend; ++i)
            {
                LIKWID_MARKER_START(markerName(type==0?"vetor_gradiente_newton_padrao":"vetor_gradiente_newton_inexato",10));
                in->solution[i] = rosenbrock_dx(i, x, in->n) * -1;
                LIKWID_MARKER_STOP(markerName(type==0?"vetor_gradiente_newton_padrao":"vetor_gradiente_newton_inexato",10));


                LIKWID_MARKER_START(markerName(type==0?"matriz_hessiana_newton_padrao":"matriz_hessiana_newton_inexato",10));
                for (int j = jstart; j < jend; j += SIZE_BLOCK)
                {
                    mF[(i * in->n) + j] = rosenbrock_dxdy(i, j, x, in->n);
                    mF[(i * in->n) + (j + 1)] = rosenbrock_dxdy(i, (j + 1), x, in->n);
                    mF[(i * in->n) + (j + 2)] = rosenbrock_dxdy(i, (j + 2), x, in->n);
                    mF[(i * in->n) + (j + 3)] = rosenbrock_dxdy(i, (j + 3), x, in->n);
                    mF[(i * in->n) + (j + 4)] = rosenbrock_dxdy(i, (j + 4), x, in->n);
                    mF[(i * in->n) + (j + 5)] = rosenbrock_dxdy(i, (j + 5), x, in->n);
                    mF[(i * in->n) + (j + 6)] = rosenbrock_dxdy(i, (j + 6), x, in->n);
                    mF[(i * in->n) + (j + 7)] = rosenbrock_dxdy(i, (j + 7), x, in->n);
                    mF[(i * in->n) + (j + 8)] = rosenbrock_dxdy(i, (j + 8), x, in->n);
                    mF[(i * in->n) + (j + 9)] = rosenbrock_dxdy(i, (j + 9), x, in->n);
                }
                LIKWID_MARKER_STOP(markerName(type==0?"matriz_hessiana_newton_padrao":"matriz_hessiana_newton_inexato",10));

            }
        }
    }

    CalculateTimeDerivate(type, in, timeDerivate);

    return mF;
}
