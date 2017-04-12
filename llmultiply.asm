;
;  Test cases. All numbers are in hex.
;
;      111122223333 *     555566667777 =          5B061D958BF0ECA7C0481B5
;  3456FEDCAAAA1000 * EDBA00112233FF01 = 309A912AF7188C57E62072DD409A1000
;  FFFFEEEEDDDDCCCC * BBBBAAAA99998888 = BBBB9E2692C5DDDCC28F7531048D2C60
;  FFFFFFFFFFFFFFFF * FFFFFFFFFFFFFFFF = FFFFFFFFFFFFFFFE0000000000000001
;  00000001FFFFFFFF * 00000001FFFFFFFF = 0000000000000003FFFFFFFC00000001
;  FFFEFFFFFFFFFFFF * FFFF0001FFFFFFFF = FFFE0002FFFDFFFE0001FFFE00000001
;

;  void llmultiply(unsigned long long int l1,
;                  unsigned long long int l2,
;                  unsigned char *result);
        
;  a = AH * (2 << 32) + AL
;  b = BH * (2 << 32) + BL
;        
;  a * b = (AH * BH            * (2 << 64)) +
;          (AH * BL + AL * BH) * (2 << 32)  +
;           AL * BL

; For more infromation, see the lab manual!
        
	SECTION .data



	SECTION .text
	ALIGN	16
	BITS	32

	;; To access parameter BH on the stack you do
	;;   MOV EAX, [EBP + 20]
	;; Replace the zero below with the correct value to be able
	;; to write your assembly code a little bit more readable: 
	;;   MOV EAX, [EBP + BH_OFF]
AL_OFF	EQU     8	; Offset from EBP to low  bits of a (AL)
AH_OFF	EQU     12	; Offset from EBP to high bits of a (AH)
BL_OFF	EQU     16	; Offset from EBP to low  bits of b (BL)
BH_OFF	EQU     20	; Offset from EBP to high bits of b (BH)
RES_OFF	EQU     24	; Offset from EBP to result array pointer
	;;    ^^^^^ Replace 0 with correct values above
        
	GLOBAL llmultiply

llmultiply:
	PUSH EBP
	MOV EBP, ESP

	;; Put your implementation here
	PUSH EAX 
	PUSH EBX 
	PUSH ECX 
	PUSH EDX

	PUSH EDI
	PUSH ESI

	MOV EDI, 0
	MOV ECX, 0
	MOV EBX, 0
	MOV ESI, 0

	MOV EAX, [EBP + AL_OFF] 
	MOV EDX, [EBP + BL_OFF]
	MUL EDX ; al * bl   EAX = low EDX = high

	MOV EDI, EAX  
	MOV ECX, EDX

	MOV EAX, [EBP + AL_OFF]
	MOV EDX, [EBP + BH_OFF]
	MUL EDX ; al * bh

	ADD ECX, EAX
	ADC EBX, EDX
	ADC ESI, 0

	MOV EAX, [EBP + AH_OFF]
	MOV EDX, [EBP + BL_OFF]
	MUL EDX ; ah * bl
	
	ADD ECX, EAX
	ADC EBX, EDX
	ADC ESI, 0

	MOV EAX, [EBP + AH_OFF]
	MOV EDX, [EBP + BH_OFF]
	MUL EDX ; ah * bh

	ADD EBX, EAX
	ADC ESI, EDX

	MOV EAX, [EBP + RES_OFF]
	MOV [EAX], EDI
	MOV [EAX + 4], ECX
	MOV [EAX + 8], EBX
	MOV [EAX + 12], ESI

	POP EAX 
	POP EBX 
	POP ECX 
	POP EDX

	POP EDI
	POP ESI

	POP EBP				; restore EBP reg
	RET				;  return
