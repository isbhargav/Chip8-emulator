#include <iostream>

using namespace std;
/*
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
 There are total 16 (8 bit) registers named as V0,V1,...Vf
 There is also special 16 bit I register which stores address lower 12 bits(0x000-0x1ff)
 Vf register is not used by program, it used as flag register


 */

int main() {
    return 0;
}
