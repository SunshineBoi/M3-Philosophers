/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_validate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:54:24 by kong              #+#    #+#             */
/*   Updated: 2026/06/23 16:54:54 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lst_size(char **lst)
{
	int	i;

	i = 0;
	if (!lst)
		return (0);
	while (lst[i])
		i++;
	return (i);
}

int	is_digit(char ch)
{
	return (ch >= '0' && ch <= '9');
}

long	ft_atol(char *str)
{
	long	sum;
	int		digit;
	long	limit;

	if (!str)
		return (-1);
	if (*str == '+')
		str++;
	if (!*str)
		return (-1);
	sum = 0;
	limit = LONG_MAX;
	while (*str)
	{
		if (!is_digit(*str))
			return (-1);
		digit = *str - '0';
		if (sum > limit / 10 || (sum == limit / 10 && digit > limit % 10))
			return (-1);
		sum = (sum * 10) + digit;
		str++;
	}
	return (sum);
}
