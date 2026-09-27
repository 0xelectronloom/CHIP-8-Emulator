#pragma once
#include <string>
#include<fstream>
#include <random>
#include <iostream>

#define MEM_SIZE 4096


class CHIP_8
{
private:
	std::random_device rd;
	std::mt19937 rng{ rd() };
	std::uniform_int_distribution<int> random_byte{ 0, 255 };
public:
	CHIP_8(){}
	unsigned char chip8_fontset[80] =
	{
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
	bool running;
	unsigned short opcode;
	//memory
	unsigned char memory[MEM_SIZE] = {}; // 4KB
	//regs
	unsigned char V[16];

	unsigned short I;
	unsigned short pc;

	// Graphics
	unsigned char gfx[64 * 32];

	// Timers
	unsigned char delay_timer;
	unsigned char sound_timer;

	// Stack 
	unsigned short stack[16];
	unsigned char sp;
	//Keypad
	bool key[16] = {};

	void init()
	{
		opcode = 0;
		I = 0;
		pc = 0x200;
		sp = 0;
		running = false;
		std::fill(std::begin(memory), std::end(memory), 0);
		std::fill(std::begin(V), std::end(V), 0);
		std::fill(std::begin(gfx), std::end(gfx), 0);
		std::fill(std::begin(stack), std::end(stack), 0);
		std::fill(std::begin(key), std::end(key), false);

		delay_timer = 0;
		sound_timer = 0;

		for (int i = 0; i < 80; ++i)
		{
			memory[i] = chip8_fontset[i];
		}
	}
	void load_rom(const std::string& rom_path)
	{
		std::ifstream file(rom_path, std::ios::binary | std::ios::ate);

		if (!file)
		{
			throw std::runtime_error("Could not open ROM");
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		if (size > MEM_SIZE - 0x200)
		{
			throw std::runtime_error("ROM is too large");
		}

		file.read(
			reinterpret_cast<char*>(&memory[0x200]),
			size
		);
		running = true;
	}
	void emulateCycle() {

		// Fetch opcode
		fetch_opcode();
		pc += 2;

		// Decode opcode
		unsigned char x = (opcode & 0x0F00) >> 8;
		unsigned char y = (opcode & 0x00F0) >> 4;
		unsigned char n = (opcode & 0x000F);
		unsigned char nn = (opcode & 0x00FF);
		unsigned short nnn = (opcode & 0x0FFF);

		int res;
		switch ((opcode & 0xF000) >> 12)
		{
		case 0x0:
			if (opcode == 0x00E0)
			{
				// Clear Screen
				std::fill(std::begin(gfx), std::end(gfx), 0);
			}
			else if (opcode == 0x00EE)
			{
				// RET
				pc = stack[--sp];
				

			}
			else {
				// 0NNN
			}
			break;

		case 0x1:
			pc = nnn;
			break;
		case 0x2:
			stack[sp] = pc ;
			sp++;
			pc = nnn;
			break;

		case 0x3:
			// SE
			if (V[x] == nn)
				pc += 2;
			break;
		case 0x4:
			// SNE
			if (V[x] != nn)
				pc += 2;
			break;
		case 0x5:
			if (V[x] == V[y])
				pc += 2;
			break;
		case 0x6:
			V[x] = nn;
			break;
		case 0x7:
			V[x] += nn;
			break;
		case 0x8:
			switch (n) {
			case 0x0:
				V[x] = V[y];
				break;
			case 0x1:
				V[x] |= V[y];
				break;
			case 0x2:
				V[x] &= V[y];
				break;
			case 0x3:
				V[x] ^= V[y];
				break;
			case 0x4:
				res = V[x] + V[y];
				V[0xF] = res > 255;
				V[x] = res & 0xFF;
				break;
			case 0x5:
				V[0xf] = V[x] > V[y];
				V[x] = V[x] - V[y];

				break;
			case 0x6:
				V[0xf] = V[x] & 0x01;
				V[x] >>= 1;
				break;
			case 0x7:
				V[0xf] = V[y] > V[x];
				V[x] = V[y] - V[x];

				break;
			case 0xE:
				V[0xF] = (V[x] & 0x80) >> 7;
				V[x] <<= 1;
				break;

			}
			break;
		case 0x9: // Skip next instruction if Vx != Vy.

			if (V[x] != V[y])
				pc += 2;
			break;


		case 0xA: // ANNN: Sets I to the address NNN
			// Execute opcode
			I = nnn;
			break;

		case 0xB:
			pc = nnn + V[0];
			break;
		case 0xC:
			V[x] = static_cast<unsigned char>(random_byte(rng)) & nn;
			break;
		case 0xD:
		{
			V[0xF] = 0;

			for (int row = 0; row < n; row++)
			{
				unsigned char sprite = memory[I + row];

				for (int col = 0; col < 8; col++)
				{
					// Is this sprite pixel ON?
					if (sprite & (0x80 >> col))
					{
						int px = (V[x] + col) % 64;
						int py = (V[y] + row) % 32;

						int index = py * 64 + px;

						// Collision
						if (gfx[index])
							V[0xF] = 1;

						// XOR pixel
						gfx[index] ^= 1;
					}
				}
			}

			break;
		}
		case 0xE:
			switch (nn)
			{
			case 0x9E:
				// SKP Vx
				// Skip next instruction if key Vx is pressed
				if (V[x] < 16 && key[V[x]])
					pc += 2;
				break;

			case 0xA1:
				// SKNP Vx
				// Skip next instruction if key Vx is NOT pressed
				if (V[x] < 16 && !key[V[x]])
					pc += 2;
				break;

			default:
				printf("Unknown opcode: 0x%04X\n", opcode);
				break;
			}
			break;
		case 0xF:
			switch (nn)
			{
			case 0x07:
				// LD Vx, DT
				V[x] = delay_timer;
				break;

			case 0x0A:
				// LD Vx, K
			{
				//std::cout << "paused : x=" << std::hex << x <<std::endl;
				bool keyPressed = false;

				for (int i = 0; i < 16; ++i)
				{
					if (key[i])
					{
						V[x] = i;
						keyPressed = true;
						break;
					}
				}

				// No key pressed:
				// stay on this instruction
				if (!keyPressed)
					pc -= 2;
			}
			break;

			case 0x15:
				// LD DT, Vx
				delay_timer = V[x];
				break;

			case 0x18:
				// LD ST, Vx
				sound_timer = V[x];
				break;

			case 0x1E:
				// ADD I, Vx
				I += V[x];
				break;

			case 0x29:
				// LD F, Vx
				// Each font character is 5 bytes
				I = V[x] * 5;
				break;

			case 0x33:
				// LD B, Vx
				memory[I] = V[x] / 100;
				memory[I + 1] = (V[x] / 10) % 10;
				memory[I + 2] = V[x] % 10;
				break;

			case 0x55:
				// LD [I], Vx
				for (int i = 0; i <= x; ++i)
				{
					memory[I + i] = V[i];
				}
				break;

			case 0x65:
				// LD Vx, [I]
				for (int i = 0; i <= x; ++i)
				{
					V[i] = memory[I + i];
				}
				break;

			default:
				printf("Unknown opcode: 0x%04X\n", opcode);
				break;
			}
			break;
		default:
			printf("Unknown opcode: 0x%X\n", opcode);
		}



	}
	
	void updateTimers()
	{
		if (delay_timer > 0)
			--delay_timer;

		if (sound_timer > 0)
		{
			// sound handling later
			--sound_timer;
		}
	}
	void fetch_opcode() {
		opcode = memory[pc] << 8 | memory[pc + 1];

	}
};



