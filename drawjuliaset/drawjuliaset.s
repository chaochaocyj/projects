.data
num1: .word 1500
num2: .word 480000
num3: .word 240000
num4: .word 4000000

.text
  .global drawjuliaset

  drawjuliaset :
    stmfd sp!, {r4-r11, lr}
    mov   r11, r0    @ r11 = cY
    ldr   r4, =frame    @ r4 = frame
    mov   r5, #0    @ r5 = x, x = 0

  forX :
    cmp   r5, #640  @ if ( x < 640 )
    bge   overX
    mov   r6, #0    @ r6 = y, initialize y = 0,

  forY :
    cmp   r6, #480  @ if ( y < 480 )
    bge   overY

    ldr   r0, =num1    @ r0 = 1500
    ldr   r0, [r0]
    mul   r0, r0, r5       @ r0 = 1500x
    ldr   r1, =num2  @ r1 = 480000 (640 >> 1 = 320, 320*1500 = 480000)
    ldr   r1, [r1]
    sub   r0, r1           @ r0 = 1500x - 480000,op1
    mov   r1, #320         @ r1 = 320
    bl    __aeabi_idiv     @ r0 = ( 1500x - 480000 ) / 320
    mov   r8, r0           @ r8 = ( 1500x - 480000 ) / 320

    mov   r0, #1000        @ r0 = 1000
    mul   r0, r0, r6       @ r0 = 1000y
    ldr   r1, =num3        @ r1 = 240000 (480 >> 1 = 240, 240*1000 = 240000)
    ldr   r1, [r1]
    sub   r0, r1           @ r0 = 1000y - 240000
    mov   r1, #240         @ r1 = 240
    bl    __aeabi_idiv     @ r0 = ( 1000y - 240000 ) / 240
    mov   r9, r0           @ r9 = ( 1000y - 240000 ) / 240

    mov   r7, #255         @ r7 = i, i = maxIter = 255
    mul   r0, r8, r8       @ r0 = zx * zx (r8 = zx)
    mul   r1, r9, r9       @ r1 = zy * zy (r9 = zy)

    add   r2, r0, r1       @ r2 = zx * zx + zy * zy
    ldr   r3, =num4        @ r3 = 4000000
    ldr   r3, [r3]

    mov   r10, #0          @tmp = 0
    cmp   r10, #1
    cmpne r2, r3           @ zx * zx + zy * zy < 4000000,cd1
    bge   overWhile
    cmplt r7, #0           @ i > 0,cd2
    ble   overWhile


  While :
    sub   r0, r1           @ r0 = zx * zx - zy * zy
    mov   r1, #1000
    bl    __aeabi_idiv     @ r0 = ( zx * zx - zy * zy ) / 1000
    sub   r10, r0, #700    @ r10 = ( zx * zx - zy * zy ) / 1000 - 700
                           @ r10 = tmp , cx = -700,op2

    mul   r0, r8, r9       @ r0 = zx * zy (r8 = zx, r9 = zy)
    mov   r1, #500         @ r1 = 500 (2/1000 = 1/500)
    bl    __aeabi_idiv     @ r0 = zx * zy / 500
    add   r9, r0, r11      @ zy = zx*zy/500 + cY (r9 = zy, r11 = cY)

    mov   r8, r10          @ zx = tmp (r8 = zx, r10 = tmp)

    sub   r7, #1           @ i--
    mul   r0, r8, r8       @ r0 = zx * zx (r8 = zx)
    mul   r1, r9, r9       @ r1 = zy * zy (r9 = zy)
    add   r2, r0, r1       @ r2 = zx * zx + zy * zy
    ldr   r3, =num4 @ r3 = 4000000
    ldr   r3, [r3]

    cmp   r2, r3           @ zx * zx + zy * zy < 4000000
    bge   overWhile
    cmp   r7, #0           @ i > 0
    ble   overWhile
    b     While

  overWhile :
    and   r7, r7, #0xff      @ r7 = i, i = i & 0xff

    orr   r7, r7, r7, lsl #8 @ r7 = (i & 0xff) | ((i & 0xff) << 8),op3

    ldr   r0, =0xffff        @ r0 = 0xffff
    bic   r7, r0, r7         @ r7 = (~r7) & 0xffff = color

    mov   r0, r4             @ r0 = frame
    mov   r1, #1280          @ r1 = 1280 (640*2)
    mul   r1, r1, r6         @ r1 = 1280y (r6 = y)
    add   r0, r1             @ r0 = frame + 1280y
    add   r0, r5, lsl #1     @ r0 = frame + 1280y + x*2^1(r5 = x)

    str  r7, [r0]           @ frame[y][x] = color

    add   r6, #1            @ y++
    b     forY

  overY :
    add   r5, #1            @ x++
    b     forX

  overX :
    add r3, r3, ror #1      @ op4
    add lr, r0, pc          @ must
    ldmfd sp!, {r4-r11, lr}
    mov   pc, lr

