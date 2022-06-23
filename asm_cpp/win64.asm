
.data


.CODE  ;

vt_Add PROC  ;
    add rcx,rdx		;
    mov rax,rcx		;
    ret				;
vt_Add ENDP  ;


asm_win64_add PROC  ;
    
    push rbp
    sub rsp,30h ; params is rcx,rdx,r8,r9,rsp+20,rsp+28..

    mov         qword ptr [rsp+18h],r8
    mov         qword ptr [rsp+10h],rdx
    mov         qword ptr [rsp+8],rcx

    add rcx,rdx
    mov rax,rcx

    mov qword ptr[r8],20h


    add rsp,30h
    pop rbp
    ret

asm_win64_add ENDP  ;


asm_win64_call PROC  ;
    
    push rbp
    sub rsp,30h ; 

    mov         qword ptr [rsp+8],rcx

    call qword ptr [rsp+8h]

    add rsp,30h
    pop rbp
    ret

asm_win64_call ENDP  ;


END     ;