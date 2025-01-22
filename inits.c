/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:24:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/22 21:48:04 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_param_mutexes(t_params *params)
{
	if (pthread_mutex_init(&params->printf_lock, NULL) != 0)
	{
		error_msg("Error initializing mutexes.");
		return (ERROR);
	}
	if (pthread_mutex_init(&params->gen_lock, NULL) != 0)
	{
		error_msg("Error initializing mutexes.");
		pthread_mutex_destroy(&params->printf_lock);
		return (ERROR);
	}
	return (0);
}

int	init_params(t_params *params, char **argv)
{
	params->philos_count = ft_atoi(argv[1]);
	params->tt_die = ft_atoi(argv[2]) * 1000;
	params->tt_eat = ft_atoi(argv[3]) * 1000;
	params->tt_sleep = ft_atoi(argv[4]) * 1000;
	if (argv[5])
		params->must_eat_count = ft_atoi(argv[5]);
	else
		params->must_eat_count = -2;
	if (params->philos_count == -1 || params->tt_die == -1
		|| params->tt_eat == -1 || params->tt_sleep == -1)
		return (error_msg("Error: integer overflow occurred."), ERROR);
	if (params->tt_die < 6e4 || params->tt_eat < 6e4 || params->tt_sleep < 6e4)
		return (error_msg("Error: time to die/eat/sleep is too short."), ERROR);
	if (params->philos_count > 200)
		return (error_msg("Error: too many philosophers."), ERROR);
	params->dead_status = false;
	params->all_ready = false;
	params->start_time = 0;
	if (init_param_mutexes(params) == ERROR)
		return (ERROR);
	return (0);
}

void	init_philo(int i, pthread_mutex_t *frk, t_params *prm, t_philo *phl)
{
	phl[i].id = i + 1;
	phl[i].left_fork = &frk[i]; // todo fork order. maybe reverse left/right
	if (prm->philos_count > 1)
	{
		phl[i].right_fork = &frk[(i + 1) % prm->philos_count];
		if (prm->philos_count % 2 == 0)
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
	phl[i].philos_count = phl[i].params->philos_count;
	phl[i].tt_die = phl[i].params->tt_die;
	phl[i].tt_eat = phl[i].params->tt_eat;
	phl[i].tt_sleep = phl[i].params->tt_sleep;
	phl[i].must_eat_count = phl[i].params->must_eat_count;
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
		init_philo(i, forks, params, philos);
		i++;
	}
	return (0);
}
