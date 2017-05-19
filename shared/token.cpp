#include "token.h"

token::token()
{
}

token::~token()
{
}

std::ostream& operator<<(std::ostream& os, const token& token)
{
    os << token.get_value();
    return os;
}
