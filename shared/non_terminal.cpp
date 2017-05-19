#include "non_terminal.h"

non_terminal::non_terminal()
{

}

non_terminal::non_terminal(const std::string& value)
    : value_(value)
{

}

bool non_terminal::is_terminal() const
{
    return false;
}

const std::string& non_terminal::get_value() const
{
    return value_;
}

non_terminal::~non_terminal()
{

}

std::ostream& operator<<(std::ostream& os, const non_terminal& val)
{
    os << val.value_;
    return os;
}
