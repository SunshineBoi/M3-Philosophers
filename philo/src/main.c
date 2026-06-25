/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 16:34:15 by kong              #+#    #+#             */
/*   Updated: 2026/06/24 10:52:00 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutex_arr(pthread_mutex_t *arr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		pthread_mutex_destroy(&arr[i]);
		i++;
	}
}

void	free_app(t_app *app)
{
	if (app)
	{
		if (app->arr_philos)
			free(app->arr_philos);
		pthread_mutex_destroy(&app->mutex_flag);
		pthread_mutex_destroy(&app->mutex_print);
		if (app->mutex_lasteaten)
		{
			destroy_mutex_arr(app->mutex_lasteaten, app->n_philo);
			free(app->mutex_lasteaten);
		}
		if (app->mutex_forks)
		{
			destroy_mutex_arr(app->mutex_forks, app->n_philo);
			free(app->mutex_forks);
		}
		free(app);
	}
}

t_app	*init_app(char **av)
{
	t_app			*app;
	// struct timeval	start;

	app = malloc(sizeof(t_app));
	if (!app)
		return (printf("philo: Error\n"), NULL);
	memset(app, 0, sizeof(t_app));
	if (pthread_mutex_init(&app->mutex_print, NULL) != 0)
		return (printf("philo: Error\n"), free(app), NULL);
	if (pthread_mutex_init(&app->mutex_flag, NULL) != 0)
		return (printf("philo: Error\n"),
			pthread_mutex_destroy(&app->mutex_print), free(app), NULL);
	if (init_app_times(app, av) == -1)
		return (free_app(app), NULL);
	if (init_app_arr(app) == -1)
		return (free_app(app), NULL);
	app->flag_stop = 0;
	app->err = ERR_OK;
	// if (gettimeofday(&start, NULL) == -1)
	// 	return (printf("philo: Error\n"), free_app(app), NULL);
	// app->t_start_app = (start.tv_sec * 1000) + (start.tv_usec / 1000);
	return (app);
}

int main(int ac, char **av)
{
	t_app	*app;

	if (ac != 5 && ac != 6)
		return (EXIT_FAILURE);
	app = init_app(av + 1);
	if (!app)
		return (EXIT_FAILURE);
	if (philo(app) == -1)
	{
		perrcode(app); 
		free_app(app); 
		return (EXIT_FAILURE);
	}
	free_app(app);
	return (EXIT_SUCCESS);
}
