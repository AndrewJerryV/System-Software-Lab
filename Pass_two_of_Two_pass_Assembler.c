#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    char opcode[20], operand[20], label[20], code[20], mnemonic_hex[25];
    char symbol[20], object_code[20];
    int locctr, start_addr, prog_len, sym_addr, flag, flag1;

    char text_buffer[70], temp_buffer[20], prog_name[20];
    int text_len = 0, text_start_addr = 0;

    FILE *fp_inter, *fp_obj, *fp_list, *fp_optab, *fp_symtab;

    fp_inter = fopen("output.txt", "r");    
    fp_optab = fopen("opcode.txt", "r");    
    fp_symtab = fopen("symtab.txt", "r");   
    fp_obj = fopen("objprog.txt", "w");     
    fp_list = fopen("twoout.txt", "w");     

    if (!fp_inter || !fp_optab || !fp_symtab || !fp_obj || !fp_list) {
        printf("Error: Could not open one or more files.\n");
        return 1;
    }

    fscanf(fp_inter, "%s %s %s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {

        strncpy(prog_name, label, 6);
        prog_name[6] = '\0'; 
        sscanf(operand, "%x", &start_addr); 
    } else {
        strcpy(prog_name, "      ");
        start_addr = 0;
    }

    int last_locctr;
    char line_buffer[100];
    fseek(fp_inter, 0, SEEK_SET); 
    while (fgets(line_buffer, sizeof(line_buffer), fp_inter) != NULL) {
        sscanf(line_buffer, "%x", &last_locctr);
    }
    prog_len = last_locctr - start_addr;

    fprintf(fp_obj, "H^%-6s^%06X^%06X\n", prog_name, start_addr, prog_len);
    printf("H^%-6s^%06X^%06X\n", prog_name, start_addr, prog_len);

    fseek(fp_inter, 0, SEEK_SET);

    fscanf(fp_inter, "%s %s %s", label, opcode, operand);
    fprintf(fp_list, "%-8s %-8s %-8s\n", label, opcode, operand);

    fscanf(fp_inter, "%x %s %s %s", &locctr, label, opcode, operand);
    text_start_addr = locctr; 

    while (strcmp(opcode, "END") != 0) {
        object_code[0] = '\0'; 

        flag = 0;
        rewind(fp_optab);
        while (fscanf(fp_optab, "%s %s", code, mnemonic_hex) != EOF) {
            if (strcmp(opcode, code) == 0) {
                flag = 1;
                break;
            }
        }

        if (flag == 1) { 
            flag1 = 0;
            rewind(fp_symtab);
            while (fscanf(fp_symtab, "%x %s", &sym_addr, symbol) != EOF) {
                if (strcmp(operand, symbol) == 0) {
                    flag1 = 1;
                    break;
                }
            }
            if (flag1 == 1) {
                sprintf(object_code, "%s%04X", mnemonic_hex, sym_addr);
            } else {

                strcpy(object_code, "000000"); 
            }
        } else if (strcmp(opcode, "WORD") == 0) {
            sprintf(object_code, "%06X", atoi(operand));
        } else if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'C') { 
                for (int i = 2; i < strlen(operand) - 1; i++) {
                    sprintf(temp_buffer, "%X", operand[i]); 
                    strcat(object_code, temp_buffer);
                }
            } else if (operand[0] == 'X') { 
                strncpy(object_code, operand + 2, strlen(operand) - 3);
                object_code[strlen(operand) - 3] = '\0';
            }
        }

        if (strlen(object_code) > 0) {

            if (text_len + strlen(object_code) > 60) {
                fprintf(fp_obj, "T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
                printf("T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
                text_buffer[0] = '\0';
                text_len = 0;
                text_start_addr = locctr;
            }

            strcat(text_buffer, object_code);
            text_len += strlen(object_code);
        } else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {

            if (text_len > 0) {
                fprintf(fp_obj, "T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
                printf("T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
            }
            text_buffer[0] = '\0';
            text_len = 0;

            fscanf(fp_inter, "%x %s %s %s", &locctr, label, opcode, operand);
            text_start_addr = locctr;
            continue; 
        }

        fprintf(fp_list, "%-8X %-8s %-8s %-8s %-10s\n", locctr, label, opcode, operand, object_code);

        fscanf(fp_inter, "%x %s %s %s", &locctr, label, opcode, operand);
    }

    if (text_len > 0) {
        fprintf(fp_obj, "T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
        printf("T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
    }

    fprintf(fp_obj, "E^%06X\n", start_addr);
    printf("E^%06X\n", start_addr);

    fprintf(fp_list, "%-8X %-8s %-8s %-8s\n", locctr, label, opcode, operand);

    fclose(fp_inter);
    fclose(fp_optab);
    fclose(fp_symtab);
    fclose(fp_obj);
    fclose(fp_list);

    return 0;
}
