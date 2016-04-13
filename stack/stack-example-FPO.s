	.file	"stack-example.c"
	.text
.globl function
	.type	function, @function
function:
	subl	$16, %esp
	movl	$0, 11(%esp)
	movb	$0, 15(%esp)
	movl	$0, 1(%esp)
	movl	$0, 5(%esp)
	movw	$0, 9(%esp)
	addl	$16, %esp
	ret
	.size	function, .-function
.globl main
	.type	main, @function
main:
	subl	$12, %esp
	movl	$3, 8(%esp)
	movl	$2, 4(%esp)
	movl	$1, (%esp)
	call	function
	movl	$0, %eax
	addl	$12, %esp
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20120305 (Red Hat 4.4.6-4)"
	.section	.note.GNU-stack,"",@progbits
