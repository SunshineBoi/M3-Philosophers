/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:48:51 by kong              #+#    #+#             */
/*   Updated: 2026/06/25 19:12:19 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*single_philo_routine(t_philo *philo)
{
	t_app	*app;

	app = philo->app;
	// print thinking
	printf_action(philo, get_timestamp(app), ACT_THINK);
	// grab one fork
	pthread_mutex_lock(&app->mutex_forks[philo->right_fork]);
	printf_action(philo, get_timestamp(app), ACT_FORK);
	// there is no other fork
	while (get_stopflag(app) != 1)
		usleep(500);
	pthread_mutex_unlock(&app->mutex_forks[philo->right_fork]);
	return (NULL);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;
	t_app	*app;

	philo = (t_philo *)data;
	app = philo->app;
	// philo->tid = pthread_self();
	if (app->n_philo == 1)
		return (single_philo_routine(philo));
	if (philo->id % 2 == 1)
		precise_sleep(app, (app->t_eat + app->t_sleep) / 2);
		// precise_sleep(app, app->t_eat);
	while (get_stopflag(app) != 1)
	{
		printf_action(philo, get_timestamp(app), ACT_THINK);
		// if (philo->id % 2 == 1 && app->t_eat > app->t_sleep)
		// 	precise_sleep(app, app->t_eat - app->t_sleep);
		// if (philo->id % 2 == 1 && philo->n_eaten > 0)
		// {
		// 	long surplus = app->t_eat - app->t_sleep;
		// 	if (surplus > 0)
		// 		precise_sleep(app, surplus);
		// }
		// !
		if (philo->n_eaten > 0)
		{
			long surplus = app->t_eat - app->t_sleep;
			if (surplus > 0)
				precise_sleep(app, surplus);
			precise_sleep(app, 1);
		}
		// start eating
		if (eating_routine(app, philo) == -1)
			break ;
		// time to sleep
		if (sleeping_routine(philo) == -1)
			break ;
	}
	return (NULL);
}

static int	helper_watcher(t_app *app, int philo_id, int *finished)
{	
	long	t_passed;

	pthread_mutex_lock(&app->mutex_lasteaten[philo_id]);
	// normal exit
	if (app->arr_philos[philo_id].n_eaten == app->n_toeat)
	{
		pthread_mutex_unlock(&app->mutex_lasteaten[philo_id]);
		(*finished)++;
		return (0);
	}
	// t_passed = get_timestamp(app) - get_lasteatentime(app, i);
	t_passed = get_timestamp(app) - app->arr_philos[philo_id].t_lasteaten;
	pthread_mutex_unlock(&app->mutex_lasteaten[philo_id]);
	if (t_passed > app->t_die)
	{
		set_stopflag(app);
		printf_action(&app->arr_philos[philo_id], get_timestamp(app), ACT_DIE);
		return (-1) ;
	}
	return (0);
}

void	*watcher_routine(void *data)
{
	t_app	*app;
	int		i;
	int		finished;

	app = (t_app *)data;

	while (1)
	{
		i = 0;
		finished = 0;
		// observe death time
		while (i < app->n_philo)
		{
			if (helper_watcher(app, i, &finished) == -1)
				return (NULL);
			i++;
		}
		if (app->n_toeat != -1 && finished == app->n_philo)
			return (NULL);
		usleep(500);
	}
}
