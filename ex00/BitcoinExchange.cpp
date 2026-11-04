/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 19:24:16 by aaycan            #+#    #+#             */
/*   Updated: 2026/11/04 03:59:43 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <iomanip>
#include <stdexcept>

static bool	is_valid_date(const std::string &date);
static bool	is_leap_year(int year);
static bool	is_number(const std::string &str);

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const std::string &database)
{
	loadDatabase(database);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
	: _rates(other._rates) {}

BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_rates = other._rates;
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

void	BitcoinExchange::loadDatabase(const std::string &database)
{
	std::ifstream			file(database.c_str());
	std::string				line;
	std::string				date;
	std::string				rate;
	std::string::size_type	comma;

	if (!file.is_open())
		throw (std::runtime_error("could not open database."));
	if (!std::getline(file, line) || line != "date,exchange_rate")
		throw (std::runtime_error("bad database header."));
	_rates.clear();
	while (std::getline(file, line))
	{
		comma = line.find(',');
		if (comma == std::string::npos)
			throw (std::runtime_error("bad database line => " + line));
		date = line.substr(0, comma);
		rate = line.substr(comma + 1);
		if (!is_valid_date(date) || !is_number(rate) || (rate[0] == '-'))
			throw (std::runtime_error("bad database line => " + line));
		_rates[date] = std::strtod(rate.c_str(), NULL);
	}
	if (_rates.empty())
		throw (std::runtime_error("database is empty."));
}

void	BitcoinExchange::processInput(const std::string &input) const
{
	std::ifstream	file(input.c_str());
	std::string		line;

	if (_rates.empty())
		throw (std::runtime_error("no database loaded."));
	if (!file.is_open())
		throw (std::runtime_error("could not open file."));
	if (std::getline(file, line) && (line != "date | value"))
		processLine(line);
	while (std::getline(file, line))
		processLine(line);
}

void	BitcoinExchange::processLine(const std::string &line) const
{
	std::string::size_type	separator;
	std::string				date;
	std::string				value_str;
	double					value;

	separator = line.find(" | ");
	if (separator == std::string::npos)
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return ;
	}
	date = line.substr(0, separator);
	value_str = line.substr(separator + 3);
	if (!is_valid_date(date) || !is_number(value_str))
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return ;
	}
	value = std::strtod(value_str.c_str(), NULL);
	if (value < 0)
		std::cerr << "Error: not a positive number." << std::endl;
	else if (value > 1000)
		std::cerr << "Error: too large a number." << std::endl;
	else if (date < _rates.begin()->first)
		std::cerr << "Error: no rate available before => " << date
			<< std::endl;
	else
		std::cout << std::setprecision(10) << date << " => " << value << " = "
			<< (value * rateAt(date)) << std::endl;
}

double	BitcoinExchange::rateAt(const std::string &date) const
{
	std::map<std::string, double>::const_iterator	it;

	it = _rates.upper_bound(date);
	--it;
	return (it->second);
}

static bool	is_valid_date(const std::string &date)
{
	static const int		days_in_month[12] = {31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31};
	int						year;
	int						month;
	int						day;
	int						max_day;
	std::string::size_type	i;

	if ((date.size() != 10) || (date[4] != '-') || (date[7] != '-'))
		return (false);
	i = 0;
	while (i < date.size())
	{
		if ((i != 4) && (i != 7) && !std::isdigit(static_cast<unsigned char>(date[i])))
			return (false);
		i++;
	}
	year = std::atoi(date.substr(0, 4).c_str());
	month = std::atoi(date.substr(5, 2).c_str());
	day = std::atoi(date.substr(8, 2).c_str());
	if (month < 1 || month > 12 || day < 1)
		return (false);
	max_day = days_in_month[month - 1];
	if ((month == 2) && (is_leap_year(year)))
		max_day = 29;
	return (day <= max_day);
}

static bool	is_leap_year(int year)
{
	return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

static bool	is_number(const std::string &str)
{
	std::string::size_type	i;
	bool					digit_before;
	bool					digit_after;

	i = 0;
	digit_before = false;
	digit_after = false;
	if ((i < str.size()) && (str[i] == '-' || str[i] == '+'))
		i++;
	while ((i < str.size()) && std::isdigit(static_cast<unsigned char>(str[i])))
	{
		digit_before = true;
		i++;
	}
	if ((i < str.size()) && (str[i] == '.'))
	{
		i++;
		while ((i < str.size())
			&& std::isdigit(static_cast<unsigned char>(str[i])))
		{
			digit_after = true;
			i++;
		}
		if (!digit_after)
			return (false);
	}
	return ((digit_before) && (i == str.size()));
}
