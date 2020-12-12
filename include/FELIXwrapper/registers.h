#ifndef REGISTERS_H
#define REGISTERS_H

#include <cstdint>

namespace AliceO2 {
namespace roc {
namespace Cru {
namespace ScRegisters {

uint32_t SC_LINK = 0xe100;
uint32_t SC_RESET = 0xe110;
uint32_t SWT_WR_WORD_L = 0xe160;
uint32_t SWT_WR_WORD_M = 0xe164;
uint32_t SWT_WR_WORD_H = 0xe170;
uint32_t SWT_MON = 0xe1b0;
uint32_t SWT_CMD = 0xe180;
uint32_t SWT_RD_WORD_L = 0xe190;
uint32_t SWT_RD_WORD_M = 0xe194;
uint32_t SWT_RD_WORD_H = 0xe1a0;

} // namespace ScRegs
} // namespace Cru
} // namespace roc
} // namespace AliceO2

#endif
