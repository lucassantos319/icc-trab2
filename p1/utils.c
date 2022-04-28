// Nome: Alexandre Carneiro e Lucas Sidnei dos Santos
// GRR: 20166359 e 20163040

#include "utils.h"

infos *GetInfos(int *countProblems)
{

    infos *in = (infos *)malloc(sizeof(infos) * (*countProblems));
    int length;

    while (scanf("%d", &in[*countProblems - 1].n) == 1)
    {

        in[(*countProblems) - 1].initialsApproaches = (double *)malloc(sizeof(double) * in[(*countProblems) - 1].n);
        scanf("%s", in[(*countProblems) - 1].f);

        length = strlen(in[(*countProblems)].f);

        if (length > 0 && in[(*countProblems)].f[length - 1] == '\n')
            in[(*countProblems)].f[length - 1] = '\0';

        for (int i = 0; i < in[(*countProblems) - 1].n; ++i)
            scanf("%lf", &in[(*countProblems) - 1].initialsApproaches[i]);

        scanf("%lf", &in[(*countProblems) - 1].epsilon);
        scanf("%d", &in[(*countProblems) - 1].itMax);

        in[(*countProblems) - 1].newton.solution = (double **)malloc(sizeof(double *) * in[(*countProblems) - 1].itMax);
        for (int i = 0; i < in[(*countProblems) - 1].itMax; ++i)
            in[(*countProblems) - 1].newton.solution[i] = (double *)malloc(sizeof(double) * in[(*countProblems) - 1].n);

        in[(*countProblems) - 1].newtonInaccurate.solution = (double **)malloc(sizeof(double *) * in[(*countProblems) - 1].itMax);
        for (int i = 0; i < in[(*countProblems) - 1].itMax; ++i)
            in[(*countProblems) - 1].newtonInaccurate.solution[i] = (double *)malloc(sizeof(double) * in[(*countProblems) - 1].n);

        ++(*countProblems);
        in = realloc(in, sizeof(infos) * (*countProblems) - 1);
    }

    return in;
}

char *validateArgs(int argc, char *argv[])
{
    if (argc == 3)
    {
        if (strcmp(argv[1], "-o") == 0)
            return argv[2];
    }

    return NULL;
}

int GetMaxIt(infos in)
{
    int newtonIt = in.newton.it, newtonInaccuraceIt = in.newtonInaccurate.it;
    // printf("newtonIt = %d ; ", newtonIt);
    // printf("newtonModifyIt = %d ; ", newtonModifyIt);
    // printf("newtonInaccuraceIt = %d\n", newtonInaccuraceIt);

    if (newtonIt >= newtonInaccuraceIt)
        return newtonIt;

    if (newtonInaccuraceIt >= newtonIt)
        return newtonInaccuraceIt;

    return 0;
}

