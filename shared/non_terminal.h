#pragma once
#include "token.h"

class non_terminal
	: public token
{
public:

	non_terminal();

	non_terminal(const std::string& value);

	bool is_terminal() const override;
	
	const std::string& get_value() const override;

	~non_terminal();

	friend std::ostream& operator<<(std::ostream& os, const non_terminal& val);

private:
	std::string value_;
};

