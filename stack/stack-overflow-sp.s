	.file	"stack-overflow.c"
	.text
.globl function
	.type	function, @function
function:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$72, %esp
	movl	8(%ebp), %eax
	movl	%eax, -44(%ebp)
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	movl	-44(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	strcpy
	movl	-12(%ebp), %eax
	xorl	%gs:20, %eax
	je	.L3
	call	__stack_chk_fail
.L3:
	leave
	ret
	.size	function, .-function
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$288, %esp
	movl	%gs:20, %eax
	movl	%eax, 284(%esp)
	xorl	%eax, %eax
	movl	$0, 24(%esp)
	jmp	.L5
.L6:
	movl	24(%esp), %eax
	movb	$65, 28(%esp,%eax)
	addl	$1, 24(%esp)
.L5:
	cmpl	$254, 24(%esp)
	jle	.L6
	leal	28(%esp), %eax
	movl	%eax, (%esp)
	call	function
	movl	$0, %eax
	movl	284(%esp), %edx
	xorl	%gs:20, %edx
	je	.L8
	call	__stack_chk_fail
.L8:
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20120305 (Red Hat 4.4.6-4)"
	.section	.note.GNU-stack,"",@progbits
