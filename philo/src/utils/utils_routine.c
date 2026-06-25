/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:49:47 by kong              #+#    #+#             */
/*   Updated: 2026/06/25 11:32:16 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	pickup_forks(t_philo *philo, long first_fork, long sec_fork)
{
	t_app	*app;

	app = philo->app;
	pthread_mutex_lock(&app->mutex_forks[first_fork]);
	printf_action(philo, get_timestamp(app), ACT_FORK);
	if (get_stopflag(app) == 1)
	{
		pthread_mutex_unlock(&app->mutex_forks[first_fork]);
		return (-1);
	}
	pthread_mutex_lock(&app->mutex_forks[sec_fork]);
	printf_action(philo, get_timestamp(app), ACT_FORK);
	if (get_stopflag(app) == 1)
	{
		pthread_mutex_unlock(&app->mutex_forks[first_fork]);
		pthread_mutex_unlock(&app->mutex_forks[sec_fork]);
		return (-1);
	}
	return (0);
}

int	eating_routine(t_app *app, t_philo *philo)
{
	long	t_eat;

	// for odd number fork grab left first and vice versa
	if (philo->id % 2 == 0)
	{
		if (pickup_forks(philo, philo->left_fork, philo->right_fork) == -1)
			return (-1);
	}
	else
	{
		if (pickup_forks(philo, philo->right_fork, philo->left_fork) == -1)
			return (-1);
	}
	pthread_mutex_lock(&app->mutex_lasteaten[philo->id]);
	philo->t_lasteaten = get_timestamp(app);
	t_eat = philo->t_lasteaten;
	philo->n_eaten += 1;
	pthread_mutex_unlock(&app->mutex_lasteaten[philo->id]);
	printf_action(philo, t_eat, ACT_EAT);
	precise_sleep(app, app->t_eat);
	// put down forks
	pthread_mutex_unlock(&app->mutex_forks[philo->left_fork]);
	pthread_mutex_unlock(&app->mutex_forks[philo->right_fork]);
	// check has finsihed eating
	if (app->n_toeat != -1 && philo->n_eaten == app->n_toeat)
		return (-1);
	return (0);
}

int	sleeping_routine(t_philo *philo)
{
	t_app	*app;

	app = philo->app;
	// check status before sleeping
	if (get_stopflag(app) == 1)
		return (-1);
	printf_action(philo, get_timestamp(app), ACT_SLEEP);
	precise_sleep(app, app->t_sleep);
	return (0);
}
