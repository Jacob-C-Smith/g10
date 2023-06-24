;	x86 calling conventions
;	rcx/xmm0
;	rdx/xmm1
;	r8/xmm2
;	r9/xmm3
;	RTL (C)
;
;	Stack aligned on 16 bytes. 32 bytes shadow space on stack.
;	The specified 8 registers can only be used for parameters
;	1 through 4. For C++ classes, the hidden this parameter
;	is the first parameter, and is passed in RCX.
;

_DATA SEGMENT

_DATA ENDS

_CONST SEGMENT
all_minus_1:
    real4 -0.f
    real4 -0.f
    real4 -0.0f
    real4 -0.f
_CONST ENDS

_TEXT SEGMENT

; Transposes inverse matrix
PUBLIC AVX2GJKLine
AVX2GJKLine PROC
    vmovaps xmm2, xmmword ptr [all_minus_1] ; xmm2 =    [ -1 -1 -1 _ ]
    vmovaps xmm0, [rcx]                     ; xmm0 =  A [  x  y  z _ ]
    vmovaps xmm1, [rcx + 8]                 ; xmm1 =  B [  x  y  z _ ]
    vmulps  xmm2, xmm2, xmm0                ; xmm2 = -A [ -x -y -z _ ]
    vaddps  xmm3, xmm1, xmm2                ; xmm3 = B - A
    
    ; TODO:
    ;                    ; if same_direction(xmm3, xmm2)
    ; jz .same_direction_return_zero
    vmovaps xmm0, xmm3   ; xmm0 <- B - A
    vmovaps xmm1, xmm2   ; xmm1 <- -A
    ;                    ; xmm1 <- avx2_cross_product(xmm0, xmm1)
    ;                    ; xmm2 <- avx2_cross_product(xmm1, xmm3)
                         ; <- xmm2
    jmp exit
    
    same_direction_return_zero:
    ; points <- (vec3*)(rcx); points[2]; points[1] = 0x0; points_count--;
    ; rdx = xmm2
    jmp exit

    exit:

    mov xmmwrod ptr[rdx], xmm2 ; Return the direction to the caller

    xor rax, rax ; rax = 0 (false)
    leave        ; return rax
    ret
AVX2GJKLine ENDP

_TEXT ENDS

END
