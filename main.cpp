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

// chip 8 font set
uint8_t chip8_fontset[80]={
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

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


void emulateCyle_chip8(){
    // Fetch opcode
    // Big Endian hence, MSB is at lower address
    uint8_t MSB = memory[PC];
    uint8_t LSB = memory[PC+1];
    opcode = (MSB<<8) | LSB;


    // Decode and Execute
    switch (opcode & 0xF000) {
        case 0x0000:{
            switch (opcode){
                case 0x00E0:{ // clear screen
                    memset(gfx, 0, sizeof(gfx));       // clear display
                    PC += 2;
                    break;
                }
                case 0x00EE:{ //return from subroutine
                    // restore address from stack and decrement SP
                    PC = stack[SP];
                    SP -= 1;
                    break;
                }
                default:{ assert(false && "Wrong Opcode"); }
            }
            break;
        }
        case 0x1000:{ // 1NNN: goto NNN
            uint16_t nnn = opcode & 0x0FFF;
            PC = nnn;
            break;
        }
        case 0x2000:{ // 2NNN: Calls subroutine at NNN.
            uint16_t nnn = opcode & 0x0FFF;
            SP += 1;
            stack[SP] = PC;
            PC = nnn;
            break;
        }
        case 0x6000:{ // 6XNN: Sets VX to NN.
            uint16_t x = (opcode & 0x0F00)>>8;
            uint16_t nn = opcode & 0x00FF;
            V[x] = nn;
            PC += 2;
            break;
        }
        case 0xA000:{ // ANNN: set I to NNN
            I = opcode & 0x0FFF;
            PC += 2;
            break;
        }
        case 0xC000:{ // CXNN: Rand Vx = rand() & nn
            uint16_t x = (opcode & 0x0F00)>>8;
            uint8_t nn = opcode & 0x00FF;
            uint8_t r = (rand()%256);
            V[x] = r & nn;
            PC += 2;
            break;
        }
        case 0xD000:{ // AXYN:draw(Vx, Vy, N)
            uint16_t x = (opcode & 0x0F00)>>8;
            uint16_t y = (opcode & 0x00F0)>>4;
            uint16_t n = opcode & 0x000F;
            // draw(V[x],V[y],n)
            PC += 2;
            break;
        }
        case 0xF00:{
            switch (opcode & 0xF0FF){
                case 0xF00A:{  // wait for input
                    uint16_t x = (opcode & 0x0F00)>>8;
                    char c  = (char)getchar();
                    V[x] = c;
                    PC += 2;
                    break;
                }
                case 0xF033:{  // set_BCD(Vx) *(I+0) = BCD(3); *(I+1) = BCD(2); *(I+2) = BCD(1);
                    uint16_t x = (opcode & 0x0F00)>>8;
                    uint16_t n = V[x];
                    memory[I + 2] = (n%10);
                    n /= 10;
                    memory[I + 1] = (n%10);
                    n /= 10;
                    memory[I] = (n%10);
                    n /= 10;
                    PC += 2;
                    break;
                }
                case 0xF065:{  //reg_load(Vx, &I) Fills V0 to VX (including VX) with values from memory starting at address I.
                    uint16_t x = (opcode & 0x0F00)>>8;
                    for(int i=0;i<=x;i++)
                        V[i] = memory[I+i];
                    PC += 2;
                    break;
                }
                case 0xF029:{  //I = sprite_addr[Vx]
                    uint16_t x = (opcode & 0x0F00)>>8;
                    I = V[x];
                    PC += 2;
                    break;
                }
            }
        }

    }
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
   for(int i=0;i<n/2;++i)
   {
       emulateCyle_chip8();
   }




    // check if programing was copied correctly to memory
//   for(int i=0;i<n;i++)
//   {
//       assert(program[i]==memory[0x200+i] && "Program copy failed");
//       printf("%x ",program[i]);
//   }

    return 0;
}




//1  00e0 - clear screen
//2  c0ff - Rand Vx = rand() & 0xFF
//3  a224 - I = 0x0224
//4  f033 - set_BCD(Vx) *(I+0) = BCD(3); *(I+1) = BCD(2); *(I+2) = BCD(1);
//5  f265 - reg_load(Vx, &I) Fills V0 to VX (including VX) with values from memory starting at address I.
//6  f029 - I = sprite_addr[Vx]
//7  6000 - Sets VX to NN.
//8  6300 - Sets VX to NN.
//9  d035 -
//10  f129
//11  6005
//12  d035
//13  f229
//14  600a
//15  d035
//16  f00a
//17  1200