void PrintResult(infos *in, char *arqName)
{
    if (arqName == NULL)
    {

        printf("%d\n", in->n);
        printf("%s\n", in->f);
        printf("#\n");
        printf("Iteração \t| Newton Padrão \t| Newton Inexato\n");

        int maxIt = GetMaxIt(*in);
        for (int j = 0; j < maxIt; ++j)
        {
            printf("%d \t|", j);

            if (in->newton.it > j)
            {
                double fx = in->newton.solution[j][0];
                if (isnan(fx) || isinf(fx))
                    printf(" \t\t\t| ");
                else
                    printf(" %1.14e\t| ", fx);
            }
            else
                printf(" \t\t\t| ");

            if (in->newtonInaccurate.it > j)
            {
                double fx = in->newtonInaccurate.solution[j][0];
                if (isnan(fx) || isinf(fx))
                    printf(" \t\t\t| ");
                else
                    printf(" %1.14e\t| ", fx);
            }
            else
                printf(" \t\t\t| ");

            printf("\n");
        }

        printf("Tempo total \t| %1.14e\t| %1.14e\n", in->newton.timeTotal, in->newtonInaccurate.timeTotal);
        printf("Tempo derivadas | %1.14e\t| %1.14e\n", in->newton.timeDerivate, in->newtonInaccurate.timeDerivate);
        printf("Tempo SL \t| %1.14e\t| %1.14e\n", in->newton.timeSL, in->newtonInaccurate.timeSL);
        printf("#\n\n");
    }
    else
    {
        FILE *arq;
        printf("entrei aqui");
        arq = fopen(arqName, "w+");

        fprintf(arq, "%d\n", in->n);
        fprintf(arq, "%s\n", in->f);
        fprintf(arq, "#\n");
        fprintf(arq, "Iteração \t| Newton Padrão \t| Newton Modificado \t| Newton Inexato\n");

        int maxIt = GetMaxIt(*in);
        for (int j = 0; j < maxIt; ++j)
        {
            fprintf(arq, "%d \t|", j);

            if (in->newton.it > j)
            {
                double fx = in->newton.solution[j][0];
                if (isnan(fx) || isinf(fx))
                    fprintf(arq, " %1.14e\t\t\t| ", fx);
                else
                    fprintf(arq, " %1.14e\t| ", fx);
            }

            if (in->newtonInaccurate.it > j)
            {
                double fx = in->newtonInaccurate.solution[j][0];
                if (isnan(fx) || isinf(fx))
                    fprintf(arq, " %1.14e\t\t\t| ", fx);
                else
                    fprintf(arq, " %1.14e\t| ", fx);
            }
            fprintf(arq, "\n");
        }

        fprintf(arq, "Tempo total \t| %1.14e\t| %1.14e\n", in->newton.timeTotal, in->newtonInaccurate.timeTotal);
        fprintf(arq, "Tempo derivadas | %1.14e\t| %1.14e\n", in->newton.timeDerivate, in->newtonInaccurate.timeDerivate);
        fprintf(arq, "Tempo SL \t| %1.14e\t| %1.14e\n", in->newton.timeSL, in->newtonInaccurate.timeSL);
        fprintf(arq, "#\n\n");
    }
}

void PrintMatrix(double **x, int n)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            printf("%g ", x[i][j]);

        printf("\n");
    }
}

void PrintSolution(infos *in, double **x, int it, int n)
{
    int count;
    char **variables;

    void *f = evaluator_create(in->f);
    assert(f);
    evaluator_get_variables(f, &variables, &count);

    for (int i = 0; i < it + 1; ++i)
    {
        for (int j = 0; j < n; ++j)
            printf("x => %1.14e\n", x[i][j]);

        printf("\n");
    }
}

void PrintVector(double *x, int n)
{
    for (int i = 0; i < n; ++i)
        printf("%g ", x[i]);

    printf("\n");
}

double GetBiggestValue(double *x, int n)
{
    double max = fabs(x[0]);
    for (int i = 1; i < n; ++i)
        if (fabs(x[i]) > max)
            max = fabs(x[i]);

    return max;
}

double timestamp(void)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)(tp.tv_sec * 1000.0 + tp.tv_usec / 1000.0));
}

void CalculateTimeTotal(int type, infos *in, double timeTotal)
{
    if (type == 0)
    {
        in->newton.timeTotal = 0;
        in->newton.timeTotal += timestamp() - timeTotal;
    }

    if (type == 1)
    {
        in->newtonInaccurate.timeTotal = 0;
        in->newtonInaccurate.timeTotal += timestamp() - timeTotal;
    }
}

void CalculateTimeSL(int type, infos *in, double timeSL)
{
    if (type == 0)
        in->newton.timeSL += timestamp() - timeSL;

    if (type == 1)
        in->newtonInaccurate.timeSL += timestamp() - timeSL;
}

void CalculateTimeDerivate(int type, infos *in, double timeDerivate)
{
    if (type == 0)
    {
        in->newton.timeDerivate = 0;
        in->newton.timeDerivate += timestamp() - timeDerivate;
    }

    if (type == 1)
    {
        in->newtonInaccurate.timeDerivate = 0;
        in->newtonInaccurate.timeDerivate += timestamp() - timeDerivate;
    }
}
