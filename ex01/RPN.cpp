/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 06:48:08 by aaycan            #+#    #+#             */
/*   Updated: 2026/11/04 06:48:08 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <stack>
#include <list>
#include <climits>

static bool	is_operator(char c);
static int	apply(char op, int left, int right);

RPN::RPN() {}

RPN::RPN(const RPN &other)
{
	(void)other;
}

RPN	&RPN::operator=(const RPN &other)
{
	(void)other;
	return (*this);
}

RPN::~RPN() {}

int	RPN::evaluate(const std::string &expression) const
{
	std::stack<int, std::list<int> >	operands;
	std::string::size_type				i;
	int									left;
	int									right;

	i = 0;
	while (i < expression.size())
	{
		if (expression[i] == ' ')
		{
			i++;
			continue ;
		}
		if ((i + 1 < expression.size()) && (expression[i + 1] != ' '))
			throw (Error());
		if ((expression[i] >= '0') && (expression[i] <= '9'))
			operands.push(expression[i] - '0');
		else if (is_operator(expression[i]))
		{
			if (operands.size() < 2)
				throw (Error());
			right = operands.top();
			operands.pop();
			left = operands.top();
			operands.pop();
			operands.push(apply(expression[i], left, right));
		}
		else
			throw (Error());
		i++;
	}
	if (operands.size() != 1)
		throw (Error());
	return (operands.top());
}

const char	*RPN::Error::what(void) const throw()
{
	return ("Error");
}

static bool	is_operator(char c)
{
	return ((c == '+') || (c == '-') || (c == '*') || (c == '/'));
}

static int	apply(char op, int left, int right)
{
	double	result;

	if ((op == '/') && (right == 0))
		throw (RPN::Error());
	if (op == '+')
		result = static_cast<double>(left) + right;
	else if (op == '-')
		result = static_cast<double>(left) - right;
	else if (op == '*')
		result = static_cast<double>(left) * right;
	else
		result = static_cast<double>(left) / right;
	if ((result > INT_MAX) || (result < INT_MIN))
		throw (RPN::Error());
	if (op == '/')
		return (left / right);
	return (static_cast<int>(result));
}
