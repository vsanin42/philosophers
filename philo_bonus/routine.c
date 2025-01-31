/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:18:48 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/31 16:39:40 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	process_single(t_philo *philo)
{
	int	status;

	status = 0;
	philo->params->pids[0] = fork();
	if (philo->params->pids[0] == -1)
		return (error_msg("Error: forking failed."), ERROR);
	if (philo->params->pids[0] == 0)
	{
		philo->params->start_time = get_current_time();
		safe_printf(philo, LEFT_FORK);
		susleep(philo->params->tt_die, philo->params);
		safe_printf(philo, DIED);
		close_param_sems(philo->params);
		sem_close(philo->sem_philo);
		exit(0);
	}
	if (waitpid(philo->params->pids[0], &status, 0) == -1)
		return (error_msg("Error: failed in waitpid."), ERROR);
	return (0);
}

void	process_eat(t_philo *philo)
{
	sem_wait(philo->params->sem_forks);
	safe_printf(philo, LEFT_FORK);
	sem_wait(philo->params->sem_forks);
	safe_printf(philo, RIGHT_FORK);
	sem_wait(philo->sem_philo);
	philo->last_meal = get_current_time();
	sem_post(philo->sem_philo);
	philo->times_eaten += 1;
	// some mechanism to prevent from eating if dead?
	safe_printf(philo, EAT);
	susleep(philo->params->tt_eat, philo->params);
	sem_post(philo->params->sem_forks); // bit earlier post on forks
	sem_post(philo->params->sem_forks);
	if (philo->times_eaten == philo->params->must_eat_count)
	{
		sem_wait(philo->sem_philo);
		philo->full = true;
		sem_post(philo->sem_philo);
	}
}

void	process_think(t_philo *philo, bool print_flag)
{
	long	eat;
	long	sleep;
	long	think;

	if (print_flag == true)
		safe_printf(philo, THINK);
	if (philo->params->philos_count % 2 == 0)
		return ;
	eat = philo->params->tt_eat;
	sleep = philo->params->tt_sleep;
	think = eat * 2 - sleep;
	if (think < 0)
		think = 0;
	susleep(think * 0.5, philo->params);
}

void	process_offset(t_philo *philo)
{
	if (philo->params->philos_count % 2 == 0)
	{
		if (philo->id % 2 == 0)
			susleep(30000, philo->params);
	}
	else
	{
		if (philo->id % 2 != 0)
			process_think(philo, false);
	}
}

void	process_routine(t_philo *philo, t_philo *philo_start)
{
	sem_wait(philo->params->sem_start);
	create_philo_threads(philo);
	process_offset(philo);
	while (is_dinner_over(philo->params) == false)
	{
		process_eat(philo);
		if (is_philo_full(philo))
			break ; // if philo
		safe_printf(philo, SLEEP);
		susleep(philo->params->tt_sleep, philo->params);
		process_think(philo, true);
	}
	process_terminate(philo, philo_start);
}
