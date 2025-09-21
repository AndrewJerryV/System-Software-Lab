#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

    FILE *f1 = fopen("input.txt", "r");
    FILE *f2 = fopen("opcode.txt", "r");
    FILE *f3 = fopen("output.txt", "w");
    FILE *f4 = fopen("symtab.txt", "w");

    char s[256];
    char lab[64], opcode[64], opa[128];
    char opcode1[64], opa1[64];
    int locctr = 0, x = 0, start = 0, length = 0;

    while (fscanf(f1, "%s %s %s", lab, opcode, opa) != EOF) {
        if (strcmp(opcode, "END") == 0) {
            fprintf(f3, "\n\t%s\t%s\t%s", lab, opcode, opa);
            break;
        }

        if (strcmp(opcode, "START") == 0) {
            fprintf(f3, "\t%s\t%s\t%s", lab, opcode, opa);
            sscanf(opa, "%x", &locctr);
            start = locctr;
        } 
		else {
            rewind(f2);
            x = 0;
            while (fscanf(f2, "%s %s", opcode1, opa1) != EOF) {
                if (strcmp(opcode, opcode1) == 0) {
                    x = 1;
                    break;
                }
            }
            if (x == 1) {
                fprintf(f3, "\n%X\t%s\t%s\t%s", locctr, lab, opcode, opa);
                locctr += 3;
            } else {
                fprintf(f3, "\n%X\t%s\t%s\t%s", locctr, lab, opcode, opa);

                if (strcmp(lab, "-") != 0 && strcmp(lab, "") != 0) {
                    fprintf(f4, "%X\t%s\n", locctr, lab);
                }

                if (strcmp(opcode, "RESW") == 0)
                    locctr += 3 * atoi(opa);
				else if (strcmp(opcode, "WORD") == 0)
                    locctr += 3;
				else if (strcmp(opcode, "BYTE") == 0) {
                    if (opa[0] == 'C' && opa[1] == '\'') {
                        int len = 0;
                        for (int i = 2; opa[i] != '\'' && opa[i] != '\0'; i++) len++;
                        locctr += len;
                    } 
					else if (opa[0] == 'X' && opa[1] == '\'') {
                        int len = 0;
                        for (int i = 2; opa[i] != '\'' && opa[i] != '\0'; i++) len++;
                        locctr += (len + 1) / 2;
                    } 
					else 
                        locctr += 1;
                } else if (strcmp(opcode, "RESB") == 0) {
                    locctr += atoi(opa);
                } 
				else 
                    locctr += 3;
            }
        }
    }

    length = locctr - start;
    printf("\nProgram Length: %X\n", length);

    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);

    printf("\n--- output.txt ---\n");
    f3 = fopen("output.txt", "r");
    if (f3) {
        while (fgets(s, sizeof(s), f3) != NULL) 
			printf("%s", s);
        fclose(f3);
    }

    printf("\n\n--- symtab.txt ---\n");
    f4 = fopen("symtab.txt", "r");
    if (f4) {
        while (fgets(s, sizeof(s), f4) != NULL) 
			printf("%s", s);
        fclose(f4);
    }
	printf("\n\n--- Object code ---\n");

    FILE *fp1 = fopen("output.txt", "r");
    FILE *fp2 = fopen("opcode.txt", "r");
    FILE *fp3 = fopen("symtab.txt", "r");
    FILE *fp4 = fopen("objprog.txt", "w");
    FILE *fp5 = fopen("twoout.txt", "w");

    char label[64], operand[128], code[64], mnemonic_hex[64];
    char symbol[64], object_code[512];
    int loc, start_addr = 0, prog_len = 0, sym_addr;
    int flag, flag1;
    char text_buffer[1024], temp_buffer[64], prog_name[32];
    int text_len = 0, text_start_addr = 0;

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
    } else {
        strcpy(prog_name, "      ");
        start_addr = 0;
        fseek(fp1, 0, SEEK_SET);
    }

    int last_locctr = 0;
    char line_buffer[512];
    fseek(fp1, 0, SEEK_SET);
    while (fgets(line_buffer, sizeof(line_buffer), fp1) != NULL) {
        int tmp;
        if (sscanf(line_buffer, "%x", &tmp) == 1) last_locctr = tmp;
    }
    prog_len = last_locctr - start_addr;

    fprintf(fp4, "H^%-6s^%06X^%06X\n", prog_name, start_addr, prog_len);
    printf("H^%-6s^%06X^%06X\n", prog_name, start_addr, prog_len);

    fseek(fp1, 0, SEEK_SET);

    if (fscanf(fp1, "%s %s %s", label, opcode, operand) == 3) 
        fprintf(fp5, "%-8s %-8s %-8s\n", label, opcode, operand);
    else 
        fseek(fp1, 0, SEEK_SET);

    if (fscanf(fp1, "%x %s %s %s", &loc, label, opcode, operand) != 4) {
        fprintf(fp4, "E^%06X\n", start_addr);
        printf("E^%06X\n", start_addr);
        fclose(fp1); fclose(fp2); fclose(fp3); fclose(fp4); fclose(fp5);
        return 0;
    }

    text_start_addr = loc;
    text_buffer[0] = '\0';
    text_len = 0;

    while (1) {
        if (strcmp(opcode, "END") == 0)
            break;

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
            } 
			else if (operand[0] == 'X' && operand[1] == '\'') {
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
                text_start_addr = loc;
            }
            if (text_len == 0) {
				text_start_addr = loc;
				strcpy(text_buffer, object_code);
			} else {
				strcat(text_buffer, "^");        
				strcat(text_buffer, object_code);
			}
text_len += strlen(object_code);
        } else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
            if (text_len > 0) {
                fprintf(fp4, "T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
                printf("T^%06X^%02X^%s\n", text_start_addr, text_len / 2, text_buffer);
                text_buffer[0] = '\0';
                text_len = 0;
            }
        }
        fprintf(fp5, "%-8X %-8s %-8s %-8s %-10s\n", loc, label, opcode, operand, object_code);
        if (fscanf(fp1, "%x %s %s %s", &loc, label, opcode, operand) != 4) 
            break;
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

    printf("\nPass2 done: objprog.txt and twoout.txt generated.\n");
    return 0;
}
