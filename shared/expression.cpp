#include "expression.h"

expression::expression()
{
}

void expression::push_back(const std::shared_ptr<token>& token)
{
    tokens_.emplace_back(token);
}

expression::container::const_iterator expression::begin() const
{
    return tokens_.cbegin();
}

expression::container::const_iterator expression::end() const
{
    return tokens_.cend();
}

expression::container::iterator expression::begin()
{
    return tokens_.begin();
}

expression::container::iterator expression::end()
{
    return tokens_.end();
}

size_t expression::size() const
{
    return tokens_.size();
}

std::shared_ptr<token>& expression::operator[](size_t i)
{
    return tokens_[i];
}

std::shared_ptr<token> expression::operator[](size_t i) const
{
    return tokens_[i];
}

void expression::clear()
{
    tokens_.clear();
}

void expression::resize(size_t n)
{
    tokens_.resize(n);
}

bool expression::only_non_terminals() const
{
    for (const auto& v : tokens_)
    {
        if (v->is_terminal())
        {
            return false;
        }
    }
    return true;
}

bool expression::only_terminals() const
{
    for (const auto& v : tokens_)
    {
        if (!v->is_terminal())
        {
            return false;
        }
    }
    return true;
}

bool expression::only_terminals(const std::vector<std::shared_ptr<token>>& tokens) const
{
    for (const auto& v : tokens_)
    {
        if (!v->is_terminal() && std::find_if(tokens.begin(), tokens.end(), [&](const auto& x) { return v->get_value() == x->get_value(); }) == tokens.end())
        {
            return false;
        }
    }
    return true;
}

bool expression::is_epsilon(const std::vector<std::shared_ptr<non_terminal>>& vec) const
{
    for (const auto& v : tokens_)
    {
        if (v->is_terminal() || std::find_if(vec.begin(), vec.end(), [&](const auto& x) { return v->get_value() == x->get_value(); }) == vec.end())
        {
            return false;
        }
    }
    return true;
}

size_t expression::count_terminals() const
{
    size_t count = 0;
    for (const auto& v : tokens_)
    {
        count += v->is_terminal();
    }
    return count;
}

std::ostream& operator<<(std::ostream& os, const expression& expr)
{
    for (const auto& v : expr)
    {
        os << std::string(" ") << *v;
    }
    return os;
}
