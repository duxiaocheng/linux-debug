	.file	"stack-example.c"
	.text
.globl function
	.type	function, @function
function:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$0, -5(%ebp)
	movb	$0, -1(%ebp)
	movl	$0, -15(%ebp)
	movl	$0, -11(%ebp)
	movw	$0, -7(%ebp)
	leave
	ret
	.size	function, .-function
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	movl	$3, 8(%esp)
	movl	$2, 4(%esp)
	movl	$1, (%esp)
	call	function
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20120305 (Red Hat 4.4.6-4)"
	.section	.note.GNU-stack,"",@progbits
