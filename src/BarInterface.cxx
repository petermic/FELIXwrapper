#include "BarInterface.h"
#include "flxcard/FlxCard.h"
#include <boost/exception/all.hpp>
#include <exception>

namespace AliceO2 {
namespace roc {

BarInterface::BarInterface(std::shared_ptr<CardInterface> ci,int bar_index)
{
    if(bar_index<0 || bar_index >2) BOOST_THROW_EXCEPTION(std::range_error("BAR index out of range"));
    card = ci;
    _bar_ind = bar_index;
    _isBar2 = (bar_index == 2);
    _base_add = ci->getBarBase(bar_index);
}

BarInterface::BarInterface(int card_n, u_int lock_mask, int bar_index)
{
    if(bar_index<0 || bar_index >2) BOOST_THROW_EXCEPTION(std::range_error("BAR index out of range"));
    card = std::make_shared<CardInterface>(card_n,lock_mask);
    _bar_ind = bar_index;
    _isBar2 = (bar_index == 2);
    _base_add = card->getBarBase(bar_index);
}

void BarInterface::writeRegister(u_long addr, u_long data)
{
    if(_isBar2) card->set_reg(addr,data);
    else
    {
        BOOST_THROW_EXCEPTION(std::logic_error("BAR0 and BAR1 write not yet implemented"));
    }
}

u_long BarInterface::readRegister(u_long addr)
{
    if(_isBar2) return card->get_reg(addr);
    else
    {
        BOOST_THROW_EXCEPTION(std::logic_error("BAR0 and BAR1 read not yet implemented"));
    }
}

} // namespace roc
} // namespace AliceO2