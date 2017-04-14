#pragma once
#include "token.h"
class non_terminal
	: public token
{
public:

	non_terminal()
	{

	}

	non_terminal(const std::string& value)
		: value_(value)
	{

	}

	bool is_terminal() const override
	{
		return false;
	}
	
	const std::string& get_value() const override
	{
		return value_;
	}

	~non_terminal()
	{

	}

	friend std::ostream& operator<<(std::ostream& os, const non_terminal& val)
	{
		os << val.value_;
		return os;
	}

private:
	std::string value_;
};

