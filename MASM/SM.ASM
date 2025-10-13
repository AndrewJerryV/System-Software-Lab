; DATA SEGMENT
DATA SEGMENT
    M1 DB 10,13,"ENTER STRING(DELIMITER: '`'): $"
    M2 DB 10,13,"NUMBER OF VOWELS: $"
    M3 DB 10,13,"NUMBER OF DIGITS: $"
    M4 DB 10,13,"NUMBER OF CONSONANTS: $"
    INSTR DB 20 DUP(0)
    MAXLEN DB 20
    DELIM DB '`'
    VCNT DB 00H
    DGCNT DB 00H
    CNCNT DB 00H
DATA ENDS

; EXTRA SEGMENT
EXTRA SEGMENT
    VWSTR DB "aeiouAEIOU"
    DGSTR DB "0123456789"
EXTRA ENDS

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

; CODE SEGMENT
CODE SEGMENT
    ASSUME CS:CODE, DS:DATA, ES:EXTRA
START:
    MOV AX, DATA
    MOV DS, AX
    MOV AX, EXTRA
    MOV ES, AX

    LEA SI, INSTR
    PRTMSG M1
    MOV BX, 00
    MOV CH, 00H
    MOV CL, MAXLEN
    MOV AH, 01H
GETC:
    INT 21H
    CMP AL, DELIM
    JE ENDGET
    INC BL
    MOV [SI], AL
    INC SI
    LOOP GETC

ENDGET:
    CLD
    LEA SI, INSTR
    MOV CH, 00H
    MOV CL, BL
    JCXZ DISPLAY_RESULTS
CHKA:
    MOV AL, [SI]

    ; Check for Vowel
    LEA DI, VWSTR
    PUSHF
    PUSH CX
    MOV CX, 10
    REPNZ SCASB
    ; BUG FIX 1: Check the JNE *before* restoring flags
    JNE CHKD_NO_VOWEL
    POP CX
    POPF
    INC VCNT
    JMP ENDC
CHKD_NO_VOWEL:
    POP CX
    POPF

CHKD:
    ; Check for Digit
    LEA DI, DGSTR
    PUSHF
    PUSH CX
    MOV CX, 10
    REPNZ SCASB
    ; BUG FIX 1: Check the JNE *before* restoring flags
    JNE CHKC_NO_DIGIT
    POP CX
    POPF
    INC DGCNT
    JMP ENDC
CHKC_NO_DIGIT:
    POP CX
    POPF

CHKC:
    ; BUG FIX 2: Add a check to ignore spaces
    CMP AL, ' '
    JE ENDC ; If it's a space, skip to the next character
    INC CNCNT

ENDC:
    INC SI
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