/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:18:48 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/02 13:13:36 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// single philo workaround to avoid messing with something for a known outcome.
// 1 philo = 1 fork, philo can't eat and dies.
// start time is used in safe_printf for creating a timestamp.
// print that a fork was taken. actual action of locking makes no difference.
// sleep for tt_die, print death, return.
void	*philone(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->params->start_time = get_current_time();
	safe_printf(philo, LEFT_FORK);
	susleep(philo->params->tt_die, philo->params);
	safe_printf(philo, DIED);
	return (NULL);
}

// main thread routine.
// 1. wait for the main thread to flip a bool to sync threads.
// 2. increment threads_running variable to sync monitor.
// 3. offset the execution based on even/odd id.
// 4. repeat eat-sleep-think cycle until anyone's starvation or own fullness.
void	*routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	sync_threads(philo);
	pthread_mutex_lock(&philo->params->gen_lock);
	philo->params->threads_running += 1;
	pthread_mutex_unlock(&philo->params->gen_lock);
	routine_offset(philo);
	while (is_dinner_over(philo->params) == false)
	{
		if (routine_eat(philo) == ERROR)
			break ;
		if (is_philo_full(philo))
			break ;
		safe_printf(philo, SLEEP);
		susleep(philo->params->tt_sleep, philo->params);
		routine_think(philo, true);
	}
	return (NULL);
}

// eating routine.
// 1. attempt to take a fork. if successful, print respective message.
// 1.1. do it again. (left/right doesn't matter, they're grabbed differently).
// 2. set current meal time at the start of the meal.
// 3. increment times_eaten count. if it's relevant then it's checked below.
// 4. prevent printing if the philo died at the same time. if not then print.
// 4. sleep for tt_eat.
// 5. if eaten enough times, set full bool to true.
// 5.1. if must_eat_count wasn't specified, it's -2.
// in that case the condition is never true.
// 6. release the forks.
int	routine_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_printf(philo, LEFT_FORK);
	pthread_mutex_lock(philo->right_fork);
	safe_printf(philo, RIGHT_FORK);
	pthread_mutex_lock(&philo->philo_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->philo_lock);
	philo->times_eaten += 1;
	if (is_philo_dead(philo) == true)
		return (ERROR);
	safe_printf(philo, EAT);
	susleep(philo->params->tt_eat, philo->params);
	if (philo->times_eaten == philo->params->must_eat_count)
	{
		pthread_mutex_lock(&philo->philo_lock);
		philo->full = true;
		pthread_mutex_unlock(&philo->philo_lock);
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

// even philos_count is self-regulating.
// odd philos_count needs to be adjusted to keep the system balanced.
// done by forcing a prolonged thinking condition
// to neutralize "greediness" for forks.
void	routine_think(t_philo *philo, bool print_flag)
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
void	routine_offset(t_philo *philo)
{
	if (philo->params->philos_count % 2 == 0)
	{
		if (philo->id % 2 == 0)
			susleep(30000, philo->params);
	}
	else
	{
		if (philo->id % 2 != 0)
			routine_think(philo, false);
	}
}
