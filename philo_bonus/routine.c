/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:18:48 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 22:49:29 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// single philo workaround to avoid creating something for a known outcome.
// 1 philo = 1 fork, philo can't eat and dies. a process must be created anyway.
// start time is used in safe_printf for creating a timestamp.
// print that a fork was taken. sem_waiting makes no difference.
// sleep for tt_die, print death, close semaphores, exit child.
// in the meantime parent waits for the child to terminate.
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
		safe_printf(philo, FORK);
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

// eating routine.
// 1. wait on sem_forks = consume a fork resource. print respective message.
// 2. set current meal time at the start of the meal.
// 3. increment times_eaten count. if it's relevant then it's checked below.
// 4. prevent printing if the philo died at the same time. if not then print.
// 4. sleep for tt_eat.
// 5. after eating post on sem_forks = return the fork resource.
// 6. if eaten enough times, set full bool to true and post on sem_full.
// 6.1. if must_eat_count wasn't specified, it's -2.
// in that case the condition is never true.
int	process_eat(t_philo *philo)
{
	sem_wait(philo->params->sem_forks);
	safe_printf(philo, FORK);
	sem_wait(philo->params->sem_forks);
	safe_printf(philo, FORK);
	sem_wait(philo->sem_philo);
	philo->last_meal = get_current_time();
	sem_post(philo->sem_philo);
	philo->times_eaten += 1;
	if (is_philo_dead(philo) == true)
		return (ERROR);
	safe_printf(philo, EAT);
	susleep(philo->params->tt_eat, philo->params);
	if (philo->times_eaten == philo->params->must_eat_count)
	{
		sem_wait(philo->sem_philo);
		philo->full = true;
		sem_post(philo->sem_philo);
		sem_post(philo->params->sem_full);
	}
	sem_post(philo->params->sem_forks); // bit earlier post on forks
	sem_post(philo->params->sem_forks); // MOVED THIS BACK DOWN. TEST AGAIN.
	return (0);
}

// even philos_count is self-regulating.
// odd philos_count needs to be adjusted to keep the system balanced.
// done by forcing a prolonged thinking condition
// to neutralize "greediness" for forks.
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

// offsets the philo execution based on even/odd id.
// for odd philos_count start by thinking differently to avoid deadlocks.
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

// main routine of each process.
// 1. create monitoring and shutdown threads.
// 2. offset the execution based on even/odd id.
// 3. repeat eat-sleep-think cycle until anyone's starvation or own fullness.
void	process_routine(t_philo *philo, t_philo *philo_start)
{
	//sem_wait(philo->params->sem_start);
	create_philo_threads(philo);
	process_offset(philo);
	while (is_dinner_over(philo->params) == false)
	{
		if (process_eat(philo) == ERROR)
			break ;
		if (is_philo_full(philo))
			break ;
		safe_printf(philo, SLEEP);
		susleep(philo->params->tt_sleep, philo->params);
		process_think(philo, true);
	}
	process_terminate(philo, philo_start);
}
