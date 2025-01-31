/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 00:03:12 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	is_philo_full(t_philo *philo)
{
	sem_wait(philo->sem_philo);
	if (philo->full == true)
	{
		sem_post(philo->sem_philo);
		return (true);
	}
	sem_post(philo->sem_philo);
	return (false);
}

int	wait_for_children(t_params *params)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	while (i < params->philos_count)
	{
		if (waitpid(params->pids[i], &status, 0) == -1)
			return (error_msg("Error: failed in waitpid."), ERROR);
		i++;
	}
	return (0);
}

int	start_dinner(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	if (params->philos_count == 1)
		return (process_single(philos));
	params->start_time = get_current_time(); // better be here and inited once
	while (i < params->philos_count)
	{
		params->pids[i] = fork();
		if (params->pids[i] == -1)
			return (error_msg("Error: forking failed."), ERROR);
		if (params->pids[i] == 0)
			process_routine(&philos[i], philos); // if leaks, pass original philos to traverse and close all sems
		i++;
	}
	// while (i-- > 0)
	// 	sem_post(params->sem_start);
	return (0);
}

int	wait_for_full(t_params *params)
{
	int	i;

	i = 0;
	if (params->must_eat_count < 0)
		return (ERROR);
	while (i++ < params->philos_count)
		sem_wait(params->sem_full);
	while (i-- >= 0)
		sem_post(params->sem_shutdown);
	return (0);
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_philo		philos[MAX_PHILOS];
	pid_t		pids[MAX_PHILOS];

	unlink_sems_at_launch();
	if (check_args(argc, argv) == ERROR || init_params(&params, argv, pids))
		return (ERROR);
	init_philos(philos, &params);
	if (start_dinner(philos, &params) == ERROR)
		return (clean_semaphores(philos, &params, 1));
	wait_for_full(&params);
	wait_for_children(&params);
	if (clean_semaphores(philos, &params, 0) == ERROR)
		return (ERROR);
	return (0);
}
