/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:24:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/25 16:15:47 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_param_mutexes(t_params *params)
{
	if (pthread_mutex_init(&params->printf_lock, NULL) != 0)
	{
		error_msg("Error initializing printf lock.");
		return (ERROR);
	}
	if (pthread_mutex_init(&params->gen_lock, NULL) != 0)
	{
		error_msg("Error initializing general lock.");
		pthread_mutex_destroy(&params->printf_lock);
		return (ERROR);
	}
	return (0);
}

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

int	init_philo(int i, pthread_mutex_t *frk, t_params *prm, t_philo *phl)
{
	phl[i].id = i + 1;
	phl[i].left_fork = &frk[i];
	if (prm->philos_count > 1)
	{
		phl[i].right_fork = &frk[(i + 1) % prm->philos_count];
		if ((i + 1) % 2 == 0)
		{
			phl[i].left_fork = &frk[(i + 1) % prm->philos_count];
			phl[i].right_fork = &frk[i];
		}
	}
	else
		phl[i].right_fork = NULL;
	phl[i].params = prm;
	phl[i].dead = &prm->dead_status;
	phl[i].times_eaten = 0;
	phl[i].full = false;
	phl[i].last_meal = 0; // maybe bad but better initialize it for monitor checks
	if (pthread_mutex_init(&phl[i].philo_lock, NULL) != 0)
	{
		error_msg("Error initializing mutexes.");
		return (destroy_forks(frk, prm, -1), ERROR);
	}
	return (0);
}

int	init_forks(t_params *params, pthread_mutex_t *forks)
{
	int	i;

	i = 0;
	while (i < params->philos_count)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			error_msg("Error initializing mutexes.");
			return (destroy_forks(forks, params, i), ERROR);
		}
		i++;
	}
	return (0);
}

int	init_p_f(t_philo *philos, pthread_mutex_t *forks, t_params *params)
{
	int	i;

	i = 0;
	if (init_forks(params, forks) == ERROR)
		return (ERROR);
	while (i < params->philos_count)
	{
		if (init_philo(i, forks, params, philos) == ERROR)
			return (destroy_philo_locks(philos, i), ERROR);
		i++;
	}
	return (0);
}
