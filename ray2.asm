%if 0
set -e
OUT=ray
#nasm -f elf32 $0 -o "${OUT}.o" -l nasm.list -Lw+
nasm -f elf32 $0 -o "${OUT}.o"
#ld -N -Ttext-segment=0x10000 --noinhibit-exec -z noseparate-code -m elf_i386 "${OUT}.o" -o "$OUT"
ld -T fbdev/ldscript -Ttext-segment=0x10000 --noinhibit-exec -m elf_i386 "${OUT}.o" -o "$OUT"
objdump --disassembler-color=on -Mintel -zdSl "$OUT"
ls -l "$OUT"
./fbdev/pack-ect.sh "$OUT"
exit 0
%endif

%use fp
BITS 32
extern _start
;org 0x10000


%assign STRIDE	1920
%assign WIDTH	600
%assign HEIGHT	600
%assign SAMPLES	512



%macro zero4 1
	xorps %1, %1
%endmacro

%macro dataref 2
	[section .rodata align=4]
	%%str:	dd %2
	__?SECT?__
	movss	%1, [%%str]
%endmacro

%macro dataref4 2
	[section .rodata align=4]
	%%str:	dd %2
		dd %2
		dd %2
		dd %2
	__?SECT?__
	movups	%1, [%%str]
%endmacro

%macro set4 2
%if 0
	dataref4	%1, %2
%else
	dataref		%1, %2
	shufps		%1, %1, 0
%endif
%endmacro

%macro set1 2
	dataref		%1, %2
%endmacro

%macro sc 3
	%ifnum %2
		%fatal not implemented
	%else
		[section .rodata align=4]
		%%str:	dd %3
		__?SECT?__
		%1      %2, [%%str]
	%endif
%endmacro

%macro cmulss 2
	sc	mulss, %1, %2
%endmacro

%macro flc 1
	%if %isnum(%1)
		%if %1 == 1
			fld1
		%else
			fc      fld, %1
		%endif
	%else
		fc      fld, %1
	%endif
%endmacro




%macro dotP 2
	dpps	%1, %2, 113
%endmacro

%macro dotP3 2
	dpps	%1, %2, 119
%endmacro

%if 0
	movaps	A, B
	dotP3	X, B
	sqrtps	X, X
	divps	B, X

	movaps	A, B
	dotP3	X, B
	sqrtps	X, X
	divps	A, X

	movaps	A, B
	dotP3	X, B
	sqrtps	X, X
	divps	B, X

	movaps	A, B
	dotP3	X, B
	sqrtps	X, X
	divps	B, X




	movaps	A, B
	dotP3	A, B
	sqrtps	A, A
	divps	B, A

	movaps	A, B
	dotP3	C, B
	sqrtps	C, C
	divps	A, C

	movaps	A, B
	dotP3	A, B
	sqrtps	A, A
	divps	B, A

	movaps	A, B
	dotP3	A, B
	sqrtps	A, A
	divps	B, A



	movaps	xmm0, xmm2	; tmp198, _33
	dotP3	xmm0, xmm2	; tmp198, _33,
	sqrtps	xmm0, xmm0	; tmp199, tmp198
	divps	xmm2, xmm0	; _36, tmp199

	movaps	xmm1, xmm0	; rand, rand
	dotP3	xmm3, xmm0	; tmp212, rand,
	sqrtps	xmm3, xmm3	; tmp213, tmp212
	divps	xmm1, xmm3	; rand, tmp213

	movaps	xmm0, xmm1	; tmp215, _43
	dotP3	xmm0, xmm1	; tmp215, _43,
	sqrtps	xmm0, xmm0	; tmp216, tmp215
	divps	xmm1, xmm0	; nextdir, tmp216

	movaps	xmm1, xmm0	; tmp159, _19
	dotP3	xmm1, xmm0	; tmp159, _19,
	sqrtps	xmm1, xmm1	; tmp160, tmp159
	divps	xmm0, xmm1	; _22, tmp160

%endif

[section .text align=1]

; grep -vF -e .cfi ray.asm | tr '#' ';' | sed -E 's/\.(size|type)/\;&/' | sed 's/PTR //g'  > ray2.asm
; perl -pE 'if(/^\s*dd\s+(-?\d+)/){$f=sprintf("%.7g",unpack("f", pack("l",$1)));s/$1/$f/}'

