#include "asmGeneration.h"
#include "hash.h"
#include "lex.yy.h"

#define IS_ABSOLUTE(op) op->type != TK_IDENTIFIER && op->type != TYPE_TMP
#define MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(op) IS_ABSOLUTE(op) ? "\tmovl\t$0x%s, %%eax\n" : "\tmovl\t_%s, %%eax\n"
#define MOVL_ABSOLUTE_OR_VARIABLE_TO_EBX(op) IS_ABSOLUTE(op) ? "\tmovl\t$0x%s, %%ebx\n" : "\tmovl\t_%s, %%ebx\n"
#define PARSE_BOOLEAN_TO_STRING(op) op->type == LIT_TRUE ? "1" : (op->type == LIT_FALSE ? "0" : op->text)

ChainedList *func_current_param = NULL, *func_param_start = NULL;

void generate_ASM(TAC *tac)
{
    FILE *fout = fopen("out.s", "w");

    // DATA SECTION
    fprintf(fout, "# DATA SECTION\n"
                  ".section .rdata,\"dr\"\n"
                  "printint:   .ascii \"%%d\\n\\0\"\n"
                  "printstring:    .ascii \"%%s\\n\\0\"\n\n");

    // GLOBAL VARIABLES DECLARATIONS
    fprintf(fout, ".data\n");
    hash_generate_global_ASM(fout);

    // FUNCS DECLARATIONS
    while (tac)
    {
        switch (tac->type)
        {
        case TAC_LABEL:
            fprintf(fout,
                    "\t# TAC_LABEL\n"
                    "_%s:\n",
                    tac->res->text);
            break;
        case TAC_FUNC_PARAM_FUNC_NAME:
            // Doesn't generate code, only sets the neccessary data
            func_current_param = tac->res->params_names;
            func_param_start = tac->res->params_names;
            break;
        case TAC_FUNC_PARAM:
            fprintf(fout,
                    "\t# TAC_FUNC_PARAM\n"
                    "\tpushl\t_%s\n",
                    (char *)func_current_param->val);
            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), PARSE_BOOLEAN_TO_STRING(tac->op1));
            fprintf(fout, "\tmovl\t%%eax, _%s\n\n", (char *)func_current_param->val);

            // Advances so that it can be used by the next parameter
            func_current_param = func_current_param->next;
            break;
        case TAC_FUNC_CALL:
            fprintf(fout,
                    "\t# TAC_FUNC_CALL\n"
                    "\tcall\t_%s\n"
                    "\tmovl\t%%eax, _%s\n",
                    tac->op1->text,
                    tac->res->text);

            // Pops the pushed elements, in reverse order
            func_param_start = revert_chained_list(func_param_start);
            func_current_param = func_param_start;

            while (func_current_param)
            {
                fprintf(fout, "\tpopl\t_%s\n", (char *)func_current_param->val);
                func_current_param = func_current_param->next;
            }
            fprintf(fout, "\n");

            // Needs to revert it back so that it can be used later
            // as `revert_chained_list` deletes the list
            func_param_start = revert_chained_list(func_param_start);
            break;
        case TAC_VECTOR_ACCESS:
            fprintf(fout,
                    "\t# TAC_VECTOR_ACCESS\n"
                    "\tmovl\t_%s+0x%d, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->op1->text,
                    (int)(strtol(tac->op2->text, NULL, 16) * 4),
                    tac->res->text);
            break;
        case TAC_COPY:
            fprintf(fout, "\t# TAC_COPY\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), PARSE_BOOLEAN_TO_STRING(tac->op1));
            fprintf(fout, "\tmovl\t%%eax, _%s\n\n", tac->res->text);

            break;
        case TAC_COPY_IDX:
            fprintf(fout, "\t# TAC_COPY_IDX\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op2), tac->op2->text);
            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EBX(tac->op1), tac->op1->text);
            fprintf(fout, "\tmovl\t%%ebx, _%s(,%%eax,4)\n\n", tac->res->text);

            break;
        case TAC_BEGINFUN:
            fprintf(
                fout,
                "\n\n\t.globl\t_%s\n"
                "_%s:\n"
                "\t# TAC_BEGINFUN\n"
                "\tpushl\t%%ebp\n"
                "\tmovl\t%%esp, %%ebp\n"
                "\tandl\t$-16, %%esp\n"
                "\tsubl\t$16, %%esp\n\n",
                tac->res->text,
                tac->res->text);
            break;
        case TAC_ENDFUN:
            fprintf(fout,
                    "\t# TAC_ENDFUN\n"
                    "\tleave\n"
                    "\tret\n\n");
            break;
        case TAC_ADD:
            fprintf(fout, "\t# TAC_ADD\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EBX(tac->op1), tac->op1->text);
            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op2), tac->op2->text);
            fprintf(fout,
                    "\taddl\t%%ebx, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_SUB:
            fprintf(fout, "\t# TAC_SUB\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EBX(tac->op2), tac->op2->text);
            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), tac->op1->text);
            fprintf(fout,
                    "\tsubl\t%%ebx, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_MULTIPLY:
            fprintf(fout, "\t# TAC_MULTIPLY\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EBX(tac->op1), tac->op1->text);
            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op2), tac->op2->text);
            fprintf(fout,
                    "\timull\t%%ebx, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_DIVIDE:
            fprintf(fout, "\t# TAC_DIVIDE\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), tac->op1->text);
            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EBX(tac->op2), tac->op2->text);
            fprintf(fout,
                    "\tcltd\n"
                    "\tidivl\t%%ebx\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_LT:
            fprintf(fout, "\t# TAC_LT\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op2), tac->op2->text);
            fprintf(fout,
                    IS_ABSOLUTE(tac->op1) ? "\tcmpl\t$0x%s, %%eax\n" : "\tcmpl\t_%s, %%eax\n",
                    tac->op1->text);
            fprintf(fout,
                    "\tsetg\t%%al\n"
                    "\tmovzbl\t%%al, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_LE:
            fprintf(fout, "\t# TAC_LE\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), tac->op1->text);
            fprintf(fout,
                    IS_ABSOLUTE(tac->op2) ? "\tcmpl\t$0x%s, %%eax\n" : "\tcmpl\t_%s, %%eax\n",
                    tac->op2->text);
            fprintf(fout,
                    "\tsetle\t%%al\n"
                    "\tmovzbl\t%%al, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_GT:
            fprintf(fout, "\t# TAC_GT\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), tac->op1->text);
            fprintf(fout,
                    IS_ABSOLUTE(tac->op2) ? "\tcmpl\t$0x%s, %%eax\n" : "\tcmpl\t_%s, %%eax\n",
                    tac->op2->text);
            fprintf(fout,
                    "\tsetg\t%%al\n"
                    "\tmovzbl\t%%al, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_GE:
            fprintf(fout, "\t# TAC_GE\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op2), tac->op2->text);
            fprintf(fout,
                    IS_ABSOLUTE(tac->op1) ? "\tcmpl\t$0x%s, %%eax\n" : "\tcmpl\t_%s, %%eax\n",
                    tac->op1->text);
            fprintf(fout,
                    "\tsetle\t%%al\n"
                    "\tmovzbl\t%%al, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_OR:
            fprintf(fout, "\t# TAC_OR\n");

            fprintf(fout,
                    IS_ABSOLUTE(tac->op1) ? "\tmov\t$0x%s, %%al\n" : "\tmov\t_%s, %%al\n",
                    PARSE_BOOLEAN_TO_STRING(tac->op1));
            fprintf(fout,
                    IS_ABSOLUTE(tac->op2) ? "\tmov\t$0x%s, %%bl\n" : "\tmov\t_%s, %%bl\n",
                    PARSE_BOOLEAN_TO_STRING(tac->op2));
            fprintf(fout, "\tor\t%%bl, %%al\n"); // al = al || bl
            fprintf(fout, "\tmov\t%%al, _%s\n\n", tac->res->text);
            break;
        case TAC_AND:
            fprintf(fout, "\t# TAC_AND\n");

            fprintf(fout,
                    IS_ABSOLUTE(tac->op1) ? "\tmov\t$0x%s, %%al\n" : "\tmov\t_%s, %%al\n",
                    PARSE_BOOLEAN_TO_STRING(tac->op1));
            fprintf(fout,
                    IS_ABSOLUTE(tac->op2) ? "\tmov\t$0x%s, %%bl\n" : "\tmov\t_%s, %%bl\n",
                    PARSE_BOOLEAN_TO_STRING(tac->op2));
            fprintf(fout, "\tand\t%%bl, %%al\n"); // al = al && bl
            fprintf(fout, "\tmov\t%%al, _%s\n\n", tac->res->text);
            break;
        case TAC_EQ:
            fprintf(fout, "\t# TAC_EQ\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), PARSE_BOOLEAN_TO_STRING(tac->op1));
            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EBX(tac->op2), PARSE_BOOLEAN_TO_STRING(tac->op2));
            fprintf(fout,
                    "\tcmpl\t%%eax, %%ebx\n"
                    "\tsete\t%%al\n"
                    "\tmovzbl\t%%al, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_DIF:
            fprintf(fout, "\t# TAC_DIF\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), tac->op1->text);
            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EBX(tac->op2), tac->op2->text);
            fprintf(fout,
                    "\tcmpl\t%%eax, %%ebx\n"
                    "\tsetne\t%%al\n"
                    "\tmovzbl\t%%al, %%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_UNARY_MINUS:
            fprintf(fout, "\t# TAC_UNARY_MINUS\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), tac->op1->text);
            fprintf(fout,
                    "\tnegl\t%%eax\n"
                    "\tmovl\t%%eax, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_UNARY_NEGATION:
            fprintf(fout, "\t# TAC_UNARY_NEGATION\n");

            // Negating a boolean, is the same as bool (xor) 1
            fprintf(fout,
                    IS_ABSOLUTE(tac->op1) ? "\tmov\t$0x%s, %%al\n" : "\tmov\t_%s, %%al\n",
                    PARSE_BOOLEAN_TO_STRING(tac->op1));
            fprintf(fout,
                    "\tmov\t$1, %%bl\n"
                    "\txor\t%%bl, %%al\n"
                    "\tmov\t%%al, _%s\n\n",
                    tac->res->text);
            break;
        case TAC_JUMP:
            fprintf(fout, "\t# TAC_JUMP\n");
            fprintf(fout, "\tjmp\t_%s\n\n", tac->res->text);
            break;
        case TAC_JUMPFALSE:
            fprintf(fout, "\t# TAC_JUMPFALSE\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), PARSE_BOOLEAN_TO_STRING(tac->op1));
            fprintf(fout,
                    "\tmovl\t$1, %%ebx\n"
                    "\tcmpl\t%%eax, %%ebx\n");
            fprintf(fout, "\tjne\t_%s\n\n", tac->res->text);
            break;
        case TAC_RETURN:
            fprintf(fout, "\t# TAC_RETURN\n");

            fprintf(fout, MOVL_ABSOLUTE_OR_VARIABLE_TO_EAX(tac->op1), PARSE_BOOLEAN_TO_STRING(tac->op1));
            fprintf(fout,
                    "\tleave\n"
                    "\tret\n\n");
            break;
        case TAC_PRINT:
            fprintf(fout, "\t# TAC_PRINT\n");

            int print_int = TRUE;

            // Checa se é um inteiro, ou uma string, ou uma variável pra printar
            if (tac->op1->type == LIT_STRING)
            {
                char *escaped_str = escaped_string(tac->op1->text);
                fprintf(fout, "\tmovl\t$__%s_string_r4nd0m, %%eax\n", escaped_str);
                print_int = FALSE;
            }
            else if (tac->op1->type == LIT_FALSE || tac->op1->type == LIT_TRUE)
            {
                fprintf(fout, "\tmovl\t$%s, %%eax\n", PARSE_BOOLEAN_TO_STRING(tac->op1));
            }
            else if (tac->op1->type != TK_IDENTIFIER && tac->op1->type != TYPE_TMP)
            {
                fprintf(fout, "\tmovl\t$0x%d, %%eax\n", (int)strtol(tac->op1->text, NULL, 16));
            }
            else
            {
                fprintf(fout, "\tmovl\t_%s, %%eax\n", tac->op1->text);
            }

            fprintf(fout, "\tmovl\t%%eax, 4(%%esp)\n");
            fprintf(fout,
                    print_int == TRUE
                        ? "\tmovl\t$printint, (%%esp)\n"
                        : "\tmovl\t$printstring, (%%esp)\n");
            fprintf(fout, "\tcall\t_printf\n\n");
            break;
        default:
            break;
        }

        tac = tac->next;
        fflush(fout);
    }

    fclose(fout);
    return;
}
