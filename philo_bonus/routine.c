/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:18:48 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/31 00:10:04 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	process_single(t_philo *philo)
{
	philo->params->pids[0] = fork();
	if (philo->params->pids[0] == -1)
		return (error_msg("Error: forking failed."), ERROR);
	if (philo->params->pids[0] == 0)
	{
		philo->params->start_time = get_current_time();
		safe_printf(philo, LEFT_FORK);
		susleep(philo->params->tt_die, philo->params);
		safe_printf(philo, DIED);
		sem_close(philo->params->sem_printf);
		sem_close(philo->params->sem_forks);
		exit(0);
	}
	return (wait_for_children(philo->params));
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
	safe_printf(philo, EAT);
	susleep(philo->params->tt_eat, philo->params);
	if (philo->times_eaten == philo->params->must_eat_count)
	{
		sem_wait(philo->sem_philo);
		philo->full = true;
		sem_post(philo->sem_philo);
	}
	sem_post(philo->params->sem_forks);
	sem_post(philo->params->sem_forks);
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

void	process_routine(t_philo *philo)
{
	sem_wait(philo->params->sem_start);
	// syncing? threads running? self monitoring thread? routine offset?
	// printf("Philo %d start time: %ld\n", philo->id, philo->params->start_time);
	process_offset(philo);
	while (is_dinner_over(philo->params) == false)
	{
		process_eat(philo);
		if (is_philo_full(philo))
			break ;
		safe_printf(philo, SLEEP);
		susleep(philo->params->tt_sleep, philo->params);
		process_think(philo, true);
	}
	sem_close(philo->params->sem_printf);
	sem_close(philo->params->sem_forks);
	sem_close(philo->params->sem_start);
	sem_close(philo->params->sem_global);
	sem_close(philo->params->sem_shutdown);
	sem_close(philo->sem_philo);
	// if leaks, add close_philo_sems or its variation here + pass orig philos * here
	pthread_join(philo->th_monitor, NULL);
	pthread_join(philo->th_shutdown, NULL);
	exit(0);
}













// void	*routine(void *arg)
// {
// 	t_philo			*philo;

// 	philo = (t_philo *)arg;
// 	sync_threads(philo);
// 	// set last meal time to current time here? it's kinda handled
// 	// pthread_mutex_lock(&philo->params->gen_lock);
// 	philo->params->threads_running += 1;
// 	// pthread_mutex_unlock(&philo->params->gen_lock);
// 	routine_offset(philo);
// 	while (is_dinner_over(philo->params) == false)
// 	{
// 		routine_eat(philo);
// 		if (is_philo_full(philo))
// 			break ;
// 		safe_printf(philo, SLEEP);
// 		susleep(philo->params->tt_sleep, philo->params);
// 		routine_think(philo, true);
// 	}
// 	return (NULL);
// }

