	.file	"matmul.c"
	.version	"01.01"
gcc2_compiled.:
.text
	.align 4
.globl matmul
	.type	 matmul,@function
matmul:
	pushl %ebp
	movl %esp,%ebp
	subl $104,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl $2,-16(%ebp)
	movl 8(%ebp),%eax
	movl %eax,-20(%ebp)
	movl -20(%ebp),%ecx
	movl %ecx,%eax
	cltd
	idivl -16(%ebp)
	movl %eax,-24(%ebp)
	movl 12(%ebp),%eax
	movl %eax,-28(%ebp)
	movl 16(%ebp),%eax
	movl %eax,-32(%ebp)
	movl 20(%ebp),%eax
	movl %eax,-36(%ebp)
	movl $0,-4(%ebp)
	.p2align 4,,7
.L2:
	movl -4(%ebp),%eax
	cmpl -24(%ebp),%eax
	jl .L5
	jmp .L3
	.p2align 4,,7
.L5:
	nop
	movl $0,-8(%ebp)
	.p2align 4,,7
.L6:
	movl -8(%ebp),%eax
	cmpl -24(%ebp),%eax
	jl .L9
	jmp .L4
	.p2align 4,,7
.L9:
	nop
	movl $0,-12(%ebp)
	.p2align 4,,7
.L10:
	movl -12(%ebp),%eax
	cmpl -24(%ebp),%eax
	jl .L13
	jmp .L8
	.p2align 4,,7
