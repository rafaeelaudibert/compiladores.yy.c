# DATA SECTION
.section .rdata,"dr"
printint:   .ascii "%d\0"
printstring:    .ascii "%s\0"

_a: .long 0x6
    


# FUNÇÕES
	.globl	_main
_main:
    # TAC BEGIN_FUNC
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp

    # TAC PRINT (INT)
    movl    _a, %eax
	movl	%eax, 4(%esp)
	movl	$printint, (%esp)
	call	_printf

    # TAC RETURN
	movl	$0, %eax

    # TAC END_FUNC
	leave
	ret
