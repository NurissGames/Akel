// This file is a part of Akel
// Authors : @kbz_8
// Created : 06/05/2021
// Updated : 19/05/2021

#include <Modules/ELTM/eltm.h>

namespace Ak
{
	Token::Token(std::variant<eltm_token, std::string> value)
	{
		_value = std::move(value);
	}

	bool Token::isString()
	{
		return std::holds_alternative<std::string>(_value);
	}
	bool Token::isKeyword()
	{	
		return std::holds_alternative<eltm_token>(_value);
	}

	bool Token::isKwActivate()
	{
		return _activateKw;
	}
	void Token::activateKw(bool enable)
	{
		_activateKw = enable;
	}
}

