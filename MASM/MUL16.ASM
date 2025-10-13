; DATA SEGMENT
DATA SEGMENT
    M1 DB 10,13,"ENTER FIRST NUMBER: $"
    M2 DB 10,13,"ENTER SECOND NUMBER: $"
    M3 DB 10,13,"PRODUCT: $"
    PROD DB 4 DUP(00H)
DATA ENDS

; MACRO DEFINITIONS
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

PRTDCM MACRO
    MOV DL, [SI]
    ADD DL, 30H
    MOV AH, 02H
    INT 21H
ENDM

; CODE SEGMENT
CODE SEGMENT
    ASSUME CS:CODE, DS:DATA
START:
    MOV AX, DATA
    MOV DS, AX

    PRTMSG M1
    GETDCM
    MOV BH, AL
    GETDCM
    MOV BL, AL

    PRTMSG M2
    GETDCM
    MOV CH, AL
    GETDCM
    MOV CL, AL

    LEA SI, PROD
    MOV AL, CL
    MOV AH, 00H
    MUL BL
    AAM
    MOV [SI], AL
    INC SI
    MOV [SI], AH

    MOV AH, 00H
    MOV AL, BH
    MUL CL
    AAM
    MOV DX, AX
    ADD DL, [SI]

    MOV AH, 00H
    MOV AL, CH
    MUL BL
    AAM
    ADD DX, AX
    MOV AL, DL
    MOV AH, 00H
    AAM
    ADD DH, AH
    MOV [SI], AL
    MOV DL, DH
    MOV DH, 00H

    INC SI
    MOV AH, 00H
    MOV AL, BH
    MUL CH
    AAM
    ADD DX, AX
    MOV AL, DL
    MOV AH, 00H
    AAM
    MOV [SI], AL
    ADD DH, AH
    INC SI
    MOV AL, DH
    MOV [SI], AL

    PRTMSG M3
    PRTDCM
    DEC SI
    PRTDCM
    DEC SI
    PRTDCM
    DEC SI
    PRTDCM

    MOV AH, 4CH
    INT 21H
CODE ENDS
END START