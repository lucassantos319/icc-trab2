// Nome: Alexandre Carneiro e Lucas Sidnei dos Santos
// GRR: 20166359 e 20163040

#include "functions.h"

int main(int argc, char *argv[])
{
    int countProblems = 1;
    char *arqName = validateArgs(argc, argv);
    infos *in = GetInfos(&countProblems);
    for (int i = 0; i < countProblems - 1; ++i)
    {
        ResolveProblems(&in[i]);
 	printf("OK\n");
 	PrintResult(&in[i], arqName);
    }
}
