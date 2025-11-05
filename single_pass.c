#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    FILE *f_input, *f_opcode, *f_inter, *f_symtab, *f_obj, *f_list;
    char label[64], opcode[64], operand[128], prog_name[32] = "      ";
    int locctr = 0, start_addr = 0, length = 0;

    // PASS 1
    f_input = fopen("input.txt", "r"), f_opcode = fopen("opcode.txt", "r");
    f_inter = fopen("output.txt", "w"), f_symtab = fopen("symtab.txt", "w");

    fscanf(f_input, "%s %s %s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        sscanf(operand, "%x", &start_addr);
        strncpy(prog_name, label, 6);
        prog_name[6] = '\0';
        fprintf(f_inter, "\t%s\t%s\t%s", label, opcode, operand);
        fscanf(f_input, "%s %s %s", label, opcode, operand);
    }
    locctr = start_addr;

    while (strcmp(opcode, "END") != 0) {
        fprintf(f_inter, "\n%X\t%s\t%s\t%s", locctr, label, opcode, operand);
        if (strcmp(label, "-") != 0) fprintf(f_symtab, "%X\t%s\n", locctr, label);
        
        char op[64], val[64]; int found = 0; rewind(f_opcode);
        while (fscanf(f_opcode, "%s %s", op, val) != EOF) if (strcmp(opcode, op) == 0) { found = 1; break; }

        if (found) locctr += 3;
        else if (strcmp(opcode, "WORD") == 0) locctr += 3;
        else if (strcmp(opcode, "RESW") == 0) locctr += 3 * atoi(operand);
        else if (strcmp(opcode, "RESB") == 0) locctr += atoi(operand);
        else if (strcmp(opcode, "BYTE") == 0) locctr += (operand[0] == 'X') ? (strlen(operand) - 3) / 2 : strlen(operand) - 3;
        fscanf(f_input, "%s %s %s", label, opcode, operand);
    }
    fprintf(f_inter, "\n\t%s\t%s\t%s", label, opcode, operand);
    length = locctr - start_addr;
    fclose(f_input); fclose(f_opcode); fclose(f_inter); fclose(f_symtab);

    // PASS 2
    f_inter = fopen("output.txt", "r"), f_opcode = fopen("opcode.txt", "r");
    f_symtab = fopen("symtab.txt", "r"), f_obj = fopen("objprog.txt", "w");
    f_list = fopen("twoout.txt", "w");

    char code[64], m_hex[64], sym[64], obj_code[512], txt_buf[1024] = "";
    int loc, sym_addr, txt_len = 0, txt_start = 0;
    
    printf("\nProgram Length: %X\n", length);
    fprintf(f_obj, "H^%-6s^%06X^%06X\n", prog_name, start_addr, length);
    printf("H^%-6s^%06X^%06X\n", prog_name, start_addr, length);

    char line[256]; fgets(line, sizeof(line), f_inter); fprintf(f_list, "%s", line);

    while (fscanf(f_inter, "%x %s %s %s", &loc, label, opcode, operand) == 4 && strcmp(opcode, "END") != 0) {
        obj_code[0] = '\0';
        rewind(f_opcode); int op_found = 0;
        while (fscanf(f_opcode, "%s %s", code, m_hex) != EOF) {
            if (strcmp(opcode, code) == 0) {
                rewind(f_symtab); int sym_found = 0, cur_sym_addr = 0;
                while (fscanf(f_symtab, "%x %s", &sym_addr, sym) != EOF) if (strcmp(operand, sym) == 0) { sym_found = 1; cur_sym_addr = sym_addr; break; }
                sprintf(obj_code, "%s%04X", m_hex, sym_found ? cur_sym_addr : 0);
                op_found = 1; break;
            }
        }
        if (!op_found) {
            if (strcmp(opcode, "WORD") == 0) sprintf(obj_code, "%06X", atoi(operand));
            else if (strcmp(opcode, "BYTE") == 0) {
                if (operand[0] == 'C') for (int i = 2; i < strlen(operand) - 1; i++) sprintf(obj_code + strlen(obj_code), "%02X", operand[i]);
                else if (operand[0] == 'X') sscanf(operand, "X'%[^']'", obj_code);
            }
        }

        if ( (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0 || (txt_len > 0 && txt_len + strlen(obj_code) > 60)) && txt_len > 0) {
            fprintf(f_obj, "T^%06X^%02X^%s\n", txt_start, txt_len / 2, txt_buf);
            printf("T^%06X^%02X^%s\n", txt_start, txt_len / 2, txt_buf);
            txt_len = 0; txt_buf[0] = '\0';
        }

        if (strlen(obj_code) > 0) {
            if (txt_len == 0) { txt_start = loc; strcpy(txt_buf, obj_code); } 
            else { strcat(txt_buf, "^"); strcat(txt_buf, obj_code); }
            txt_len += strlen(obj_code);
        }
        fprintf(f_list, "%-8X %-8s %-8s %-8s %-10s\n", loc, label, opcode, operand, obj_code);
    }
    
    if (txt_len > 0) {
        fprintf(f_obj, "T^%06X^%02X^%s\n", txt_start, txt_len / 2, txt_buf);
        printf("T^%06X^%02X^%s\n", txt_start, txt_len / 2, txt_buf);
    }
    fprintf(f_obj, "E^%06X\n", start_addr);
    printf("E^%06X\n", start_addr);

    fclose(f_inter); fclose(f_opcode); fclose(f_symtab); fclose(f_obj); fclose(f_list);
    printf("\nPass 2 done: objprog.txt and twoout.txt generated.\n");
    return 0;
}