.L13:
	movl -4(%ebp),%eax
	imull -16(%ebp),%eax
	movl %eax,%edx
	imull -20(%ebp),%edx
	leal 0(,%edx,8),%ecx
	movl %ecx,%eax
	addl -28(%ebp),%eax
	movl -12(%ebp),%edx
	imull -16(%ebp),%edx
	leal 0(,%edx,8),%ecx
	leal (%ecx,%eax),%ebx
	movl %ebx,-40(%ebp)
	movl -12(%ebp),%eax
	imull -16(%ebp),%eax
	movl %eax,%edx
	imull -20(%ebp),%edx
	leal 0(,%edx,8),%ecx
	movl %ecx,%eax
	addl -32(%ebp),%eax
	movl -8(%ebp),%edx
	imull -16(%ebp),%edx
	leal 0(,%edx,8),%ecx
	leal (%ecx,%eax),%edi
	movl %edi,-44(%ebp)
	movl -4(%ebp),%eax
	imull -16(%ebp),%eax
	movl %eax,%edx
	imull -20(%ebp),%edx
	leal 0(,%edx,8),%ecx
	movl %ecx,%eax
	addl -36(%ebp),%eax
	movl -8(%ebp),%edx
	imull -16(%ebp),%edx
	leal 0(,%edx,8),%ecx
	leal (%ecx,%eax),%ebx
	movl %ebx,-48(%ebp)
	movl -48(%ebp),%eax
	movl -48(%ebp),%edx
	movl -40(%ebp),%ecx
	movl -44(%ebp),%edi
	movl %edi,-104(%ebp)
	fldl (%ecx)
	movl -104(%ebp),%ebx
	fmull (%ebx)
	movl -40(%ebp),%ecx
	addl $8,%ecx
	movl -20(%ebp),%esi
	leal 0(,%esi,8),%edi
	movl %edi,-104(%ebp)
	movl -44(%ebp),%esi
	fldl (%ecx)
	movl -104(%ebp),%ebx
	fmull (%esi,%ebx)
	faddp %st,%st(1)
	fldl (%edx)
	faddp %st,%st(1)
	fstpl (%eax)
	movl -48(%ebp),%eax
	addl $8,%eax
	movl -48(%ebp),%edx
	addl $8,%edx
	movl -40(%ebp),%ecx
	movl -44(%ebp),%edi
	addl $8,%edi
	movl %edi,-104(%ebp)
	fldl (%ecx)
	movl -104(%ebp),%ebx
	fmull (%ebx)
	movl -40(%ebp),%ecx
	addl $8,%ecx
	movl -20(%ebp),%edi
	movl %edi,-104(%ebp)
	movl -104(%ebp),%ebx
	leal 0(,%ebx,8),%esi
	movl -44(%ebp),%edi
	addl %esi,%edi
	movl %edi,-104(%ebp)
	movl -104(%ebp),%esi
	addl $8,%esi
	fldl (%ecx)
	fmull (%esi)
	faddp %st,%st(1)
	fldl (%edx)
	faddp %st,%st(1)
	fstpl (%eax)
	movl -20(%ebp),%edx
	leal 0(,%edx,8),%eax
	movl -48(%ebp),%edx
	movl -20(%ebp),%ebx
	movl %ebx,-104(%ebp)
	movl -104(%ebp),%edi
	leal 0(,%edi,8),%ecx
	movl -48(%ebp),%ebx
	movl %ebx,-104(%ebp)
	movl -20(%ebp),%edi
	movl %edi,-100(%ebp)
	movl -100(%ebp),%ebx
	leal 0(,%ebx,8),%esi
	movl -40(%ebp),%edi
	movl %edi,-100(%ebp)
	movl -44(%ebp),%ebx
	movl %ebx,-52(%ebp)
	movl -100(%ebp),%edi
	fldl (%edi,%esi)
	movl -52(%ebp),%ebx
	fmull (%ebx)
	movl -20(%ebp),%esi
	leal 0(,%esi,8),%edi
	movl %edi,-100(%ebp)
	movl -100(%ebp),%ebx
	addl -40(%ebp),%ebx
	movl %ebx,-56(%ebp)
	movl -56(%ebp),%esi
	addl $8,%esi
	movl -20(%ebp),%edi
	movl %edi,-60(%ebp)
	movl -60(%ebp),%ebx
	leal 0(,%ebx,8),%ebx
	movl %ebx,-100(%ebp)
	movl -44(%ebp),%edi
	movl %edi,-64(%ebp)
	fldl (%esi)
	movl -100(%ebp),%ebx
	movl -64(%ebp),%edi
	fmull (%edi,%ebx)
	faddp %st,%st(1)
	movl -104(%ebp),%ebx
	fldl (%ebx,%ecx)
	faddp %st,%st(1)
	fstpl (%edx,%eax)
	movl -20(%ebp),%eax
	leal 0(,%eax,8),%edx
	movl %edx,%ecx
	addl -48(%ebp),%ecx
	leal 8(%ecx),%eax
	movl -20(%ebp),%edx
	leal 0(,%edx,8),%ecx
	movl -48(%ebp),%edi
	addl %ecx,%edi
	movl %edi,-104(%ebp)
	movl -104(%ebp),%edx
	addl $8,%edx
	movl -20(%ebp),%ebx
	movl %ebx,-104(%ebp)
	movl -104(%ebp),%edi
	leal 0(,%edi,8),%ecx
	movl -40(%ebp),%ebx
	movl %ebx,-104(%ebp)
	movl -44(%ebp),%esi
	addl $8,%esi
	movl -104(%ebp),%edi
	fldl (%edi,%ecx)
	fmull (%esi)
	movl -20(%ebp),%ecx
	leal 0(,%ecx,8),%ebx
	movl %ebx,-104(%ebp)
	movl -104(%ebp),%esi
	addl -40(%ebp),%esi
	leal 8(%esi),%ecx
	movl -20(%ebp),%edi
	movl %edi,-104(%ebp)
	movl -104(%ebp),%ebx
	leal 0(,%ebx,8),%esi
	movl -44(%ebp),%edi
	addl %esi,%edi
	movl %edi,-104(%ebp)
	movl -104(%ebp),%esi
	addl $8,%esi
	fldl (%ecx)
	fmull (%esi)
	faddp %st,%st(1)
	fldl (%edx)
	faddp %st,%st(1)
	fstpl (%eax)
.L12:
	incl -12(%ebp)
	jmp .L10
	.p2align 4,,7
