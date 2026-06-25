/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 16:36:50 by kong              #+#    #+#             */
/*   Updated: 2026/06/23 15:39:12 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>

typedef enum	e_errcode
{
	ERR_OK = 0,
	ERR_GEN = -1, // general error
	ERR_BADIN = -2,  // invalid input
}	t_errcode;

typedef enum	e_action
{
	ACT_FORK,
	ACT_EAT,
	ACT_SLEEP,
	ACT_THINK,
	ACT_DIE,
}	t_action;

typedef struct s_app
{
	long			n_philo;
	long			n_toeat;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	long			t_start_app;
	pthread_mutex_t	*mutex_lasteaten;
	pthread_mutex_t	*mutex_forks;  // need mutex_init / destroy_mutex
	struct s_philo	*arr_philos;
	pthread_mutex_t	mutex_print;
	int				flag_stop;
	pthread_mutex_t	mutex_flag;
	int				err;
}	t_app;

typedef struct s_philo
{
	t_app		*app;
	int			id;
	pthread_t	tid;
	int			n_eaten;
	long		t_lasteaten;
	long		left_fork;
	long		right_fork;
}	t_philo;

/* main.c */
void	destroy_mutex_arr(pthread_mutex_t *arr, int size);
void	free_app(t_app *app);
t_app	*init_app(char **av);

/* err.c */
void	set_errcode(t_app *app, t_errcode err);
void	perrcode(t_app *app);

/* philo.c */
int		join_threads(t_app *app, int threads_size);
t_philo	init_philo(t_app *app, int philo_id);
int		philo(t_app *app);

/* print.c */
void	printf_action(t_philo *philo, long timestamp, t_action action);

/* routine.c */
void	*single_philo_routine(t_philo *philo);
void	*philo_routine(void *data);
void	*watcher_routine(void *data);

/* utils/utils_init.c */
int		init_app_times(t_app *app, char **av);
int		init_app_arr(t_app *app);

/* utils/utils_philo.c */
long	get_lasteatentime(t_app *app, int philo_id);
long	get_timestamp(t_app *app);
int		get_stopflag(t_app *app);
void	set_stopflag(t_app *app);
void	precise_sleep(t_app *app, long duration);

/* utils/utils_routine.c */
int		eating_routine(t_app *app, t_philo *philo);
int		sleeping_routine(t_philo *philo);

/* utils/utils_str.c */
int		lst_size(char **lst);
int		is_digit(char ch);
long	ft_atol(char *str);

#endif