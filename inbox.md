# Inbox

## Reading

- [ ] reading about `setuid`, file permissions and mode, SSL/TLS encryption
- [ ] Unix job control fg,bg electrical
- [ ] netcat, netstat, ss
- [ ] gdb

## High perfomance computing

Notes from [Algorithmica](https://en.algorithmica.org/)

### Dennard Scaling

- reducing transistor size by 30%, doubles the transistor density ($0.7^2 \approx 0.5$), increase the clock speed by 40% ($\frac{1}{0.7} \approx 0.4$), leaves overall power density same. Also came to be known as Moore's law.
- thermodynamically a computer is an efficient device to convert electrical power to heat, which has limitaon the size of crystals. Moore's law and Dennard scaling are just obserevations they have physical limitations. dennard scaling has stopped due to power issue.
- if transistor become small their capacitance decreases, increasing the clock frequency ($\frac{1}{\omega C}$). also requiring less required voltage to flip.
- **Leakage effect** : due to smaller sizes of transistor, electrons are not behaving as the should due to magnetic fields and other things.
- we cant scale down transistors (Dennard scaling). But we are still doubling the no. of transistors (Moore's law) and we are using those to do execute multiple instructions in single clock cycle.

### Modern computing and Hardware

- **pipelining** overlapping execution of instruction to keep every part of cpu busy
- **out of order execution** executing different part of programs without waiting for previous to complete.
- **SIMD** executing the same operation on 128, 256, 512 bit of data split into groups
- **parallel computing, gpu** multiple identical cores
- **distributed computing** adding multiple chips
- **ASICs FPGAs** custom hardware to solve a specific problem
- **superscalar processor** adding multiple parallel execution units to same cpu

> `nm` does not mean nanometer anymore, it is just for marketing.

- `compiled` and `interpreted` languages from programmer's perspective. `native` and `runtime` for cpu perspective. Java is compiled but needs runtime.
- `JIT (just in time) compilation` is feature to compile some of the part of code to machine code. It is feature of implementation not the language.

### Assembly language (x86)

A sample assembly program to add 2 numbers

```asm
; *a = rsi, *b = rdi, *c = rdx
mov eax, DWORD PTR [rsi]  ; load 4 bytes from wherever rsi points into eax
add eax, DWORD PTR [rdi]  ; add whatever is stored at rdi to eax
mov DWORD PTR [rdx], eax  ; write contents of eax to wherever rdx points
```

- `;` is used for comments
- `[reg]` is to derefrence the pointer `reg` register is storing. in _x86_ you have to prefix with size `DWORD` means 32bit.
- [x86 assembly ref](https://www.felixcloutier.com/x86/)
- most instruction write result into first operand
- general purpose registers are `rax`, `rbx`, `rcx`, `rdx`, `rdi`, `rsi`, `rbp`, `rsp` and `r8` to `r15`. Some registers are named becaurse of historical reasons. total 16
- 32-bit, 16-bit, and 8-bit registers also exist, aliased with above. `rax`->`eax`->`ax`->`al`, lower 32 bit of `rax` are `eax` and likewise for 16- and 8-bits.
- same instruction can have different flavour (take different operands and different execution time)

- Address operand: `SIZE PTR [base + index*scale + displacement]`, `displacement` is int constant and `scale` can be `2`, `4` or `8`. It calculates the expression inside bracket and dereference it. This **complex dereferencing** is usually slower than normal dereferencing (you dont have to calculate).
- `BYTE`, `WORD`, `DWORD` and `QWORD` are _sizes_ for 8-, 16-, 32- and 64-bits respectively. (case insensetive)
- More sizes also exist for 80-, 128-, 256- and 512-bits.
- `lea` instruction does the address calc in one cycle, and stores in register. We can also use it for arithmetic like calculating `a = b+c+d` if d is contant.

On linux _GAX (GNU Assembler)_ which produce assembly in _AT&T_ syntax. The difference in _Intel_ and _AT&T_ is given below

- _AT&T_ instructions need to be suffixed with the size operands. `b` for byte, `w` for word(16 bits), `l` for 32-bit int or 64-bit float, `q` for quad (64-bits), `s` for 32-bit floating point and `t` for ten bytes
- last operand is used to specify destination of result
- registers are prefixed by `%`, and constants with `$`. `%rax` and `$1`;
- memory addressing: displacement(%base, %index, scale);
- `#` can also be used for comments

- `jmp` instruction jump without checking anything
- `FLAG` register is there to check if you should jump or not for `jne`

```asm
loop:
    add edx, DWORD PTR [rax]    ; add *rax(start of array) to edx
    add rax, 4                  ; add 4 to rax
    cmp rax, rcx                ; cmp rax with rcx(end of array) and store result in FLAG
    jne loop                    ; jump to loop after checking FLAG
```

- In loop unrolling we group the instruction to together, instead of updating a value of 16 elements array in 16 iteration, update 4 values together in 4 iterations.

Below is one instruction shorter version of the above add code.

```asm
    mov rax,  -100              ; 100 (25 times 4) is the array size
loop:
    add edx, DWORD PTR [rax + 100 + rcx]  ; this will give ith element
    add rax, 4                            ; add 4 to rax, rax will become 0 at the end
    jnz  loop                             ; jump if 0, add updates the flag if 0,overflow or underflow
```

### Function and Recursion

- `rbp`: stack base pointer, points to the start of the stack
- `rsp`: stack pointer, points to the top of the stack
- to call a function push all the registers to the stack, then push the `rip`, and then go to the function, when exiting read from top of the stack.
- for syntactic sugar we have below 4 instructions to play with stack
  - `push`, writes data to stack, decrements stack pointer
  - `pop`, reads data from stack, increment stack pointer
  - `call`, puts the address of the next instruction on top of the stack, and jumps to label
  - `ret`, reads return address from top of the stack and jumps to it
- stack grows toward lower memory address
- memory between `rsp` and `rbp` is called _stack frame_, usually **8MB**.
- _function inlining_, is stitching the caller function code into callee, to save the pain of working with stack.
- _tail recursive_, function returns right after making a recursive call, so we dont have anything to story on stack, saving time and memory

- `jmp rax`, jump by a non constant value

### Machine code layout

- cpu pipeline is divided into 2 parts, _frontend_ where the instructions are fetched from the memory and decoded and _backend_ when the are scheduled and finally executed.
- **fetch stage**, a fixed size (32bytes on x86) is fetched and load from a memory address.
- **decode stage** cpu discard everything before instruction pointer, split rest of them in instructions
- machine instructions are encoded using variable number of bytes
