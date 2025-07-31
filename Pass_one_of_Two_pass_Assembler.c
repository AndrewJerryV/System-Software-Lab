#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main() {
  FILE *f1, *f2, *f3, *f4;
  char s[100], lab[30], opcode[30], opa[30], opcode1[30], opa1[30];
  int locctr, x = 0;
  f1 = fopen("input.txt", "r");
  f2 = fopen("opcode.txt", "r");
  f3 = fopen("out1.txt", "w");
  f4 = fopen("sym1.txt", "w");
  while (fscanf(f1, "%s%s%s", lab, opcode, opa) != EOF) {
    if (strcmp(lab, "**") == 0) {
      if (strcmp(opcode, "START") == 0) {
        fprintf(f3, "%s %s %s", lab, opcode, opa);
        locctr = (atoi(opa));
      } else {
        rewind(f2);
        x = 0;
        while (fscanf(f2, "%s%s", opcode1, opa1) != EOF) {
          if (strcmp(opcode, opcode1) == 0) 
            x = 1;
        }
        if (x == 1) {
          fprintf(f3, "\n %d %s %s %s", locctr, lab, opcode, opa);
          locctr = locctr + 3;
        }
      }
    } else {
      if (strcmp(opcode, "RESW") == 0) {
        fprintf(f3, "\n %d %s %s %s", locctr, lab, opcode, opa);
        fprintf(f4, "\n %d %s", locctr, lab);
        locctr = locctr + (3 * (atoi(opa)));
      } else if (strcmp(opcode, "WORD") == 0) {
        fprintf(f3, "\n %d %s %s %s", locctr, lab, opcode, opa);
        fprintf(f4, "\n %d %s", locctr, lab);
        locctr = locctr + 3;
      } else if (strcmp(opcode, "BYTE") == 0) {
        fprintf(f3, "\n %d %s %s %s", locctr, lab, opcode, opa);
        fprintf(f4, "\n %d %s", locctr, lab);
        locctr = locctr + 1;
      } else if (strcmp(opcode, "RESB") == 0) {
        fprintf(f3, "\n %d %s %s %s", locctr, lab, opcode, opa);
        fprintf(f4, "\n %d %s", locctr, lab);
        locctr = locctr + 1;
      } else {
        fprintf(f3, "\n %d %s %s %s", locctr, lab, opcode, opa);
        fprintf(f4, "\n %d %s", locctr, lab);
        locctr = locctr + (atoi(opa));
      }
    }
  }
}
