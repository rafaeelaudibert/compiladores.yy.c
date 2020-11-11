#include "ASMGeneration.h"

void generate_ASM(TAC *tac)
{
    FILE *fout = fopen("out.s", "w");

    // DATA SECTION
    fprintf(fout, "# DATA SECTION\n"
                  ".section .rdata,\"dr\"\n"
                  "printint:   .ascii \"%%d\\0\"\n"
                  "printstring:    .ascii \"%%s\\0\"\n");

    // GLOBAL VARIABLES DECLARATIONS
    // TODO, já que precisamos salvar valor inicial das variáveis na HASH pra funcionar como eu quero

    // FUNCS DECLARATIONS
    while (tac)
    {
        switch (tac->type)
        {
        case TAC_BEGINFUN:
            fprintf(fout,
                    "\n\n\n\t.globl\t_%s\n"
                    "_%s:\n"
                    "\t# TAC_BEGINFUN\n"
                    "\tpushl\t%%ebp\n"
                    "\tmovl\t%%esp, %%ebp\n"
                    "\tandl\t$-16, %%esp\n\n",
                    tac->res->text,
                    tac->res->text);
            break;
        case TAC_ENDFUN:
            fprintf(fout,
                    "\t# TAC_ENDFUN\n"
                    "\tleave\n"
                    "\tret\n");
            break;
        default:
            break;
        }

        tac = tac->next;
    }

    fclose(fout);
    return;
}