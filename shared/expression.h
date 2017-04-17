#pragma once
#include "token.h"
#include "non_terminal.h"
#include <memory>
#include <vector>
#include <algorithm>
class expression
{
public:

	typedef std::vector<std::shared_ptr<token>> container;

	expression()
	{
	}

	void push_back(const std::shared_ptr<token>& token)
	{
		tokens_.emplace_back(token);
	}

	container::const_iterator begin() const
	{
		return tokens_.cbegin();
	}

	container::const_iterator end() const
	{
		return tokens_.cend();
	}

	container::iterator begin()
	{
		return tokens_.begin();
	}

	container::iterator end()
	{
		return tokens_.end();
	}

	size_t size() const
	{
		return tokens_.size();
	}

	std::shared_ptr<token>& operator[](size_t i)
	{
		return tokens_[i];
	}

	std::shared_ptr<token> operator[](size_t i) const
	{
		return tokens_[i];
	}

	void clear()
	{
		tokens_.clear();
	}

	void resize(size_t n)
	{
		tokens_.resize(n);
	}

	template<typename It1>
	void erase(It1 a, It1 b)
	{
		tokens_.erase(a, b);
	}

	bool only_non_terminals() const
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

	bool only_terminals() const
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

	bool only_terminals(const std::vector<std::shared_ptr<token>>& tokens) const
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

	bool is_epsilon(const std::vector<std::shared_ptr<non_terminal>>& vec) const
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

	size_t count_terminals() const
	{
		size_t count = 0;
		for (const auto& v : tokens_)
		{
			count += v->is_terminal();
		}
		return count;
	}

	friend std::ostream& operator<<(std::ostream& os, const expression& expr)
	{
		for (auto & v : expr)
		{
			os << " " << *v;
		}
		return os;
	}

private:
	container tokens_;
};

