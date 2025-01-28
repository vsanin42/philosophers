/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:24:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/28 15:19:25 by vsanin           ###   ########.fr       */
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
	params->dead_status = false;
	params->all_ready = false;
	params->start_time = 0;
	params->threads_running = 0;
	params->dinner_over = false;
	if (init_param_mutexes(params) == ERROR)
		return (ERROR);
	return (0);
}

int	init_philo(int i, t_params *prm, t_philo *phl)
{
	phl[i].id = i + 1;
	phl[i].params = prm;
	phl[i].dead = &prm->dead_status;
	phl[i].times_eaten = 0;
	phl[i].full = false;
	phl[i].last_meal = 0; // maybe bad but better initialize it for monitor checks
	return (0);
}


int	init_p_f(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	while (i < params->philos_count)
	{
		if (init_philo(i, params, philos) == ERROR)
			return (ERROR);
		i++;
	}
	return (0);
}

int	init_dinner(t_dinner *dinner)
{
	
}
