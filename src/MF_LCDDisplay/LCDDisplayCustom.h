#pragma once

namespace LCDDisplayCustom
{
void Add(uint8_t CS);
void AddFCU(uint8_t CS = 13, uint8_t CLK = 12, uint8_t DATA = 8);
void AddEFIS(uint8_t CS = 2, uint8_t CLK = 3, uint8_t DATA = 4);
void Clear();
void OnSet();
}
