# "Crack Me" project

## My file

My prgram is set in CrackMe.asm. This program got a password and compare it with '\0\0\0'.

### Easier weakness

The easier weakness was an empty password which was difined as a true one, because the space allocated for the password was filled with zero-elements. 

![alt text](data/MyWeakness1.png)

### Harder weakness

The harder weakness was a stack overflow, which should be used to change the returning address to skip commparing password. This weakness exits because I write the given password to the stack and get too many symbols for the password so the user can rewrite stack.

## Cracking the program

I got a [program](VZLOM.COM) from [Urodish](https://github.com/kzueirf12345) for cracking.

### Easier weakness

The easier weakness was a password started with symbol '\0'. It was done with an 'if' at the start of the program.

![alt text](data/EasierWeakness1.png)

### Harder weakness

The harder weakness was a resident program with 21h interrupt which change the ip register and change the returning value to origin program. I change ip in stack to jump to printing function with good result. I did it in program [crack.asm](crack.asm). Another weakness with resident program was changing the address of a string, that he print, from string with bad result to the string with good result. I did it in program [crack_2.asm](crack_2.asm).

### Binary patch

The [program](src/main.cpp) in folder [src](src) open the program '.COM' and change the byte meaning JE command to JMP command in the comparison of the passwords. So in every start of the program it started unlocking with every password. The changed program is located in [cracked.com](cracked.com). Also my program make a new thread, in which it draws a Roflan, which moves like DVD player logotype. More than that it started a music from the file [VI_KA.mp3](data/VI_KA.mp3).
