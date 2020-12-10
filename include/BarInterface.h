#ifndef BARINTERFACE_H
#define BARINTERFACE_H

namespace AliceO2 {
namespace roc {

#include "CardInterface.h"

class BarInterface {
public:
    BarInterface(CardInterface ci, int bar_index);
    BarInterface(int card_n, u_int lock_mask, int bar_index);
    ~BarInterface() { ~card(); }
    void writeRegister(u_long addr);
    u_long readRegister(u_long addr);
    std::shared_ptr<CardInterface> card;
private:
    u_long _base_add;
    int _bar_ind;
    bool _isBar2;
}

} // namespace roc
} // namespace AliceO2

#endif