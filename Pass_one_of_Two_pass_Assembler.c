#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main() {
  FILE * f1, * f2, * f3, * f4;
  char s[100], lab[30], opcode[30], opa[30], opcode1[30], opa1[30];
  int locctr=0x0, x = 0, start = 0, length = 0;

  f1 = fopen("input.txt", "r");
  f2 = fopen("opcode.txt", "r");
  f3 = fopen("output.txt", "w");
  f4 = fopen("symtab.txt", "w");

  if (f1 == NULL || f2 == NULL || f3 == NULL || f4 == NULL) {
    printf("Error opening files.\n");
    exit(1);
  }

  while (fscanf(f1, "%s%s%s", lab, opcode, opa) != EOF) {
    if (strcmp(lab, "**") == 0) {
      if (strcmp(opcode, "START") == 0) {
        fprintf(f3, "%s %s %s", lab, opcode, opa);
        locctr = atoi(opa);
        start = locctr;
      } else {
        rewind(f2);
        x = 0;
        while (fscanf(f2, "%s%s", opcode1, opa1) != EOF) {
          if (strcmp(opcode, opcode1) == 0)
            x = 1;
        }
        if (x == 1) {
          fprintf(f3, "\n%d %s %s %s", locctr, lab, opcode, opa);
          locctr += 3;
        }
      }
    } else {
      fprintf(f3, "\n%d %s %s %s", locctr, lab, opcode, opa);
      fprintf(f4, "%d %s\n", locctr, lab);

      if (strcmp(opcode, "RESW") == 0) {
        locctr += 3 * atoi(opa);
      } else if (strcmp(opcode, "WORD") == 0) {
        locctr += 3;
      } else if (strcmp(opcode, "BYTE") == 0) {
        if (opa[0] == 'C' && opa[1] == '\'') {
          int len = 0;
          for (int i = 2; opa[i] != '\'' && opa[i] != '\0'; i++)
            len++;
          locctr += len;
        } else if (opa[0] == 'X' && opa[1] == '\'') {
          int len = 0;
          for (int i = 2; opa[i] != '\'' && opa[i] != '\0'; i++)
            len++;
          locctr += (len + 1) / 2;
        } else {
          locctr += 1;
        }
      } else if (strcmp(opcode, "RESB") == 0) {
        locctr += atoi(opa);
      } else {
        locctr += 3;
      }
    }
  }

  length = locctr - start;
  printf("\nProgram Length: %d\n", length);

  fclose(f1);
  fclose(f2);
  fclose(f3);
  fclose(f4);

  printf("\n--- output.txt ---\n");
  f3 = fopen("output.txt", "r");
  while (fgets(s, sizeof(s), f3) != NULL) 
    printf("%s", s);
  fclose(f3);

  printf("\n\n--- symtab.txt ---\n");
  f4 = fopen("symtab.txt", "r");
  while (fgets(s, sizeof(s), f4) != NULL) 
    printf("%s", s);
  fclose(f4);
}
