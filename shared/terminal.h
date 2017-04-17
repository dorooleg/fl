#pragma once
#include "token.h"
class terminal :
	public token
{
public:
	terminal(const std::string& value)
		: value_(value)
	{

	}

	bool is_terminal() const override
	{
		return true;
	}

	const std::string& get_value() const override
	{
		return value_;
	}

	~terminal()
	{

	}
private:
	std::string value_;
};

