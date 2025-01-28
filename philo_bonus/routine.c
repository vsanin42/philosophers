/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:18:48 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/28 15:21:21 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	*routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	sync_threads(philo);
	// set last meal time to current time here? it's kinda handled
	// pthread_mutex_lock(&philo->params->gen_lock);
	philo->params->threads_running += 1;
	// pthread_mutex_unlock(&philo->params->gen_lock);
	routine_offset(philo);
	while (is_dinner_over(philo->params) == false)
	{
		routine_eat(philo);
		if (is_philo_full(philo))
			break ;
		safe_printf(philo, SLEEP);
		susleep(philo->params->tt_sleep, philo->params);
		routine_think(philo, true);
	}
	return (NULL);
}

void	routine_eat(t_philo *philo)
{
	// pthread_mutex_lock(philo->left_fork);
	safe_printf(philo, LEFT_FORK);
	// pthread_mutex_lock(philo->right_fork);
	safe_printf(philo, RIGHT_FORK);
	// pthread_mutex_lock(&philo->philo_lock);
	philo->last_meal = get_current_time();
	// pthread_mutex_unlock(&philo->philo_lock);
	philo->times_eaten += 1;
	safe_printf(philo, EAT);
	susleep(philo->params->tt_eat, philo->params);
	if (philo->times_eaten == philo->params->must_eat_count)
	{
		// pthread_mutex_lock(&philo->philo_lock);
		philo->full = true;
		// pthread_mutex_unlock(&philo->philo_lock);
	}
	// pthread_mutex_unlock(philo->left_fork);
	// pthread_mutex_unlock(philo->right_fork);
}

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
			routine_think(philo, false); // true if i want to start with odd ids thinking or doing nothing
	}
}
