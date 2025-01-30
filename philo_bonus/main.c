/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/30 18:52:13 by vsanin           ###   ########.fr       */
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

void	*monitor(void *arg)
{
	int		i;
	int		philo_num;
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo_num = philo->params->philos_count;
	while (sync_monitor(philo) == false)
		continue ;
	while (is_dinner_over(philo->params) == false)
	{
		i = 0;
		while (i < philo_num && !is_dinner_over(philo->params))
		{
			if (is_philo_dead(philo + i) == true)
			{
				// pthread_mutex_lock(&philo->params->gen_lock);	// ?
				philo->params->dinner_over = true;
				// pthread_mutex_unlock(&philo->params->gen_lock);
				safe_printf(philo + i, DIED);
				// break to avoid double died messages?
			}
			i++;
		}
	}
	return (NULL);
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
	while (i < params->philos_count)
	{
		params->pids[i] = fork();
		if (params->pids[i] == -1)
			return (error_msg("Error: forking failed."), ERROR);
		if (params->pids[i] == 0)
		{
			// thread creation and sync?
			process_routine(&philos[i]);
		}
		i++;
	}
	while (i-- > 0)
		sem_post(params->sem_start);
	return (wait_for_children(params));
}

/*
	int	i;

	i = 0;
	if (params->must_eat_count == 0) // needed at all? maybe it will simply not run
		return (ERROR);
	else if (params->philos_count == 1)
	{
		if (pthread_create(&philos[0].thread, NULL, &philone, &philos[0]) != 0)
			return (ERROR);
		return (0); // join threads here? they're joined right after in the caller
	}
	while (i < params->philos_count)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
			return (ERROR);
		i++;
	}
	if (pthread_create(&params->monitor, NULL, &monitor, philos) != 0)
		return (ERROR);
	params->start_time = get_current_time();
	pthread_mutex_lock(&params->gen_lock);
	params->all_ready = true;
	pthread_mutex_unlock(&params->gen_lock);
	return (0);
*/

int	main(int argc, char **argv)
{
	t_params	params;
	t_philo		philos[MAX_PHILOS];
	pid_t		pids[MAX_PHILOS];
	
	if (check_args(argc, argv) == ERROR || init_params(&params, argv, pids))
		return (ERROR);
	init_philos(philos, &params);
	if (start_dinner(philos, &params) == ERROR)
	{
		clean_semaphores(philos, &params);
		return (ERROR);
	}
	// if (join_threads(philos) == ERROR)	// if threads are used, join in the end
	// 	return (ERROR);
	if (clean_semaphores(philos, &params) == ERROR)
		return (ERROR);
	return (0);
}