.L11:
.L8:
	incl -8(%ebp)
	jmp .L6
	.p2align 4,,7
.L7:
.L4:
	incl -4(%ebp)
	jmp .L2
	.p2align 4,,7
.L3:
	movl 8(%ebp),%eax
	andl $1,%eax
	testl %eax,%eax
	jne .L14
	jmp .L1
	.p2align 4,,7
.L14:
	nop
	movl $0,-4(%ebp)
	.p2align 4,,7
.L15:
	movl -20(%ebp),%eax
	decl %eax
	cmpl %eax,-4(%ebp)
	jl .L18
	jmp .L16
	.p2align 4,,7
.L18:
	nop
	movl $0,-8(%ebp)
	.p2align 4,,7
.L19:
	movl -20(%ebp),%eax
	decl %eax
	cmpl %eax,-8(%ebp)
	jl .L22
	jmp .L17
	.p2align 4,,7
.L22:
	movl -4(%ebp),%eax
	imull -20(%ebp),%eax
	movl %eax,%edx
	addl -8(%ebp),%edx
	leal 0(,%edx,8),%eax
	movl -36(%ebp),%edx
	movl -4(%ebp),%ecx
	imull -20(%ebp),%ecx
	movl -8(%ebp),%ebx
	addl %ecx,%ebx
	movl %ebx,-104(%ebp)
	movl -104(%ebp),%edi
	leal 0(,%edi,8),%ecx
	movl -36(%ebp),%ebx
	movl %ebx,-104(%ebp)
	movl -4(%ebp),%esi
	imull -20(%ebp),%esi
	movl -20(%ebp),%edi
	addl %esi,%edi
	movl %edi,-100(%ebp)
	movl -100(%ebp),%ebx
	leal 0(,%ebx,8),%esi
	movl -28(%ebp),%edi
	addl %esi,%edi
	movl %edi,-100(%ebp)
	movl -100(%ebp),%esi
	addl $-8,%esi
	movl -20(%ebp),%ebx
	decl %ebx
	movl %ebx,-100(%ebp)
	movl -100(%ebp),%edi
	imull -20(%ebp),%edi
	movl %edi,-68(%ebp)
	movl -68(%ebp),%ebx
	addl -8(%ebp),%ebx
	movl %ebx,-72(%ebp)
	movl -72(%ebp),%edi
	leal 0(,%edi,8),%edi
	movl %edi,-100(%ebp)
	movl -32(%ebp),%ebx
	movl %ebx,-76(%ebp)
	fldl (%esi)
	movl -100(%ebp),%edi
	movl -76(%ebp),%ebx
	fmull (%ebx,%edi)
	movl -104(%ebp),%edi
	fldl (%edi,%ecx)
	faddp %st,%st(1)
	fstpl (%edx,%eax)
.L21:
	incl -8(%ebp)
	jmp .L19
	.p2align 4,,7
.L20:
.L17:
	incl -4(%ebp)
	jmp .L15
	.p2align 4,,7
.L16:
	nop
	movl $0,-4(%ebp)
	.p2align 4,,7
.L23:
	movl -4(%ebp),%eax
	cmpl -20(%ebp),%eax
	jl .L26
	jmp .L24
	.p2align 4,,7
.L26:
	nop
	movl $0,-12(%ebp)
	.p2align 4,,7
.L27:
	movl -12(%ebp),%eax
	cmpl -20(%ebp),%eax
	jl .L30
	jmp .L25
	.p2align 4,,7
