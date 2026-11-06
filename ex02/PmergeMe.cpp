/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaycan <aaycan@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 21:40:19 by aaycan            #+#    #+#             */
/*   Updated: 2026/11/06 22:10:31 by aaycan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <climits>

struct Node
{
	int			value;
	std::size_t	id;
};

struct Pair
{
	Node	small;
	Node	large;
};

struct NodeLess
{
	bool	operator()(const Node &left, const Node &right) const
	{
		return (left.value < right.value);
	}
};

static bool						parse_positive_int(const char *text, int &number);
static std::vector<std::size_t>	make_insertion_order(std::size_t pending_count);
static void						ford_johnson_vector(std::vector<Node> &values);
static void						ford_johnson_deque(std::deque<Node> &values);
static double					elapsed_microseconds(std::clock_t start, std::clock_t end);
static void						print_sequence(const std::vector<int> &values);
static void						print_sequence(const std::vector<Node> &values);

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	(void)other;
}

PmergeMe	&PmergeMe::operator=(const PmergeMe &other)
{
	(void)other;
	return (*this);
}

PmergeMe::~PmergeMe() {}

int	PmergeMe::run(int argc, char **argv) const
{
	std::vector<int>	input;
	std::vector<Node>	vector_values;
	std::deque<Node>	deque_values;
	int					number;
	int					i;
	std::clock_t		start;
	std::clock_t		end;
	double				vector_time;
	double				deque_time;

	if (argc < 2)
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		if (!parse_positive_int(argv[i], number))
		{
			std::cerr << "Error" << std::endl;
			return (1);
		}
		input.push_back(number);
		i++;
	}
	std::cout << "Before: ";
	print_sequence(input);
	start = std::clock();
	i = 0;
	while (i < static_cast<int>(input.size()))
	{
		Node	node;

		node.value = input[i];
		node.id = static_cast<std::size_t>(i);
		vector_values.push_back(node);
		i++;
	}
	ford_johnson_vector(vector_values);
	end = std::clock();
	vector_time = elapsed_microseconds(start, end);
	start = std::clock();
	i = 0;
	while (i < static_cast<int>(input.size()))
	{
		Node	node;

		node.value = input[i];
		node.id = static_cast<std::size_t>(i);
		deque_values.push_back(node);
		i++;
	}
	ford_johnson_deque(deque_values);
	end = std::clock();
	deque_time = elapsed_microseconds(start, end);
	std::cout << "After:  ";
	print_sequence(vector_values);
	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << input.size()
		<< " elements with std::vector : " << vector_time << " us" << std::endl;
	std::cout << "Time to process a range of " << input.size()
		<< " elements with std::deque : " << deque_time << " us" << std::endl;
	return (0);
}

static bool	parse_positive_int(const char *text, int &number)
{
	int	value;
	int	digit;
	int	i;

	if ((text == NULL) || (text[0] == '\0'))
		return (false);
	value = 0;
	i = 0;
	while (text[i] != '\0')
	{
		if ((text[i] < '0') || (text[i] > '9'))
			return (false);
		digit = text[i] - '0';
		if (value > ((INT_MAX - digit) / 10))
			return (false);
		value = (value * 10) + digit;
		i++;
	}
	if (value == 0)
		return (false);
	number = value;
	return (true);
}

static std::vector<std::size_t>	make_insertion_order(std::size_t pending_count)
{
	std::vector<std::size_t>	order;
	std::size_t					previous;
	std::size_t					boundary;
	std::size_t					end;
	std::size_t					index;
	std::size_t					next;

	if (pending_count < 2)
		return (order);
	previous = 1;
	boundary = 3;
	while (order.size() < (pending_count - 1))
	{
		end = boundary;
		if (end > pending_count)
			end = pending_count;
		index = end;
		while (index > previous)
		{
			order.push_back(index);
			index--;
		}
		next = boundary + (2 * previous);
		previous = boundary;
		boundary = next;
	}
	return (order);
}

static void	ford_johnson_vector(std::vector<Node> &values)
{
	std::vector<Pair>			pairs;
	std::vector<Node>			bigs;
	std::vector<Pair>			sorted_pairs;
	std::vector<Node>			chain;
	std::vector<std::size_t>	order;
	std::size_t					i;
	std::size_t					j;
	std::size_t					pair_index;
	std::size_t					pending_index;
	std::size_t					pending_count;
	bool						has_odd;
	Node						odd;
	Pair						pair;
	std::vector<Node>::iterator	bound;
	std::vector<Node>::iterator	position;

	if (values.size() < 2)
		return ;
	i = 0;
	while ((i + 1) < values.size())
	{
		if (values[i].value <= values[i + 1].value)
		{
			pair.small = values[i];
			pair.large = values[i + 1];
		}
		else
		{
			pair.small = values[i + 1];
			pair.large = values[i];
		}
		pairs.push_back(pair);
		bigs.push_back(pair.large);
		i += 2;
	}
	has_odd = ((values.size() % 2) != 0);
	if (has_odd)
		odd = values[values.size() - 1];
	ford_johnson_vector(bigs);
	i = 0;
	while (i < bigs.size())
	{
		j = 0;
		while (pairs[j].large.id != bigs[i].id)
			j++;
		sorted_pairs.push_back(pairs[j]);
		i++;
	}
	chain.push_back(sorted_pairs[0].small);
	i = 0;
	while (i < bigs.size())
	{
		chain.push_back(bigs[i]);
		i++;
	}
	pending_count = sorted_pairs.size() + static_cast<std::size_t>(has_odd);
	order = make_insertion_order(pending_count);
	i = 0;
	while (i < order.size())
	{
		pending_index = order[i];
		if (pending_index <= sorted_pairs.size())
		{
			pair_index = pending_index - 1;
			bound = chain.begin();
			while (bound->id != sorted_pairs[pair_index].large.id)
				bound++;
			position = std::lower_bound(chain.begin(), bound,
				sorted_pairs[pair_index].small, NodeLess());
			chain.insert(position, sorted_pairs[pair_index].small);
		}
		else
		{
			position = std::lower_bound(chain.begin(), chain.end(), odd, NodeLess());
			chain.insert(position, odd);
		}
		i++;
	}
	values.swap(chain);
}

static void	ford_johnson_deque(std::deque<Node> &values)
{
	std::deque<Pair>			pairs;
	std::deque<Node>			bigs;
	std::deque<Pair>			sorted_pairs;
	std::deque<Node>			chain;
	std::vector<std::size_t>	order;
	std::size_t					i;
	std::size_t					j;
	std::size_t					pair_index;
	std::size_t					pending_index;
	std::size_t					pending_count;
	bool						has_odd;
	Node						odd;
	Pair						pair;
	std::deque<Node>::iterator	bound;
	std::deque<Node>::iterator	position;

	if (values.size() < 2)
		return ;
	i = 0;
	while ((i + 1) < values.size())
	{
		if (values[i].value <= values[i + 1].value)
		{
			pair.small = values[i];
			pair.large = values[i + 1];
		}
		else
		{
			pair.small = values[i + 1];
			pair.large = values[i];
		}
		pairs.push_back(pair);
		bigs.push_back(pair.large);
		i += 2;
	}
	has_odd = ((values.size() % 2) != 0);
	if (has_odd)
		odd = values[values.size() - 1];
	ford_johnson_deque(bigs);
	i = 0;
	while (i < bigs.size())
	{
		j = 0;
		while (pairs[j].large.id != bigs[i].id)
			j++;
		sorted_pairs.push_back(pairs[j]);
		i++;
	}
	chain.push_back(sorted_pairs[0].small);
	i = 0;
	while (i < bigs.size())
	{
		chain.push_back(bigs[i]);
		i++;
	}
	pending_count = sorted_pairs.size() + static_cast<std::size_t>(has_odd);
	order = make_insertion_order(pending_count);
	i = 0;
	while (i < order.size())
	{
		pending_index = order[i];
		if (pending_index <= sorted_pairs.size())
		{
			pair_index = pending_index - 1;
			bound = chain.begin();
			while (bound->id != sorted_pairs[pair_index].large.id)
				bound++;
			position = std::lower_bound(chain.begin(), bound,
				sorted_pairs[pair_index].small, NodeLess());
			chain.insert(position, sorted_pairs[pair_index].small);
		}
		else
		{
			position = std::lower_bound(chain.begin(), chain.end(), odd, NodeLess());
			chain.insert(position, odd);
		}
		i++;
	}
	values.swap(chain);
}

static double	elapsed_microseconds(std::clock_t start, std::clock_t end)
{
	return (static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC);
}

static void	print_sequence(const std::vector<int> &values)
{
	std::size_t	i;

	i = 0;
	while (i < values.size())
	{
		if (i != 0)
			std::cout << " ";
		std::cout << values[i];
		i++;
	}
	std::cout << std::endl;
}

static void	print_sequence(const std::vector<Node> &values)
{
	std::size_t	i;

	i = 0;
	while (i < values.size())
	{
		if (i != 0)
			std::cout << " ";
		std::cout << values[i].value;
		i++;
	}
	std::cout << std::endl;
}
