%if 0
OUT=ray
nasm -f elf32 $0 -o "${OUT}.o"
ld -N -Ttext-segment=0x10000 --noinhibit-exec -z noseparate-code -m elf_i386 "${OUT}.o" -o "$OUT"
#objdump --disassembler-color=on -Mintel -dSl "$OUT"
exit 0
%endif


BITS 32
extern _start
;org 0x10000

[section .text align=1]

; grep -vF -e .cfi ray.asm | tr '#' ';' | sed -E 's/\.(size|type)/\;&/' | sed 's/PTR //g'  > ray2.asm

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
	
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/emmintrin.h:815:   return (__m128)__builtin_ia32_cvtdq2ps ((__v4si) __A);
	cvtdq2ps	xmm0, xmm0	; tmp105, result
; fbdev/../rand.h:73: 	return randf * mul; // vfrand
	movss	xmm1, [LC1]	; tmp108,
	shufps	xmm1, xmm1, 0	; tmp108
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
	movss	xmm4, [LC2]	; min,
; week3.c:112: 	FLOAT b = -2 * dotP(CE, d); // hitcheck
	movss	xmm5, [LC3]	; tmp166,
; week3.c:115: 	FLOAT root = b*b - 4*c;	// FLOAT CONST 4
	movss	xmm6, [LC4]	; tmp172,
L5:
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	movaps	xmm2, xmm3	; _55, eye
	subps	xmm2, [ecx+64]	; _55, MEM[(vector(4) float * {ref-all})_124 + 64B]
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	movaps	xmm0, xmm2	; tmp164, _55
	dpps	xmm0, xmm1, 113	; tmp164, ray,
; week3.c:112: 	FLOAT b = -2 * dotP(CE, d); // hitcheck
	mulss	xmm0, xmm5	; b, tmp166
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	dpps	xmm2, xmm2, 113	; tmp167, _55,
; week3.c:115: 	FLOAT root = b*b - 4*c;	// FLOAT CONST 4
	movaps	xmm7, xmm0	; powmult_62, b
	mulss	xmm7, xmm0	; powmult_62, b
; week3.c:113: 	FLOAT c = dotP(CE,CE) - r;
	subss	xmm2, [ecx+112]	; c_61, MEM[(float *)_124 + 112B]
; week3.c:115: 	FLOAT root = b*b - 4*c;	// FLOAT CONST 4
	mulss	xmm2, xmm6	; _63, tmp172
; week3.c:115: 	FLOAT root = b*b - 4*c;	// FLOAT CONST 4
	subss	xmm7, xmm2	; root_64, _63
; week3.c:122: 	FLOAT l2 = (b - SQRT(root)) / 2;	// FLOAT CONST 2
	sqrtss	xmm7, xmm7	; _65, root_64
; week3.c:122: 	FLOAT l2 = (b - SQRT(root)) / 2;	// FLOAT CONST 2
	subss	xmm0, xmm7	; _66, _65
; week3.c:122: 	FLOAT l2 = (b - SQRT(root)) / 2;	// FLOAT CONST 2
	mulss	xmm0, [LC5]	; _68,
; week3.c:124: 	if (l2 > 0)
	xorps	xmm2, xmm2	; tmp176
	maxss	xmm0, xmm2	; _68, tmp176
