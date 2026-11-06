/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/04 06:48:12 by aaycan            #+#    #+#             */
/*   Updated: 2026/11/06 02:34:25 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <string>
# include <exception>

class RPN
{
	public:
		RPN();
		RPN(const RPN &other);
		RPN	&operator=(const RPN &other);
		~RPN();

		int	evaluate(const std::string &expression) const;

		class Error : public std::exception
		{
			public:
				virtual const char	*what(void) const throw();
		};
};

#endif
