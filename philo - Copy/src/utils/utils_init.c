/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:55:36 by kong              #+#    #+#             */
/*   Updated: 2026/06/23 17:02:40 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutex_lasteaten(t_app *app)
{
	int	i;

	i = 0;
	while (i < app->n_philo)
	{
		if (pthread_mutex_init(&app->mutex_lasteaten[i], NULL) != 0)
		{
			destroy_mutex_arr(app->mutex_lasteaten, i);
			free(app->mutex_lasteaten);
			app->mutex_lasteaten = NULL;
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	init_mutex_forks(t_app *app)
{
	int	i;

	i = 0;
	while (i < app->n_philo)
	{
		if (pthread_mutex_init(&app->mutex_forks[i], NULL) != 0)
		{
			destroy_mutex_arr(app->mutex_forks, i);
			free(app->mutex_forks);
			app->mutex_forks = NULL;
			return (-1);
		}
		i++;
	}
	return (0);
}

int	init_app_times(t_app *app, char **av)
{
	app->n_philo = ft_atol(av[0]);
	if (app->n_philo <= 0)
		return (printf("philo: Bad input\n"), -1);
	if (av[4])
	{
		app->n_toeat = ft_atol(av[4]);
		if (app->n_toeat == -1 || app->n_toeat == 0)
			return (printf("philo: Bad input\n"), -1);
	}
	else
		app->n_toeat = -1;
	app->t_die = ft_atol(av[1]);
	app->t_eat = ft_atol(av[2]);
	app->t_sleep = ft_atol(av[3]);
	if (app->t_die == -1 || app->t_eat == -1 || app->t_sleep == -1)
		return (printf("philo: Bad input\n"), -1);
	return (0);
}

int	init_app_arr(t_app *app)
{
	app->mutex_lasteaten = malloc(app->n_philo * sizeof(pthread_mutex_t));
	if (!app->mutex_lasteaten)
		return (printf("philo: Error\n"), -1);
	if (init_mutex_lasteaten(app) == -1)
		return (printf("philo: Error\n"), -1);
	app->mutex_forks = malloc(app->n_philo * sizeof(pthread_mutex_t));
	if (!app->mutex_forks)
		return (printf("philo: Error\n"), -1);
	if (init_mutex_forks(app) == -1)
		return (printf("philo: Error\n"), -1);
	app->arr_philos = malloc(app->n_philo * sizeof(t_philo));
	if (!app->arr_philos)
		return (printf("philo: Error\n"), -1);
	return (0);
}
