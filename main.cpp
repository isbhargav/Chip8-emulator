#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>

/*
 * Link to Chip 8 refrence : http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
Chip 8 Memory Map (total memory 4kb):

|-----8-Bits----|

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

    - 0x000-0x1FF - Chip 8 interpreter (contains font set in emu) [First 512 bytes are reserved]
    - 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F) [Part of reserved (80) bytes for font set]
    - 0x200-0xFFF - Program ROM and work RAM [Part of Ram that can be used by programs]

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

Graphics
    - The graphics system: The chip 8 has one instruction that draws sprite to the screen. Drawing is done in XOR mode and
        if a pixel is turned off as a result of drawing, the VF register is set. This is used for collision detection.
    - The graphics of the Chip 8 are black and white and the screen has a total of 2048 pixels (64 x 32).
        This can easily be implemented using an array that hold the pixel state (1 or 0):
 */



uint16_t opcode ; // store op code
uint8_t memory[0x1000]; // 4KB memory
uint8_t V[16]; // 16 8 bit genral purpose register

uint16_t PC ; // 16 bit PC register
uint8_t SP ; // 16 bit SP register [SP always points to last address to pop]
uint16_t I ; // 16 bit index register I
uint16_t stack[16]; // 16 level stack

uint8_t gfx[64 * 32] ; // black & white screen

uint8_t delay_timer; // timer register
uint8_t sound_timer; // timer register

uint8_t key[16]; //  array to store the current state of the key

uint8_t chip8_fontset[80]; // chip 8 font set

void intitialize_chip8(uint8_t program[], int n){
    PC =  0x200; // PC starts at 0x200
    opcode = 0; // Reset opcode
    I = 0; // Reset index register
    SP = 0; // Reset stack pointer

    memset(gfx, 0, sizeof(gfx));       // clear display
    memset(stack, 0 , sizeof(stack));  // clear stack
    memset(V, 0, sizeof(V));           // clear registers
    memset(memory, 0, sizeof(memory)); // clear memory

    // load font set
     memcpy(memory+0x05, chip8_fontset, sizeof(chip8_fontset)); // copy 80 bytes

    // reset timers
    delay_timer = 0;
    sound_timer = 0;

    // Loading the program into the memory
    memcpy(memory + 0x200, program, n);
}



#define MAX 3584
int main() {
    std::string test_prg = "../test.ch8"; // program to load
    std::ifstream file(test_prg, std::ios::binary); // read program
    uint8_t program[MAX];

    file.read((char *)program,MAX);
    if(!file.eof())
    {
        std::cerr<<"Fail to read complete file";
        exit(1);
    }
    int n = file.gcount();// get total bytes read
    file.close();

   intitialize_chip8(program, n); // initailize registers and load program to memory
//   emulateCyle_chip8();




    // check if programing was copied correctly to memory
   for(int i=0;i<n;i++)
   {
       assert(program[i]==memory[0x200+i] && "Program copy failed");
       printf("%x ",program[i]);
   }

    return 0;
}




// 00e0 - clear screen
// c0ff - Rand Vx = rand() & 0xFF
// a224 -  I = 0x0224
// f033
// f265
// f029
// 6000
// 6300
// d035
// f129
// 6005
// d035
// f229
// 600a
// d035
// f00a
// 1200

