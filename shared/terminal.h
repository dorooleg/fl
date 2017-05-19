#pragma once
#include "token.h"

class terminal :
	public token
{
public:
	terminal(const std::string& value);

	bool is_terminal() const override;

	const std::string& get_value() const override;

	~terminal();

private:
	std::string value_;
};

