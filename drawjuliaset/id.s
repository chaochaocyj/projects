.data
    msga:   .asciz "*****Input ID*****\n"
    msgb:   .asciz "** Please Enter Member 1 ID : **\n"
    msgc:   .asciz "** Please Enter Member 2 ID : **\n"
    msgd:   .asciz "** Please Enter Member 3 ID : **\n"
    msge:   .asciz "** Please Enter Command **\n"
    msgf:   .asciz "*****Print Team Member ID and ID Summation*****\n"
    msgg:   .asciz "ID Summation = %d\n"
    msgh:   .asciz "*****End Print*****\n"
    endl:   .asciz "\n"
    intendl:.asciz "%d\n"
    Pr:     .asciz "p"
    Instr:  .asciz "%s"
    Inint:  .asciz "%d"

    command: .space 80

.text
    .global id
    @.global ida
    @.global idb
    @.global idc
    @.global ids


    id:
        stmfd sp!, {lr}
        ldr r0, =msga
        bl printf

        ldr r0, =msgb
        subs r1, pc, lr
        bl printf
        ldr r0, =Inint
        ldr r1, =ida
        bl scanf

        ldr r0, =msgc
        bl printf
        ldr r0, =Inint
        ldr r1, =idb
        bl scanf

        ldr r0, =msgd
        bl printf
        ldr r0, =Inint
        ldr r1, =idc
        bl scanf

        @ Add------------
        mov r0, #0
        mov r1, #0
        cmp r0,r1

        ldr r1, =ida
        ldr r1, [r1]
        ldr r2, =idb
        ldr r2, [r2]
        add r3, r1, r2  @ addeq?
        ldr r1, =idc
        ldr r1,[r1]
        add r3, r3, r1

        ldr r5, =ids
        str r3, [r5]
        @end-add------------

        ldr r0, =msge
        bl printf

        ldr r0, =Instr
        ldr r1, =command
        bl scanf

        ldr r0, =Pr
        ldrb r0,[r0]
        ldr r1, =command
        ldr r1, [r1]
        cmp r0, r1
        blne isNotp

        ldr r0, =msgf
        bl printf

        ldr r0, =intendl
        ldr r1, =ida

        ldr r1, [r1]
        bl printf

        ldr r0, =intendl
        ldr r1, =idb


        ldr r1, [r1]
        bl printf

        ldr r0, =intendl
        ldr r1, =idc


        ldr r1, [r1]
        bl printf

        ldr r0, =endl
        bl printf

        ldr r0, =msgg
        ldr r1, =ids
        ldr r1, [r1]
        bl printf

        ldr r0, =msgh
        bl printf

        ldmfd sp!, {lr}
        mov pc, lr

    isNotp:
        ldr r0, =msgh
        bl printf
        mov pc, lr
