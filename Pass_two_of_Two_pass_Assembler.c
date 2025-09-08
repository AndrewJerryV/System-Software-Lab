#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char opcode[20], operand[20], label[20], code[20], mnemonic_hex[25];
    char symbol[20], object_code[100];
    int locctr, start_addr = 0, prog_len = 0, sym_addr, flag, flag1;
    char text_buffer[200], temp_buffer[20], prog_name[20];
    int text_len = 0, text_start_addr = 0;

    FILE *fp1, *fp4, *fp5, *fp2, *fp3;
    fp1 = fopen("output.txt", "r");
    fp2 = fopen("opcode.txt", "r");
    fp3 = fopen("symtab.txt", "r");
    fp4 = fopen("objprog.txt", "w");
    fp5 = fopen("twoout.txt", "w");

    if (fscanf(fp1, "%s %s %s", label, opcode, operand) == 3) {
        if (strcmp(opcode, "START") == 0) {
            strncpy(prog_name, label, 6);
            prog_name[6] = '\0';
            sscanf(operand, "%x", &start_addr);
        } else {
            strcpy(prog_name, "      ");
            start_addr = 0;
            fseek(fp1, 0, SEEK_SET);
        }
    }

    int last_locctr = 0;
    char line_buffer[256];
    fseek(fp1, 0, SEEK_SET);
    while (fgets(line_buffer, sizeof(line_buffer), fp1) != NULL) {
        int tmp;
        if (sscanf(line_buffer, "%x", &tmp) == 1)
            last_locctr = tmp;
    }
    prog_len = last_locctr - start_addr;

    fprintf(fp4, "H^%-6s^%06X^%06X\n", prog_name, start_addr, prog_len);
    printf("H^%-6s^%06X^%06X\n", prog_name, start_addr, prog_len);

    fseek(fp1, 0, SEEK_SET);

    if (fscanf(fp1, "%s %s %s", label, opcode, operand) == 3) {
        fprintf(fp5, "%-8s %-8s %-8s\n", label, opcode, operand);
    } 

    if (fscanf(fp1, "%x %s %s %s", &locctr, label, opcode, operand) != 4) {
        fprintf(fp4, "E^%06X\n", start_addr);
        printf("E^%06X\n", start_addr);
        fclose(fp1); fclose(fp2); fclose(fp3);
        fclose(fp4); fclose(fp5);
        return 0;
    }

    text_start_addr = locctr;
    text_buffer[0] = '\0';
    text_len = 0;

    while (1) {
        if (strcmp(opcode, "END") == 0) {
            break;
        }

        object_code[0] = '\0';
        flag = 0;
        rewind(fp2);
        while (fscanf(fp2, "%s %s", code, mnemonic_hex) != EOF) {
            if (strcmp(opcode, code) == 0) {
                flag = 1;
                break;
            }
        }

        if (flag == 1) {
            flag1 = 0;
            rewind(fp3);
            while (fscanf(fp3, "%x %s", &sym_addr, symbol) != EOF) {
                if (strcmp(operand, symbol) == 0) {
                    flag1 = 1;
                    break;
                }
            }
            if (flag1 == 1) 
                sprintf(object_code, "%s%04X", mnemonic_hex, sym_addr);
            else 
                strcpy(object_code, "000000");
        } 
        else if (strcmp(opcode, "WORD") == 0) {
            int val = atoi(operand);
            sprintf(object_code, "%06X", val);
        } 
        else if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'C' && operand[1] == '\'') {
                size_t len = strlen(operand);
                object_code[0] = '\0';
                for (int i = 2; i < (int)len - 1; i++) {
                    sprintf(temp_buffer, "%02X", (unsigned char)operand[i]);
                    strcat(object_code, temp_buffer);
                }
            } else if (operand[0] == 'X' && operand[1] == '\'') {
                size_t len = strlen(operand);
                size_t n = (len >= 3) ? len - 3 : 0;
                if (n > 0) {
                    strncpy(object_code, operand + 2, n);
                    object_code[n] = '\0';
                } 
                else 
                    object_code[0] = '\0';
            } 
            else 
                object_code[0] = '\0';
        } 
        else 
            object_code[0] = '\0';

        if (strlen(object_code) > 0) {
            if (text_len + (int)strlen(object_code) > 60) {
                fprintf(fp4, "T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
                printf("T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
                text_buffer[0] = '\0';
                text_len = 0;
                text_start_addr = locctr;
            }
            if (text_len == 0) {
                text_start_addr = locctr;
            }
            strcat(text_buffer, object_code);
            text_len += strlen(object_code);            
        } else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
            if (text_len > 0) {
                fprintf(fp4, "T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
                printf("T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
                text_buffer[0] = '\0';
                text_len = 0;
            }

        }
        fprintf(fp5, "%-8X %-8s %-8s %-8s %-10s\n", locctr, label, opcode, operand, object_code);
        if (fscanf(fp1, "%x %s %s %s", &locctr, label, opcode, operand) != 4) {
            break;
        }
    } 
    if (text_len > 0) {
        fprintf(fp4, "T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
        printf("T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
    }
    fprintf(fp4, "E^%06X\n", start_addr);
    printf("E^%06X\n", start_addr);

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    return 0;
}
