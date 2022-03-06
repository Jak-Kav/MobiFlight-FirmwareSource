//
// LCDDisplay.h
//
// (C) MobiFlight Project 2022
//

#pragma once
#include <stdint.h>

namespace LCDDisplay
{
void Add(uint8_t address, uint8_t cols, uint8_t lines);
void Clear();
void OnSet();
void OnSetCustom();
}

// LCDDisplay.h
