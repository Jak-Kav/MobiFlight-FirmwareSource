#pragma once

namespace LCDDisplayCustom
{
void Add(uint8_t TYPE);
void AddFCU(uint8_t CS = 12, uint8_t CLK = 11, uint8_t DATA = 8);       // Default values
void AddEFIS(uint8_t CS = 32, uint8_t CLK = 33, uint8_t DATA = 34);     // Default values
void AddEFIS2(uint8_t CS = 35, uint8_t CLK = 36, uint8_t DATA = 37);     // Default values
void Clear();
void OnSet();
}
