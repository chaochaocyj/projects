.data
    msga:   .asciz"*****Print Name*****\n"
    msgb:   .asciz"*****End Print*****\n"
    @tnum:   .asciz"Team 8\n"

.text
    .global name
    @.global tnum
    @.global mema
    @.global memb
    @.global memc

name:
    stmfd sp!, {lr}
    ldr r0, =msga
    bl printf
    ldr r0, =tnum
    bl printf
    ldr r0, =mema
    bl printf
    ldr r0, =memb
    bl printf
    ldr r0, =memc
    bl printf
    ldr r0, =msgb
    bl printf


    ldmfd sp!, {lr}
    mov pc, lr
