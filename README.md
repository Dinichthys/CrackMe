# "Crack Me" project

## My file

My prgram is set in CrackMe.asm. This program got a password and compare it with '\0\0\0'.

### Easier weakness

The easier weakness was an empty password which was difined as a true one, because the space allocated for the password was filled with zero-elements. 

### Harder weakness

The harder weakness was a stack overflow, which should be used to change the returning address to skip commparing password.

## Cracking the program

I got a program from [Urodish] (https://github.com/kzueirf12345) for cracking.

