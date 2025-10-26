DATA SEGMENT
    M1 DB 10,13,"Enter first number: $"
    M2 DB 10,13,"Enter second number: $"
    M3 DB 10,13,"Product: $"
DATA ENDS

PRTMSG MACRO MESSAGE
    LEA DX, MESSAGE
    MOV AH, 09H
    INT 21H
ENDM

GETDCM MACRO
    MOV AH, 01H
    INT 21H
    SUB AL, 30H
ENDM

CODE SEGMENT
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX

    PRTMSG M1
    GETDCM
    MOV BL, AL

    PRTMSG M2
    GETDCM

    MOV AH, 00H
    MUL BL
    AAM
    MOV BX, AX

    PRTMSG M3

    MOV DL, BH
    ADD DL, 30H
    MOV AH, 02H
    INT 21H

    MOV DL, BL
    ADD DL, 30H
    INT 21H

    MOV AH, 4CH
    INT 21H
CODE ENDS

END START
