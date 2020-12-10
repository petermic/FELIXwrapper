#include "FELIXwrapper/CardInterface.h"
#include <boost/exception/all.hpp>
#include <exception>

namespace AliceO2 {
namespace roc {

CardInterface::CardInterface(int card_n, u_int lock_mask)
{
    _card = std::make_shared<FlxCard>();
    _card->card_open(card_n, lock_mask);
    for(int i=0;i<3;i++)
    {
        _bar_base[i] = _card->openBackDoor(i);
    }
}

CardInterface::~CardInterface()
{
    _card->card_close();
}

u_long CardInterface::getBarBase(int ind)
{
    if(ind<0 || ind>2) BOOST_THROW_EXCEPTION(std::range_error("BAR index out of range"));
    return _bar_base[ind];
}

} // namespace roc
} // namespace AliceO2