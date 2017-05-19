#include "terminal.h"

terminal::terminal(const std::string& value)
    : value_(value)
{

}

bool terminal::is_terminal() const
{
    return true;
}

const std::string& terminal::get_value() const
{
    return value_;
}

terminal::~terminal()
{

}
