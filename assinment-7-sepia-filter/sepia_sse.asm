%define b_values rdi
%define g_values rsi
%define r_values rdx
%define result rcx

%macro correct_args 0

add b_values, 4
add g_values, 4
add r_values, 4
add result, 4

%endmacro


c1_0: dd 0.272, 0.349, 0.393, 0.272
c1_1: dd 0.543, 0.686, 0.769, 0.543
c1_2: dd 0.131, 0.168, 0.189, 0.131

c2_0: dd 0.349, 0.393, 0.272, 0.349
c2_1: dd 0.686, 0.769, 0.543, 0.686
c2_2: dd 0.168, 0.189, 0.131, 0.168

c3_0: dd 0.393, 0.272, 0.349, 0.393
c3_1: dd 0.769, 0.543, 0.686, 0.769
c3_2: dd 0.189, 0.131, 0.168, 0.189

check: dd 255, 255, 255, 255

section .text
global sepia_asm

return:
	cvtps2dq xmm0, xmm0 ; from float to integer (32 bit)
	pminud xmm0, [check] ; check and swap -> min(xmm0, 255;
	
	; write value to result
	pextrb [result], xmm0, 0
	pextrb [result + 1], xmm0, 4
	pextrb [result + 2], xmm0, 8
	pextrb [result + 3], xmm0, 12

	ret

sepia_asm:

movdqu xmm3, [c1_0] ; into xmm3 -> [0-31] - c[0][0], [32-63] - c[2][0], [64-95] - c[1][0], [96-127] - c[0][0] 
movdqu xmm4, [c1_1] ; into xmm3 -> [0-31] - c[0][1], [32-63] - c[2][1], [64-95] - c[1][1], [96-127] - c[0][1] 
movdqu xmm5, [c1_2] ; into xmm3 -> [0-31] - c[0][2], [32-63] - c[2][2], [64-95] - c[1][2], [96-127] - c[0][2] 

movdqu xmm0, [b_values] ; b1, b2, b3, b4 into xmm0 -> [0-31] - b1, [32-63] - b2, [64-95] - b3, [96 - 127] - b4 
movdqu xmm1, [g_values] ; g1, g2, g3, g4 into xmm1 -> [0-31] - g1, [32-63] - g2, [64-95] - g3, [96 - 127] - g4
movdqu xmm2, [r_values] ; r1, r2, r3, r4 into xmm2 -> [0-31] - r1, [32-63] - r2, [64-95] - r3, [96 - 127] - r4

shufps xmm0, xmm0, 0x01 ; b1, b1, b1, b2 into xmm0 -> [127-96] - b1, [95-64] - b1, [63-32] - b1, [31-0] - b2; 0x01 - 00000001
shufps xmm1, xmm1, 0x01 ; g1, g1, g1, g2 into xmm1 -> [127-96] - g1, [95-64] - g1, [63-32] - g1, [31-0] - g2; 0x01 - 00000001
shufps xmm2, xmm2, 0x01 ; r1, r1, r1, r2 into xmm2 -> [127-96] - r1, [95-64] - r1, [63-32] - r1, [31-0] - r2; 0x01 - 00000001

mulps xmm0, xmm5 ; b2 * c[0][0], b1 * c[2][0], b1 * c[3][0], b1 * c[0][0]
mulps xmm1, xmm4 ; g2 * c[0][1], g1 * c[2][1], g1 * c[3][1], g1 * c[0][1]
mulps xmm2, xmm3 ; r2 * c[0][2], r1 * c[2][2], r1 * c[3][2], g1 * c[0][2]


                 ; b2 * c[0][0], b1 * c[2][0], b1 * c[3][0], b1 * c[0][0]
addps xmm0, xmm1 ;     +             +             +             +
                 ; g2 * c[0][1], g1 * c[2][1], g1 * c[3][1], g1 * c[0][1]
addps xmm0, xmm2 ;     +             +             +             +
                 ; r2 * c[0][2], r1 * c[2][2], r1 * c[3][2], g1 * c[0][2] 

call return
correct_args ; skip b1, g1, r1

movdqu xmm3, [c2_0]
movdqu xmm4, [c2_1]
movdqu xmm5, [c2_2]

movdqu xmm0, [b_values]
movdqu xmm1, [g_values]
movdqu xmm2, [r_values]

shufps xmm0, xmm0, 0x05
shufps xmm1, xmm1, 0x05
shufps xmm2, xmm2, 0x05

mulps xmm0, xmm5
mulps xmm1, xmm4
mulps xmm2, xmm3

addps xmm0, xmm1
addps xmm0, xmm2

call return
correct_args ; skip b2, g2, r2

movdqu xmm3, [c3_0]
movdqu xmm4, [c3_1]
movdqu xmm5, [c3_2]

movdqu xmm0, [b_values]
movdqu xmm1, [g_values]
movdqu xmm2, [r_values]

shufps xmm0, xmm0, 0x15
shufps xmm1, xmm1, 0x15
shufps xmm2, xmm2, 0x15

mulps xmm0, xmm5
mulps xmm1, xmm4
mulps xmm2, xmm3

addps xmm0, xmm1
addps xmm0, xmm2

call return
ret
