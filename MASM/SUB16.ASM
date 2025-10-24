ASSUME CS:CODE, DS:DATA
DATA SEGMENT
    M1 DB 10,13,"ENTER FIRST NO.: $"
    M2 DB 10,13,"ENTER SECOND NO.: $"
    M3 DB 10,13,"DIFFERENCE: $"
    DIFF DB 2 DUP(0) 
DATA ENDS

; MACRO TO PRINT A MESSAGE
PRTMSG MACRO MESSAGE
    LEA DX, MESSAGE
    MOV AH, 09H
    INT 21H
ENDM

; MACRO TO GET A DECIMAL DIGIT
GETDCM MACRO
    MOV AH, 01H
    INT 21H
    SUB AL, 30H 
ENDM

; MACRO TO PRINT A DECIMAL DIGIT FROM MEMORY
PRTDCM MACRO
    MOV DL, [SI] 
    ADD DL, 30H  
    MOV AH, 02H
    INT 21H
ENDM

CODE SEGMENT
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

    MOV AL, BL      
    SUB AL, CL      
    MOV AH, 00H     
    AAS             
    
    LEA SI, DIFF    
    MOV [SI], AL    

    MOV AL, BH      
    ADD AL, AH      
    SUB AL, CH      
    MOV AH, 00H     
    AAS             
    
    INC SI          
    MOV [SI], AL    

    PRTMSG M3
    PRTDCM          
    DEC SI          
    PRTDCM          
    MOV AH, 4CH
    INT 21H
CODE ENDS
END START
