#pragma once
#include <string>

class token
{
public:
    token();
	virtual ~token();
	virtual bool is_terminal() const = 0;
	virtual const std::string& get_value() const = 0;

	friend std::ostream& operator<<(std::ostream& os, const token& token);
};
