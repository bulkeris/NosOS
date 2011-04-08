#pragma once

typedef struct {
	unsigned char ram[64];
	unsigned char AX, BX, IP, SF;
} vmdata;

typedef struct {
	unsigned char ram[256];
	unsigned char AX, BX, IP, SF, PTR, PI, SI, TI, MODE, CN1, CN2, CN3, IOI;
} rmdata;