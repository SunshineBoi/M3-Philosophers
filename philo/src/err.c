/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kong <kong@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 16:52:23 by kong              #+#    #+#             */
/*   Updated: 2026/06/23 17:00:40 by kong             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_errcode(t_app *app, t_errcode err)
{
	if (app)
		app->err = err;
}

void	perrcode(t_app *app)
{
	if (app)
	{
		if (app->err == ERR_GEN)
			printf("philo: Error\n");
		else if (app->err == ERR_BADIN)
			printf("philo: Bad input\n");
	}
}
