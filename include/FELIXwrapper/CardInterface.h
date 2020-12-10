#ifndef CARDINTERFACE_H
#define CARDINTERFACE_H

#include "flxcard/FlxCard.h"
#include <memory>

namespace AliceO2{
namespace roc{

class CardInterface 
{
public:
    CardInterface(int card_n, u_int lock_mask);
    ~CardInterface();
    u_long getBarBase(int ind);
    std::shared_ptr<FlxCard> getCard() {return _card;}
private:
    std::shared_ptr<FlxCard> _card;
    u_long _bar_base[3];
};

} // namespace roc
} // namespace AliceO2
#endif