_ZL6vfrandv:
; fbdev/../rand.h:30: 	__asm__ volatile ( // sserand
; 30 "fbdev/../rand.h" 1
	mov	eax, [rand_state]	; rand_state, rand
	imul	eax, eax, 134775813	;, rand
	inc	eax	; rand
	push	eax	; rand
	imul	eax, eax, 134775813	;, rand
	inc	eax	; rand
	push	eax	; rand
	imul	eax, eax, 134775813	;, rand
	inc	eax	; rand
	push	eax	; rand
	imul	eax, eax, 134775813	;, rand
	inc	eax	; rand
	push	eax	; rand
	movdqu	xmm0, [esp]
	mov	[rand_state], eax	; rand, rand_state
	pop	eax	; rand
	pop	eax	; rand
	pop	eax	; rand
	pop	eax	; rand
	
	cvtdq2ps	xmm0, xmm0	; tmp105, result
; fbdev/../rand.h:73: 	return randf * mul; // vfrand
	;load4	xmm1, [INV_INT_MAX]
	set4	xmm1, 4.6566128730773925781e-10

	mulps	xmm0, xmm1	; _5, tmp108
; fbdev/../rand.h:74: } // end vfrand
	ret	
;LFE656:
	;.size	"_ZL6vfrandv", .-"_ZL6vfrandv"
	;.type	"_ZL13compute_colorDv4_fS_j", @function
_ZL13compute_colorDv4_fS_j:
;LFB673:
	push	ebx	;
	sub	esp, 24	;,
	movaps	xmm3, xmm0	; eye, eye
	mov	ecx, _ZL5scene	; ivtmp.64,
