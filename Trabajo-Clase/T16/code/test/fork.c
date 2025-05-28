#include "syscall.h"

void nada(int);
int id;
void rutina(int param)
{
    Write("hola, nuevo fork2\n", 4, 1);
}

int main()
{
    Fork((void*)rutina);
    Write("basura", 6, 1);
}

