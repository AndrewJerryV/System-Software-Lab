ASSUME CS:CODE, DS:DATA, ES:DATA ; ES now points to DATA
DATA SEGMENT
    M1    DB 10,13,"ENTER STRING(DELIMITER: '`'): $"
    M2    DB 10,13,"NUMBER OF VOWELS: $"
    M3    DB 10,13,"NUMBER OF DIGITS: $"
    M4    DB 10,13,"NUMBER OF CONSONANTS: $"
    INSTR DB 20 DUP(0)
    DELIM DB '`'
    VCNT  DB 00H
    DGCNT DB 00H
    CNCNT DB 00H
    VWSTR DB "aeiouAEIOU" ; Moved from EXTRA
    DGSTR DB "0123456789" ; Moved from EXTRA
DATA ENDS

; MACRO TO PRINT A MESSAGE
PRTMSG MACRO MESSAGE
    LEA DX, MESSAGE
    MOV AH, 09H
    INT 21H
ENDM

; MACRO TO PRINT A COUNT
PRTCNT MACRO COUNT
    MOV DL, COUNT
    ADD DL, 30H
    MOV AH, 02H
    INT 21H
ENDM

CODE SEGMENT
START:
    MOV AX, DATA
    MOV DS, AX
    MOV ES, AX          

    PRTMSG M1
    LEA DI, INSTR
    XOR CX, CX          
    MOV AH, 01H
GETC:
    INT 21H
    CMP AL, DELIM
    JE PROCESS_STRING
    MOV [DI], AL
    INC DI
    INC CX
    CMP CX, 20          
    JNE GETC

PROCESS_STRING:
    CLD
    LEA SI, INSTR
    JCXZ DISPLAY_RESULTS 
CHKA:
    LODSB               

    ; Check for Vowel
    LEA DI, VWSTR
    PUSH CX             
    MOV CX, 10          
    REPNZ SCASB
    POP CX              
    JNE CHKD            
    INC VCNT
    JMP ENDC            

CHKD:
    ; Check for Digit
    LEA DI, DGSTR
    PUSH CX
    MOV CX, 10          
    REPNZ SCASB
    POP CX
    JNE CHKC            
    INC DGCNT
    JMP ENDC

CHKC:
    ; Check for Consonant
    CMP AL, ' '
    JE ENDC             
    INC CNCNT

ENDC:
    LOOP CHKA           

DISPLAY_RESULTS:
    PRTMSG M2
    PRTCNT VCNT
    PRTMSG M3
    PRTCNT DGCNT
    PRTMSG M4
    PRTCNT CNCNT

    MOV AH, 4CH
    INT 21H
CODE ENDS
END START
