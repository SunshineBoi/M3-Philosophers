/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:47:53 by kong              #+#    #+#             */
/*   Updated: 2026/06/25 22:24:35 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_lasteatentime(t_app *app, int philo_id)
{
	long	time_lasteaten;

	pthread_mutex_lock(&app->mutex_lasteaten[philo_id]);
	time_lasteaten = app->arr_philos[philo_id].t_lasteaten;
	pthread_mutex_unlock(&app->mutex_lasteaten[philo_id]);
	return (time_lasteaten);
}

long	get_timestamp(t_app *app)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - app->t_start_app);
}

int	get_stopflag(t_app *app)
{
	int	flag;

	pthread_mutex_lock(&app->mutex_flag);
	flag = app->flag_stop;
	pthread_mutex_unlock(&app->mutex_flag);
	return (flag);
}

void	set_stopflag(t_app *app)
{
	pthread_mutex_lock(&app->mutex_flag);
	app->flag_stop = 1;
	pthread_mutex_unlock(&app->mutex_flag);
}

void	precise_sleep(t_app *app, long duration)
{
	long	start;

	start = get_timestamp(app);
	while (get_timestamp(app) - start < duration)
	{
		if (get_stopflag(app) == 1)
			return ;
		usleep(100);
	}
}
