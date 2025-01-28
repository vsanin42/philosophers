/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:24:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/29 00:37:47 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_params(t_params *params, char **argv)
{
	params->philos_count = ft_atoi(argv[1]);
	params->tt_die = ft_atoi(argv[2]) * 1000L;
	params->tt_eat = ft_atoi(argv[3]) * 1000L;
	params->tt_sleep = ft_atoi(argv[4]) * 1000L;
	if (argv[5])
		params->must_eat_count = ft_atoi(argv[5]);
	else
		params->must_eat_count = -2;
	if (secondary_init_checks(params) == ERROR)
		return (ERROR);
	params->all_ready = false;
	params->start_time = 0;
	params->threads_running = 0;
	params->dinner_over = false;
	return (0);
}


int	init_philos(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	while (i < params->philos_count)
	{
		philos[i].id = i + 1;
		philos[i].params = params;
		philos[i].times_eaten = 0;
		philos[i].full = false;
		philos[i].last_meal = 0; // maybe bad but better initialize it for monitor checks
		i++;
	}
	return (0);
}

int	init_dinner(t_dinner *dinner, char **argv) // this is bad. TODO
{
	t_params	*params;
	t_philo		philos[MAX_PHILOS];
	int			pids[MAX_PHILOS];

	if (init_params(params, argv) == ERROR)
		return (ERROR);
	if (init_philos(philos, params) == ERROR)
		return (ERROR);
	dinner->params = params;
	dinner->philos = philos;
	dinner->pids = pids;
	return (0);
}
