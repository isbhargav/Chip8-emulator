#include <iostream>

using namespace std;
/*
 * Link to Chip 8 refrence : http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
Chip 8 Memory Map (total memory 4kb):
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM

 Registers:
    -  There are total 16 (8 bit) registers named as V0,V1,...Vf
    -  There is also special 16 bit I register which stores address lower 12 bits(0x000-0x1ff)
    -  Vf register is not used by program, it used as flag register
    - Program Counter (PC) should be 16 bit (points to address of next instruction to be executed)
    - Stack pointer should be 8 bit (points to top of the stack)
    - Stack is an array of 16 16bit value, address to return to after the subroutine has finished

Instructions
    - Chip-8 language includes 36 different instructions
    - All instructions are 2 bytes long and are stored most-significant-byte first
    - In memory, the first byte of each instruction should be located at an even addresses.
    - If a program includes sprite data, it should be padded so any instructions following it will be properly situated in RAM.

 */

int main() {
    return 0;
}
