/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:58:09 by kong              #+#    #+#             */
/*   Updated: 2026/06/25 11:25:18 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// each thread should locked in to finish its IF-ELSE operation.
// cannot have one thread still in ELSE block processing it state while
// 	allowing another thread to enter printing its other state.
void	printf_action(t_philo *philo, long timestamp, t_action action)
{
	pthread_mutex_lock(&philo->app->mutex_print);
	if (get_stopflag(philo->app) == 1)
	{
		if (action == ACT_DIE)
			printf("%ld %d died\n", timestamp, philo->id + 1);
		pthread_mutex_unlock(&philo->app->mutex_print);
		return ;
	}
	if (action == ACT_FORK)
		printf("%ld %d has taken a fork\n", timestamp, philo->id + 1);
	else if (action == ACT_EAT)
		printf("%ld %d is eating\n", timestamp, philo->id + 1);
	else if (action == ACT_SLEEP)
		printf("%ld %d is sleeping\n", timestamp, philo->id + 1);
	else if (action == ACT_THINK)
		printf("%ld %d is thinking\n", timestamp, philo->id + 1);
	pthread_mutex_unlock(&philo->app->mutex_print);
}
