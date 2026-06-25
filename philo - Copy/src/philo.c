/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:45:54 by kong              #+#    #+#             */
/*   Updated: 2026/06/24 11:02:43 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	join_threads(t_app *app, int threads_size)
{
	int	i;

	i = 0;
	while (i < threads_size)
	{
		pthread_join(app->arr_philos[i].tid, NULL);
		i++;
	}
	return (0);
}

t_philo	init_philo(t_app *app, int philo_id)
{
	t_philo	philo;

	philo.app = app;
	philo.id = philo_id;
	philo.n_eaten = 0;
	philo.t_lasteaten = 0;
	philo.left_fork = (philo_id - 1 + app->n_philo) % app->n_philo;
	philo.right_fork = philo_id;

	return (philo);
}

int	philo(t_app *app)
{
	// printing format: [timestamp milliseconds] [TID] [actions]
	// actions : 'has taken a fork'; 'is eating'; 'is sleeping'; 'is thinking'; 'died'
	int			i;
	pthread_t	watcher_tid;
	struct timeval	start;

	i = 0;
	while (i < app->n_philo)
	{
		app->arr_philos[i] = init_philo(app, i);
		i++;
	}
	if (gettimeofday(&start, NULL) == -1)
		return (set_errcode(app, ERR_GEN), -1);
	app->t_start_app = (start.tv_sec * 1000) + (start.tv_usec / 1000);
	if (pthread_create(&watcher_tid, NULL, watcher_routine, app) != 0)
		return (set_errcode(app, ERR_GEN), -1);
	i = 0;
	while (i < app->n_philo)
	{
		// app->arr_philos[i] = init_philo(app, i);
		if (pthread_create(&app->arr_philos[i].tid, NULL,
			philo_routine, &app->arr_philos[i]) != 0)
			return (set_stopflag(app), join_threads(app, i),
				pthread_join(watcher_tid, NULL), set_errcode(app, ERR_GEN), -1);
		i++;
	}	
	return (pthread_join(watcher_tid, NULL), join_threads(app, app->n_philo), 0);
}