.L30:
	movl -4(%ebp),%eax
	imull -20(%ebp),%eax
	movl %eax,%edx
	addl -20(%ebp),%edx
	leal 0(,%edx,8),%eax
	movl %eax,%edx
	addl -36(%ebp),%edx
	leal -8(%edx),%eax
	movl -4(%ebp),%edx
	imull -20(%ebp),%edx
	movl %edx,%ecx
	addl -20(%ebp),%ecx
	leal 0(,%ecx,8),%edx
	movl %edx,%ecx
	addl -36(%ebp),%ecx
	leal -8(%ecx),%edx
	movl -4(%ebp),%ecx
	imull -20(%ebp),%ecx
	movl -12(%ebp),%ebx
	addl %ecx,%ebx
	movl %ebx,-104(%ebp)
	movl -104(%ebp),%edi
	leal 0(,%edi,8),%ecx
	movl -28(%ebp),%ebx
	movl %ebx,-104(%ebp)
	movl -12(%ebp),%esi
	imull -20(%ebp),%esi
	movl -20(%ebp),%edi
	addl %esi,%edi
	movl %edi,-100(%ebp)
	movl -100(%ebp),%ebx
	leal 0(,%ebx,8),%esi
	movl -32(%ebp),%edi
	addl %esi,%edi
	movl %edi,-100(%ebp)
	movl -100(%ebp),%esi
	addl $-8,%esi
	movl -104(%ebp),%ebx
	fldl (%ebx,%ecx)
	fmull (%esi)
	fldl (%edx)
	faddp %st,%st(1)
	fstpl (%eax)
.L29:
	incl -12(%ebp)
	jmp .L27
	.p2align 4,,7
.L28:
.L25:
	incl -4(%ebp)
	jmp .L23
	.p2align 4,,7
.L24:
	nop
	movl $0,-4(%ebp)
	.p2align 4,,7
.L31:
	movl -20(%ebp),%eax
	decl %eax
	cmpl %eax,-4(%ebp)
	jl .L34
	jmp .L32
	.p2align 4,,7
.L34:
	nop
	movl $0,-12(%ebp)
	.p2align 4,,7
.L35:
	movl -12(%ebp),%eax
	cmpl -20(%ebp),%eax
	jl .L38
	jmp .L33
	.p2align 4,,7
.L38:
	movl -20(%ebp),%eax
	decl %eax
	movl %eax,%edx
	imull -20(%ebp),%edx
	movl %edx,%ecx
	addl -4(%ebp),%ecx
	leal 0(,%ecx,8),%eax
	movl -36(%ebp),%edx
	movl -20(%ebp),%ecx
	decl %ecx
	movl %ecx,%edi
	imull -20(%ebp),%edi
	movl %edi,-104(%ebp)
	movl -104(%ebp),%esi
	addl -4(%ebp),%esi
	leal 0(,%esi,8),%ecx
	movl -36(%ebp),%ebx
	movl %ebx,-104(%ebp)
	movl -20(%ebp),%esi
	decl %esi
	movl %esi,%edi
	imull -20(%ebp),%edi
	movl %edi,-100(%ebp)
	movl -100(%ebp),%ebx
	addl -12(%ebp),%ebx
	movl %ebx,-80(%ebp)
	movl -80(%ebp),%edi
	leal 0(,%edi,8),%esi
	movl -28(%ebp),%ebx
	movl %ebx,-100(%ebp)
	movl -12(%ebp),%edi
	imull -20(%ebp),%edi
	movl %edi,-84(%ebp)
	movl -84(%ebp),%ebx
	addl -4(%ebp),%ebx
	movl %ebx,-88(%ebp)
	movl -88(%ebp),%edi
	leal 0(,%edi,8),%edi
	movl %edi,-92(%ebp)
	movl -32(%ebp),%ebx
	movl %ebx,-96(%ebp)
	movl -100(%ebp),%edi
	fldl (%edi,%esi)
	movl -92(%ebp),%ebx
	movl -96(%ebp),%edi
	fmull (%edi,%ebx)
	movl -104(%ebp),%ebx
	fldl (%ebx,%ecx)
	faddp %st,%st(1)
	fstpl (%edx,%eax)
.L37:
	incl -12(%ebp)
	jmp .L35
	.p2align 4,,7
.L36:
.L33:
	incl -4(%ebp)
	jmp .L31
	.p2align 4,,7
.L32:
.L1:
	leal -116(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe1:
	.size	 matmul,.Lfe1-matmul
	.ident	"GCC: (GNU) egcs-2.91.66 19990314/Linux (egcs-1.1.2 release)"
