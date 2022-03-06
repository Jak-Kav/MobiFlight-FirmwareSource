#pragma once

namespace LCDDisplayCustom
{
void Add(uint8_t CS = 13, uint8_t CLK = 12, uint8_t DATA = 8, uint8_t B_LIGHT = 10);
void Clear();
void OnSet();
}