; week3.c:195: 		if (hit != 0 && hit < min) {  // compute_color hitcheck
	comiss	xmm0, xmm2	; _68, tmp176
	je	L3	;,
	comiss	xmm4, xmm0	; min, _68
; week3.c:199: 			min = hit; // compute_color hitcheck
	cmova	ebx, edx	; v,, num
	minss	xmm4, xmm0	; min, _68
L3:
; week3.c:193: 	for(color_t v=1; v<NUM_SPHERES; v++){
	inc	edx	; v
; week3.c:193: 	for(color_t v=1; v<NUM_SPHERES; v++){
	add	ecx, 64	; ivtmp.64,
	cmp	edx, 7	; v,
	jne	L5	;,
; week3.c:206: 	if ( num == 0 || depth>=MINDEPTH ) // compute_color early return
	test	ebx, ebx	; num
	je	L17	;,
	cmp	eax, 3	; depth,
	jne	L6	;,
L17:
; week3.c:213: 		return scene[num].emission; // compute_color early return
	sal	ebx, 6	; tmp190,
	movaps	xmm0, [ebx+32+_ZL5scene]	; <retval>, scene[num_15].emission
	jmp	L2	;
L6:
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
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	movaps	xmm0, xmm2	; tmp198, _33
	dpps	xmm0, xmm2, 119	; tmp198, _33,
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/xmmintrin.h:236:   return (__m128) __builtin_ia32_sqrtps ((__v4sf)__A);
	sqrtps	xmm0, xmm0	; tmp199, tmp198
; fbdev/../vec3.h:62: 		return a/rsqrt; // norm
	divps	xmm2, xmm0	; _36, tmp199
; week3.c:224: 	if (scene[num].material == 2) {				// compute_color MIRROR
	mov	cl, [eax+_ZL5scene+52]	; _8, scene[num_15].material
; week3.c:224: 	if (scene[num].material == 2) {				// compute_color MIRROR
	cmp	cl, 2	; _8,
	movaps	xmm5, [eax+_ZL5scene+16]	; result, scene[num_15].diffuse
	jne	L9	;,
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	dpps	xmm1, xmm2, 119	; tmp204, _36,
; fbdev/../vec3.h:15: 	return a*b; // vec3 hadamard
	mulps	xmm1, xmm2	; _70, _36
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	subps	xmm1, xmm6	; _71, ray
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	addps	xmm1, xmm1	; _72, _71
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm1, xmm6	; nextdir, ray
	jmp	L10	;
L9:
; fbdev/../ray.h:52: 	vec3 rand = vfrand(); // randvec
	call	_ZL6vfrandv	;
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	movaps	xmm3, xmm0	; tmp212, rand
	dpps	xmm3, xmm0, 119	; tmp212, rand,
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/xmmintrin.h:236:   return (__m128) __builtin_ia32_sqrtps ((__v4sf)__A);
	sqrtps	xmm3, xmm3	; tmp213, tmp212
; fbdev/../vec3.h:62: 		return a/rsqrt; // norm
	movaps	xmm1, xmm0	; rand, rand
	divps	xmm1, xmm3	; rand, tmp213
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	subps	xmm1, xmm2	; _43, _36
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	movaps	xmm0, xmm1	; tmp215, _43
	dpps	xmm0, xmm1, 119	; tmp215, _43,
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/xmmintrin.h:236:   return (__m128) __builtin_ia32_sqrtps ((__v4sf)__A);
	sqrtps	xmm0, xmm0	; tmp216, tmp215
; fbdev/../vec3.h:62: 		return a/rsqrt; // norm
	divps	xmm1, xmm0	; nextdir, tmp216
; week3.c:174: 	if ( scene[num].material == 1 ) {	// brdf REFLECTIVE
	dec	cl	; _8
	jne	L10	;,
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	movaps	xmm3, xmm6	; tmp221, ray
	dpps	xmm3, xmm2, 119	; tmp221, _36,
; fbdev/../vec3.h:15: 	return a*b; // vec3 hadamard
	mulps	xmm3, xmm2	; _86, _36
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	subps	xmm3, xmm6	; _87, ray
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	addps	xmm3, xmm3	; _88, _87
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm3, xmm6	; _89, ray
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	dpps	xmm3, xmm1, 113	; tmp222, nextdir,
; week3.c:178: 		if (dotP(dr,out) > 1.0f - 0.01f) { // brdf reflective
	comiss	xmm3, [LC7]	; _82,
	jbe	L10	;,
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm5, [LC8]	; result,
L10:
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	dpps	xmm2, xmm1, 113	; tmp229, nextdir,
; week3.c:236: 	if (direction < 0) { // compute_color direction check
	xorps	xmm0, xmm0	; tmp231
	comiss	xmm0, xmm2	; tmp231, _45
	jbe	L14	;,
; fbdev/../vec3.h:20: 	return a*-1;
	movss	xmm0, [LC10]	; tmp236,
	shufps	xmm0, xmm0, 0	; tmp236
	xorps	xmm1, xmm0	; nextdir, tmp236
L14:
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
L2:
; week3.c:272: } // compute_color end
	add	esp, 24	;,
	pop	ebx	;
	ret	
;LFE673:
	;.size	"_ZL13compute_colorDv4_fS_j", .-"_ZL13compute_colorDv4_fS_j"
	;.globl	"_start"
	;.type	"_start", @function
_start:
;LFB665:
; fbdev/libcero.h:107: 	register size_t arg3r __asm__ (ARG3) = flags;
	push	1	;
	pop	esi	; arg3r
; fbdev/libcero.h:108: 	register size_t arg4r __asm__ (ARG4) = fd;
	xor	edi, edi	; arg4r
; fbdev/libcero.h:112: 	__asm__ __volatile__(
	mov	ecx, 4608000	; tmp144,
	push	2	;
	pop	edx	; tmp145
	mov	eax, 192	; ret,
; 112 "fbdev/libcero.h" 1
	push	ebp
	xor	ebp, ebp
	int	0x80
	pop	ebp
	
	xchg	ebx, eax	; ret, ret
L29:
; week3.c:94: 	int m = x - half; // screen_convert
	lea	eax, [edi-300]	; _83,
; week3.c:96: 	FLOAT result = (FLOAT)m / (FLOAT)half; // screen_convert
	cvtsi2ss	xmm0, eax	; _8, _83
; week3.c:96: 	FLOAT result = (FLOAT)m / (FLOAT)half; // screen_convert
	mulss	xmm0, [LC11]	; result,
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	shufps	xmm0, xmm0, 0	; _16
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	mulps	xmm0, [LC12]	; _17,
	addps	xmm0, [LC13]	; _17,
	movaps	[esp+16], xmm0	; %sfp, _17
	imul	esi, edi, 7680	; _78, y,
	lea	eax, [ebx+esi]	; _85,
	mov	DWORD [esp+44], eax	; %sfp, _85
	mov	ebp, -300	; ivtmp.84,
L28:
; week3.c:96: 	FLOAT result = (FLOAT)m / (FLOAT)half; // screen_convert
	cvtsi2ss	xmm0, ebp	; _12, ivtmp.84
; week3.c:96: 	FLOAT result = (FLOAT)m / (FLOAT)half; // screen_convert
	mulss	xmm0, [LC11]	; result,
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	shufps	xmm0, xmm0, 0	; _14
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	mulps	xmm0, [LC14]	; _15,
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm0, [esp+16]	; _19, %sfp
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	movaps	xmm1, xmm0	; tmp159, _19
	dpps	xmm1, xmm0, 119	; tmp159, _19,
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/xmmintrin.h:236:   return (__m128) __builtin_ia32_sqrtps ((__v4sf)__A);
	sqrtps	xmm1, xmm1	; tmp160, tmp159
; fbdev/../vec3.h:62: 		return a/rsqrt; // norm
	divps	xmm0, xmm1	; _22, tmp160
; fbdev/../vec3.h:90: 	return a*amount; // vec3 scalar_mult
	movss	xmm1, [LC16]	; tmp164,
	shufps	xmm1, xmm1, 0	; tmp164
	mulps	xmm0, xmm1	; _23, tmp164
; fbdev/../vec3.h:5: 	return a+b; // vec3 add
	addps	xmm0, [LC17]	; _23,
	movaps	[esp], xmm0	; %sfp, _23
	mov	esi, 512	; ivtmp_96,
; week3.c:348: 			vec3 sample = {0, 0, 0}; // render init sample
	xorps	xmm2, xmm2	; sample
L27:
	movaps	[esp+48], xmm2	; %sfp, sample
; fbdev/../rand.h:79: 	vec3 rand = vfrand(); // vgauss
	call	_ZL6vfrandv	;
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/xmmintrin.h:835:   return (__m128) __builtin_ia32_movhlps ((__v4sf)__A, (__v4sf)__B);
	xorps	xmm1, xmm1	; tmp167
	movhlps	xmm1, xmm0	; tmp166, rand
; week3.c:367: 					reye += gauss * BLUR_VAL; // render defocus blur
	mulps	xmm0, xmm1	; _98, tmp166
	mulps	xmm0, [LC18]	; _26,
; week3.c:367: 					reye += gauss * BLUR_VAL; // render defocus blur
	addps	xmm0, [LC17]	; reye,
; fbdev/../vec3.h:10: 	return a-b; // vec3 sub
	movaps	xmm1, [esp]	; _28, %sfp
	subps	xmm1, xmm0	; _28, reye
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:243:   return (__m128) __builtin_ia32_dpps ((__v4sf)__X,
	movaps	xmm3, xmm1	; tmp173, _28
	dpps	xmm3, xmm1, 119	; tmp173, _28,
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/xmmintrin.h:236:   return (__m128) __builtin_ia32_sqrtps ((__v4sf)__A);
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
	jne	L27	;,
; week3.c:284: 	vec3 gamma = vsqrt(sample*RGB_CORR);
	movss	xmm0, [LC21]	; tmp179,
	shufps	xmm0, xmm0, 0	; tmp179
	mulps	xmm2, xmm0	; _42, tmp179
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/xmmintrin.h:236:   return (__m128) __builtin_ia32_sqrtps ((__v4sf)__A);
	sqrtps	xmm2, xmm2	; tmp176, _42
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/emmintrin.h:857:   return (__m128i)__builtin_ia32_cvtps2dq ((__v4sf) __A);
	cvtps2dq	xmm2, xmm2	; tmp180, tmp176
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/smmintrin.h:561:   return (__m128i) __builtin_ia32_packusdw128 ((__v4si)__X, (__v4si)__Y);
	packusdw	xmm2, xmm2	; tmp181, tmp180
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/emmintrin.h:1007:   return (__m128i)__builtin_ia32_packuswb128 ((__v8hi)__A, (__v8hi)__B);
	packuswb	xmm2, xmm2	; tmp182, tmp181
; /usr/lib/gcc/x86_64-pc-linux-gnu/16/include/xmmintrin.h:1031:   *(__m128_u *)__P = __A;
	mov	eax, DWORD [esp+44]	; _85, %sfp
	movups	[eax+1200+ebp*4], xmm2	; MEM[(__m128_u * {ref-all})_85 + 1200B + ivtmp.84_91 * 4], tmp182
; week3.c:335: 		for(int x=0; x < WIDTH; x++){ // render x
	inc	ebp	; ivtmp.84
	cmp	ebp, 300	; ivtmp.84,
	jne	L28	;,
; week3.c:333: 	for(int y=0; y < HEIGHT; y++){ // render y
	inc	edi	; y
; week3.c:333: 	for(int y=0; y < HEIGHT; y++){ // render y
	cmp	edi, 600	; y,
	jne	L29	;,
	jmp	$





%macro zero 1
	times %1 db 0
%endmacro

%macro dv 0-4 0 0 0 0
	dd %1
	dd %2
	dd %3
	dd %4
%endmacro

;LFE665:
	;.size	"_start", .-"_start"
	;.section	.rodata
	;align 16
	;.type	"_ZL5scene", @object
	;.size	"_ZL5scene", 448
align 16
_ZL5scene:

; center:
	dd	0
	dd	0
	dd	0
	dd	0
; diffuse:
	dd	0
	dd	0
	dd	0
	dd	0
; emission:
	dd	0
	dd	0
	dd	0
	dd	0
; radius:
	dd	0
; material:
	db	0
	zero	11
; center:
	dd	0
	dd	-998624461
	dd	0
	dd	0
; diffuse:
	dd	1056964608
	dd	1056964608
	dd	1056964608
	dd	0
; emission:
	dd	0
	dd	0
	dd	0
	dd	0
; radius:
	dd	1232348160
; material:
	db	0
	zero	11
; center:
	dd	0
	dd	1148862464
	dd	0
	dd	0
; diffuse:
	dd	0
	dd	0
	dd	0
	dd	0
; emission:
	dd	0
	dd	0
	dd	0
	dd	0
; radius:
	dd	1232348160
; material:
	db	0
	zero	11
; center:
	dd	0
	dd	1084122726
	dd	0
	dd	0
; diffuse:
	dd	0
	dd	0
	dd	0
	dd	0
; emission:
	dd	1086324736
	dd	1086324736
	dd	1086324736
	dd	0
; radius:
	dd	1098907648
; material:
	db	0
	zero	11
; center:
	dd	-1090519040
	dd	-1090519040
	dd	-1088841318
	dd	0
; diffuse:
	dd	0
	dd	1061997773
	dd	1061997773
	dd	0
; emission:
	dd	0
	dd	0
	dd	0
	dd	0
; radius:
	dd	1035489772
; material:
	db	2
	zero	11
; center:
	dd	1058642330
	dd	-1088841318
	dd	-1088841318
	dd	0
; diffuse:
	dd	0
	dd	1056964608
	dd	0
	dd	0
; emission:
	dd	0
	dd	0
	dd	0
	dd	0
; radius:
	dd	1025758986
; material:
	db	2
	zero	11
; center:
	dd	1050253722
	dd	-1102263091
	dd	1050253722
	dd	0
; diffuse:
	dd	1061997773
	dd	0
	dd	1061997773
	dd	0
; emission:
	dd	0
	dd	0
	dd	0
	dd	0
; radius:
	dd	1052266988
; material:
	db	1
	zero	11
	;.comm	"rand_state",4,4
.end:
scenesize equ _ZL5scene.end - _ZL5scene
%warning SCENE SIZE %eval(scenesize)
	;.section	.rodata.cst4,"aM",@progbits,4
	align 4
LC1:
	dd	805306368
	align 4
LC2:
	dd	2139095039
	align 4
LC3:
	dd	-1073741824
	align 4
LC4:
	dd	1082130432
	align 4
LC5:
	dd	1056964608
	align 4
LC7:
	dd	1065185444
	;.section	.rodata.cst16,"aM",@progbits,16
	align 16
LC8:
	dd	1095307227
	dd	1095307227
	dd	1095307227
	dd	0
	;.section	.rodata.cst4
	align 4
LC10:
	dd	-2147483648
	align 4
LC11:
	dd	995783694
	;.section	.rodata.cst16
	align 16
LC12:
	dd	0
	dd	-1096393760
	dd	0
	dd	0
	align 16
LC13:
	dd	0
	dd	0
	dd	1065353216
	dd	0
	align 16
LC14:
	dd	1051089888
	dd	0
	dd	0
	dd	0
	;.section	.rodata.cst4
	align 4
LC16:
	dd	1079613850
	;.section	.rodata.cst16
	align 16
LC17:
	dd	0
	dd	0
	dd	-1065353216
	dd	0
	align 16
LC18:
	dd	1047233823
	dd	1047233823
	dd	1047233823
	dd	1047233823
	;.section	.rodata.cst4
	align 4
LC21:
	dd	1123942400
	;.section	.note.GNU-stack,"",@progbits

rand_state:
	dd	0
