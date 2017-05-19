#pragma once
#include "token.h"
#include "non_terminal.h"
#include <memory>
#include <vector>
#include <algorithm>
#include <string>

class expression
{
public:

	typedef std::vector<std::shared_ptr<token>> container;

	expression();

	void push_back(const std::shared_ptr<token>& token);

	container::const_iterator begin() const;

	container::const_iterator end() const;

	container::iterator begin();

	container::iterator end();

	size_t size() const;

	std::shared_ptr<token>& operator[](size_t i);

	std::shared_ptr<token> operator[](size_t i) const;

	void clear();

	void resize(size_t n);

	template<typename It1>
	void erase(It1 a, It1 b);

	bool only_non_terminals() const;

	bool only_terminals() const;

	bool only_terminals(const std::vector<std::shared_ptr<token>>& tokens) const;

	bool is_epsilon(const std::vector<std::shared_ptr<non_terminal>>& vec) const;

	size_t count_terminals() const;

	friend std::ostream& operator<<(std::ostream& os, const expression& expr);

private:
	container tokens_;
};

template<typename It1>
void expression::erase(It1 a, It1 b)
{
    tokens_.erase(a, b);
}
