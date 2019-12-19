#ifndef PIZZA_DEFAULT_GRAYSCALE_TABLE
#define PIZZA_DEFAULT_GRAYSCALE_TABLE

#include "Color.h"

// Default Pizza File Format 6bit grayscale table
const Color DEFAULT_GRAYSCALE_TABLE[64] = {
    {0, 0, 0},       {4, 4, 4},       {8, 8, 8},       {12, 12, 12},
    {16, 16, 16},    {20, 20, 20},    {24, 24, 24},    {28, 28, 28},
    {32, 32, 32},    {36, 36, 36},    {40, 40, 40},    {44, 44, 44},
    {48, 48, 48},    {52, 52, 52},    {56, 56, 56},    {60, 60, 60},
    {64, 64, 64},    {68, 68, 68},    {72, 72, 72},    {76, 76, 76},
    {80, 80, 80},    {84, 84, 84},    {88, 88, 88},    {92, 92, 92},
    {96, 96, 96},    {100, 100, 100}, {104, 104, 104}, {108, 108, 108},
    {112, 112, 112}, {116, 116, 116}, {120, 120, 120}, // Skip needed to
                                                       // have(255,255,255)
                                                       // white and (0,0,0)
                                                       // black
    {127, 127, 127}, {131, 131, 131}, {135, 135, 135}, {139, 139, 139},
    {143, 143, 143}, {147, 147, 147}, {151, 151, 151}, {155, 155, 155},
    {159, 159, 159}, {163, 163, 163}, {167, 167, 167}, {171, 171, 171},
    {175, 175, 175}, {179, 179, 179}, {183, 183, 183}, {187, 187, 187},
    {191, 191, 191}, {195, 195, 195}, {199, 199, 199}, {203, 203, 203},
    {207, 207, 207}, {211, 211, 211}, {215, 215, 215}, {219, 219, 219},
    {223, 223, 223}, {227, 227, 227}, {231, 231, 231}, {235, 235, 235},
    {239, 239, 239}, {243, 243, 243}, {247, 247, 247}, {251, 251, 251},
    {255, 255, 255}};

#endif