; week3.c:193: 	for(color_t v=1; v<NUM_SPHERES; v++){
	push	1	;
	pop	edx	; v
; week3.c:192: 	color_t num = 0; // compute_color hitcheck
	xor	ebx, ebx	; num
; week3.c:191: 	FLOAT min = FLOAT_MAX;	// FLOAT CONST
	;movss	xmm4, [FLOAT_MAX]	; min,
	set1	xmm4, 3.4028234663852885981e+38

; week3.c:112: 	FLOAT b = -2 * dotP(CE, d); // hitcheck
	; XXX
	;movss	xmm5, [MINUSTWO]	; tmp166,

hitcheck:
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	movaps	xmm2, xmm3	; _55, eye
	subps	xmm2, [ecx+64]	; _55, MEM[(vector(4) float * {ref-all})_124 + 64B]
	movaps	xmm0, xmm2	; tmp164, _55
	dotP	xmm0, xmm1
; week3.c:112: 	FLOAT b = -2 * dotP(CE, d); // hitcheck
	;mulss	xmm0, [MINUSTWO]	; b, tmp166
	cmulss	xmm0, -2.0
	dotP	xmm2, xmm2
; week3.c:115: 	FLOAT root = b*b - 4*c;	// FLOAT CONST 4
	movaps	xmm7, xmm0	; powmult_62, b
	mulss	xmm7, xmm0	; powmult_62, b
; week3.c:113: 	FLOAT c = dotP(CE,CE) - r;
	subss	xmm2, [ecx+112]	; c_61, MEM[(float *)_124 + 112B]
; week3.c:115: 	FLOAT root = b*b - 4*c;	// FLOAT CONST 4
	;movss	xmm6, [FOUR]	; tmp172,
	;mulss	xmm2, [FOUR]	; _63, tmp172
	cmulss	xmm2, 4.0
; week3.c:115: 	FLOAT root = b*b - 4*c;	// FLOAT CONST 4
	subss	xmm7, xmm2	; (b*b) - (4*c)
	sqrtss	xmm7, xmm7	; sqrt((b*b) - (4*c))
; week3.c:122: 	FLOAT l2 = (b - SQRT(root)) / 2;	// FLOAT CONST 2
	subss	xmm0, xmm7	; _66, _65
; week3.c:122: 	FLOAT l2 = (b - SQRT(root)) / 2;	// FLOAT CONST 2
	cmulss	xmm0, 0.5
; week3.c:124: 	if (l2 > 0)
	zero4	xmm2
	maxss	xmm0, xmm2	; _68, tmp176
; week3.c:195: 		if (hit != 0 && hit < min) {  // compute_color hitcheck
	comiss	xmm0, xmm2	; _68, tmp176
	je	notsmaller	;,
	comiss	xmm4, xmm0	; min, _68
; week3.c:199: 			min = hit; // compute_color hitcheck
	cmova	ebx, edx	; v,, num
	minss	xmm4, xmm0	; min, _68
notsmaller:
	add	ecx, 64		; next sphere
	inc	edx	; v
	cmp	edx, NUM_SPHERES; end of list?
	jne	hitcheck	;,

earlyreturn:
; week3.c:206: 	if ( num == 0 || depth>=MINDEPTH ) // compute_color early return
	test	ebx, ebx	; num
	jz	L17	;,
maxdepth:
	cmp	eax, 3	; depth,
	jne	nextray
L17:
; week3.c:213: 		return scene[num].emission; // compute_color early return
	sal	ebx, 6	; tmp190,
	movaps	xmm0, [ebx+32+_ZL5scene]	; <retval>, scene[num_15].emission
	jmp	compute_color_end

nextray:
	xchg	edx, eax	; depth, depth
	movaps	xmm6, xmm1	; ray, ray
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	shufps	xmm4, xmm4, 0	; _38
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	mulps	xmm4, xmm1	; _39, ray
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm4, xmm3	; _37, eye
; week3.c:221: 	const vec3 normal	= normV(scene[num].center, hitpoint); // compute_color normal
	mov	eax, ebx	; tmp196, num
	sal	eax, 6	; tmp196,
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	movaps	xmm2, xmm4	; _33, _37
	subps	xmm2, [eax+_ZL5scene]	; _33, scene[num_15].center

; NORM
	movaps	xmm0, xmm2	; tmp198, _33
	dotP3	xmm0, xmm2	; tmp198, _33,
	sqrtps	xmm0, xmm0	; tmp199, tmp198
	divps	xmm2, xmm0	; _36, tmp199

; week3.c:224: 	if (scene[num].material == 2) {				// compute_color MIRROR
	mov	cl, [eax+_ZL5scene+52]	; _8,
	cmp	cl, 2
	; XXX ???
	;cmp	[eax+_ZL5scene+52], 2	; _8,
	movaps	xmm5, [eax+_ZL5scene+16]	; result, scene[num_15].diffuse
	jne	mirror_skip	;,
mirror:
	dotP3	xmm1, xmm2	; tmp204, _36,
; fbdev/../vec3.h:15: 	return a*b; // vec3 hadamard
	mulps	xmm1, xmm2	; _70, _36
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	subps	xmm1, xmm6	; _71, ray
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	addps	xmm1, xmm1	; _72, _71
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm1, xmm6	; nextdir, ray
	jmp	brdf_end	;
mirror_skip:
; fbdev/../ray.h:52: 	vec3 rand = vfrand(); // randvec
	call	_ZL6vfrandv	;
	movaps	xmm3, xmm0	; tmp212, rand

; NORM
	movaps	xmm1, xmm0	; rand, rand
	dotP3	xmm3, xmm0	; tmp212, rand,
	sqrtps	xmm3, xmm3	; tmp213, tmp212
	divps	xmm1, xmm3	; rand, tmp213

; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	subps	xmm1, xmm2	; _43, _36

; NORM
	movaps	xmm0, xmm1	; tmp215, _43
	dotP3	xmm0, xmm1	; tmp215, _43,
	sqrtps	xmm0, xmm0	; tmp216, tmp215
	divps	xmm1, xmm0	; nextdir, tmp216

; week3.c:174: 	if ( scene[num].material == 1 ) {	// brdf REFLECTIVE
	dec	cl	; _8
	jne	brdf_end	;,
reflective:
	movaps	xmm3, xmm6	; tmp221, ray
	dotP3	xmm3, xmm2	; tmp221, _36,
; fbdev/../vec3.h:15: 	return a*b; // vec3 hadamard
	mulps	xmm3, xmm2	; _86, _36
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	subps	xmm3, xmm6	; _87, ray
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	addps	xmm3, xmm3	; _88, _87
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm3, xmm6	; _89, ray
	dotP	xmm3, xmm1	; tmp222, nextdir,
; week3.c:178: 		if (dotP(dr,out) > 1.0f - 0.01f) { // brdf reflective
	comiss	xmm3, [REFL_LIMIT]	; _82,
	jbe	brdf_end	;,
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm5, [DIELEC]	; result,
brdf_end:
	dotP	xmm2, xmm1	; tmp229, nextdir,
; week3.c:236: 	if (direction < 0) { // compute_color direction check
	zero4	xmm0
	comiss	xmm0, xmm2	; tmp231, _45
	jbe	change_direction_skip	;,
; fbdev/../vec3.h:20: 	return a*-1;
change_direction:
	;load4	xmm0, [NEGATE]
	set4	xmm0, -0.0	
	xorps	xmm1, xmm0	; nextdir, tmp236
change_direction_skip:
	movaps	[esp], xmm5	; %sfp, result
; week3.c:270: 	const vec3 moo = hadamard(foo, compute_color(hp_nudge, nextdir, depth+1)); // compute_color recurse
	lea	eax, [edx+1]	; _10,
	movaps	xmm0, xmm4	;, _37
	call	_ZL13compute_colorDv4_fS_j	;
; week3.c:271: 	return add(scene[num].emission,moo);
	sal	ebx, 6	; tmp239,
; fbdev/../vec3.h:15: 	return a*b; // vec3 hadamard
	mulps	xmm0, [esp]	; _48, %sfp
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm0, [ebx+_ZL5scene+32]	; <retval>, scene[num_15].emission
compute_color_end:
; week3.c:272: } // compute_color end
	add	esp, 24	;,
	pop	ebx	;
	ret	

_start:

mmap:
	inc	esi
	inc	ecx
	bswap	ecx
	mov	dl, 2
	mov	al, 192		; SYS_mmap2
	int	0x80
	xchg	ebx, eax	; ret, ret

YLOOP:
; week3.c:94: 	int m = x - half; // screen_convert
	lea	eax, [edi-300]	; _83,
; week3.c:96: 	FLOAT result = (FLOAT)m / (FLOAT)half; // screen_convert
	cvtsi2ss	xmm0, eax	; _8, _83
; week3.c:96: 	FLOAT result = (FLOAT)m / (FLOAT)half; // screen_convert
	mulss	xmm0, [RAY_STEP_Y]	; 0.00333
	shufps	xmm0, xmm0, 0	; _16
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	mulps	xmm0, [UL]	; _17,
	addps	xmm0, [FHAT]	; _17,
	movaps	[esp+16], xmm0	; %sfp, _17
	imul	esi, edi, STRIDE*4	; _78, y,
	lea	eax, [ebx+esi]	; _85,
	mov	DWORD [esp+44], eax	; %sfp, _85
	mov	ebp, -300	; ivtmp.84,
XLOOP:
; week3.c:96: 	FLOAT result = (FLOAT)m / (FLOAT)half; // screen_convert
	cvtsi2ss	xmm0, ebp	; _12, ivtmp.84
; week3.c:96: 	FLOAT result = (FLOAT)m / (FLOAT)half; // screen_convert
	mulss	xmm0, [RAY_STEP_X]	; result,
	shufps	xmm0, xmm0, 0	; _14
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	mulps	xmm0, [RL]	; _15,
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm0, [esp+16]	; _19, %sfp

; NORM
	movaps	xmm1, xmm0	; tmp159, _19
	dotP3	xmm1, xmm0	; tmp159, _19,
	sqrtps	xmm1, xmm1	; tmp160, tmp159
	divps	xmm0, xmm1	; _22, tmp160

; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	;load4	xmm1, [FOCAL_LENGTH]
	set4	xmm1, 3.4
	mulps	xmm0, xmm1	; _23, tmp164
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm0, [EYE]	; _23,
	movaps	[esp], xmm0	; %sfp, _23
	mov	esi, SAMPLES
; week3.c:348: 			vec3 sample = {0, 0, 0}; // render init sample
	zero4	xmm2
SAMPLELOOP:
	movaps	[esp+48], xmm2	; %sfp, sample
; fbdev/../rand.h:79: 	vec3 rand = vfrand(); // vgauss
	call	_ZL6vfrandv	;
	zero4	xmm1
	movhlps	xmm1, xmm0	; tmp166, rand
; week3.c:367: 					reye += gauss * BLUR_VAL; // render defocus blur
	mulps	xmm0, xmm1	; _98, tmp166
	mulps	xmm0, [BLUR_VAL]
; week3.c:367: 					reye += gauss * BLUR_VAL; // render defocus blur
	addps	xmm0, [EYE]	; reye,
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	movaps	xmm1, [esp]	; _28, %sfp
	subps	xmm1, xmm0	; _28, reye
	movaps	xmm3, xmm1	; tmp173, _28
	dotP3	xmm3, xmm1	; tmp173, _28,
	sqrtps	xmm3, xmm3	; tmp174, tmp173
; week3.c:378: 					vec3 single = compute_color(reye, blur_ray, 0); // render call cmpute_color
	xor	eax, eax	;
	divps	xmm1, xmm3	;, tmp174
	call	_ZL13compute_colorDv4_fS_j	;
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	movaps	xmm2, [esp+48]	; sample, %sfp
	addps	xmm2, xmm0	; sample, single
; week3.c:351: 				for(coord_t v=0;v < SAMPLES;v++){ // render collect samples
	dec	esi	; ivtmp_96
	jne	SAMPLELOOP


OUTPUT:
; week3.c:284: 	vec3 gamma = vsqrt(sample*RGB_CORR);
	;load4	xmm0, [SAMPLE_MUL]
	set4	xmm0, 127.0
	mulps	xmm2, xmm0	; _42, tmp179
	sqrtps	xmm2, xmm2	; tmp176, _42
	cvtps2dq	xmm2, xmm2	; tmp180, tmp176
	packusdw	xmm2, xmm2	; tmp181, tmp180
	packuswb	xmm2, xmm2	; tmp182, tmp181
	mov	eax, DWORD [esp+44]	; _85, %sfp
	movups	[eax+1200+ebp*4], xmm2	; MEM[(__m128_u * {ref-all})_85 + 1200B + ivtmp.84_91 * 4], tmp182
; week3.c:335: 		for(int x=0; x < WIDTH; x++){ // render x
	inc	ebp	; ivtmp.84
	cmp	ebp, 300	; ivtmp.84,
	jne	XLOOP
; week3.c:333: 	for(int y=0; y < HEIGHT; y++){ // render y
	inc	edi	; y
; week3.c:333: 	for(int y=0; y < HEIGHT; y++){ // render y
	cmp	edi, 600
	jne	YLOOP
	jmp	$

;align 4, db 0

[section .data align=1]


%macro zero 1
	times %1 db 0
%endmacro

%macro float 1
	;dd float32(%tok(%strcat(%1,".0")))
	dd float32(%1)
%endmacro

%macro dv 0-4 0.0,0.0,0.0,0.0
	dd %1
	dd %2
	dd %3
	dd %4
%endmacro

%macro sphere 5
	%assign num num + 1
	dv %1	; center
	dv %3	; diffuse
	dv %4	; emission
	dd %2	; radius
	db %5	; type
	zero 11
%endmacro

%assign DIFFUSE		0
%assign REFLECTIVE	1
%assign MIRROR		2

align 16, db 0
_ZL5scene:
%assign num 0

%define BLACK	{ 0.0, 0.0, 0.0 }
%define GRAY	{ 0.5, 0.5, 0.5 }
%define LIGHT	{ 6.0, 6.0, 6.0 }
%define YELLOW	{ 0.0, 0.8, 0.8 }
%define GREEN	{ 0.0, 0.5, 0.0 }
%define PINK	{ 0.8, 0.0, 0.8 }

sphere BLACK,                     0.0, BLACK,  BLACK, 0
sphere { 0.0,-1000.8, 0.0}, 1000000.0, GRAY,   BLACK, 0
sphere { 0.0, 1001.0, 0.0}, 1000000.0, BLACK,  BLACK, 0
sphere { 0.0,   4.95, 0.0},      16.0, BLACK,  LIGHT, 0
sphere {-0.5,   -0.5,-0.6},      0.09, YELLOW, BLACK, 2
sphere { 0.6,   -0.6,-0.6},      0.04, GREEN,  BLACK, 2
sphere { 0.3,   -0.2, 0.3},      0.36, PINK,   BLACK, 1

NUM_SPHERES equ num


scenesize equ $ - _ZL5scene
%warning SCENE SIZE %eval(scenesize)


	align	16
DIELEC:
	float	12.56637
	float	12.56637
	float	12.56637
	float	0.0
UL:
	float	0.0
	float	-0.3249197
	float	0.0
	float	0.0
FHAT:
	float	0.0
	float	0.0
	float	1.0
	float	0.0
RL:
	float	0.3249197
	float	0.0
	float	0.0
	float	0.0
EYE:
	float	0.0
	float	0.0
	float	-4.0
	float	0.0
BLUR_VAL:
	float	0.23
	float	0.23
	float	0.23
	float	0.23


	align	4

;INV_INT_MAX:
;	float	4.6566128730773925781e-10
;FLOAT_MAX:
;	float	3.4028234663852885981e+38
;MINUSTWO:
;	float	-2.0
;FOUR:
;	float	4.0
;HALF:
;	float	0.5
REFL_LIMIT:
	float	0.99
;NEGATE:
;	float	-0.0
RAY_STEP_X:
RAY_STEP_Y:
	float	0.003333333
;FOCAL_LENGTH:
;	float	3.4
;SAMPLE_MUL:
;	float	127.0

rand_state:
	dd	0
