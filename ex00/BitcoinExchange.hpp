/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/02 19:25:11 by aaycan            #+#    #+#             */
/*   Updated: 2026/11/04 03:58:17 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <string>

class BitcoinExchange
{
	private:
		std::map<std::string, double>	_rates;

		void	processLine(const std::string &line) const;
		double	rateAt(const std::string &date) const;

	public:
		BitcoinExchange();
		explicit BitcoinExchange(const std::string &database);
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange	&operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void	loadDatabase(const std::string &database);
		void	processInput(const std::string &input) const;
};

#endif
