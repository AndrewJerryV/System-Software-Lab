#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char opcode[20], operand[20], symbol[20], label[20];
    char code[20], mnemonic[25], objectcode[50];
    int flag, flag1, locctr, loc;
    FILE *fp1, *fp2, *fp3, *fp4;

    fp1 = fopen("output.txt", "r");
    fp2 = fopen("twoout.txt", "w");
    fp3 = fopen("opcode.txt", "r");
    fp4 = fopen("symtab.txt", "r");

    fscanf(fp1, "%s %s %s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        fprintf(fp2, "%-8s %-8s %-8s\n", label, opcode, operand);
        fscanf(fp1, "%d %s %s %s", &locctr, label, opcode, operand);
    }

    while (strcmp(opcode, "END") != 0) {
        flag = 0;
        rewind(fp3);
        while (fscanf(fp3, "%s %s", code, mnemonic) != EOF) {
            if (strcmp(code, "END") == 0) break;
            if (strcmp(opcode, code) == 0) {
                flag = 1;
                break;
            }
        }

        if (flag == 1) {  // found in OPTAB
            flag1 = 0;
            rewind(fp4);
            while (fscanf(fp4, "%d %s", &loc, symbol) != EOF) {
                if (strcmp(symbol, operand) == 0) {
                    flag1 = 1;
                    break;
                }
            }
            if (flag1 == 1) 
                sprintf(objectcode, "%s%d", mnemonic, loc); // opcode + DECIMAL address
            else 
                strcpy(objectcode, "");
        }
        else if (strcmp(opcode, "WORD") == 0) {
            sprintf(objectcode, "%d", atoi(operand));   // decimal constant
        }
        else if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'C') {
                int i;
                char temp[10];
                objectcode[0] = '\0';
                for (i = 2; i < strlen(operand) - 1; i++) {
                    sprintf(temp, "%d", operand[i]);
                    strcat(objectcode, temp);
                }
            } else if (operand[0] == 'X') {
                strncpy(objectcode, operand + 2, strlen(operand) - 3);
                objectcode[strlen(operand) - 3] = '\0';
            }
        }
        else 
            strcpy(objectcode, "");
        fprintf(fp2, "%-8s %-8s %-8s %-6d %-10s\n",
                label, opcode, operand, locctr, objectcode);

        fscanf(fp1, "%d %s %s %s", &locctr, label, opcode, operand);
    }

    fprintf(fp2, "%-8s %-8s %-8s %-6d\n", label, opcode, operand, locctr);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    return 0;
}
