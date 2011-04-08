#pragma once
#include <windows.h>
#include "struct.h"
#include <stdio.h>

#define TITLE 42
#define OFFSET WHITE1
#define HEX WHITE1
#define HEXFF WHITE
#define ASCII WHITE1
#define REG YELLOW1
#define TYPE WHITE2
#define PTRS RED
#define PROG YELLOW

#define BLACK 0
#define WHITE 15
#define WHITE1 7
#define WHITE2 8
#define BLUE 11
#define BLUE1 9
#define BLUE2 1
#define GREEN 10
#define GREEN1 2
#define GREEN2 3
#define RED 12
#define RED1 4
#define YELLOW 14
#define YELLOW1 6
#define PINK 13
#define PINK1 5

INPUT_RECORD input[1];
DWORD w;

unsigned char text[512], mode;

unsigned char getProgramInput();


void draw(rmdata *realmachine, vmdata *virtualmachine);
void printAllColors();
