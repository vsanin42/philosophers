/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:18:48 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/24 19:00:30 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	safe_printf(t_philo *philo, t_state state)
{
	long	stamp;

	stamp = get_timestamp(philo->params->start_time);
	if (is_dinner_over(philo->params) == true) // thread safe?
		return ;
	pthread_mutex_lock(&philo->params->printf_lock);
	if (state == EAT && !is_dinner_over(philo->params))
		printf("%ld\t"GREEN"%d "RESET"is eating\n", stamp, philo->id);
	else if (state == SLEEP && !is_dinner_over(philo->params))
		printf("%ld\t"GREEN"%d "RESET"is sleeping\n", stamp, philo->id);
	else if (state == THINK && !is_dinner_over(philo->params))
		printf("%ld\t"GREEN"%d "RESET"is thinking\n", stamp, philo->id);
	else if ((state == LEFT_FORK || state == RIGHT_FORK)
		&& !is_dinner_over(philo->params))
		printf("%ld\t"GREEN"%d "RESET"has taken a fork\n", stamp, philo->id);
	else if (state == DIED && !is_dinner_over(philo->params))
		printf("%ld\t"RED"%d died\n"RESET, stamp, philo->id);
	pthread_mutex_unlock(&philo->params->printf_lock);
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
				pthread_mutex_lock(&philo->params->gen_lock);
				philo->params->dinner_over = true;
				pthread_mutex_unlock(&philo->params->gen_lock);
				safe_printf(philo, DIED);
			}
			i++;
		}
	}
	return (NULL);
}

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
	pthread_mutex_lock(&philo->params->gen_lock);
	philo->params->threads_running += 1;
	pthread_mutex_unlock(&philo->params->gen_lock);
	int i = 0;
	while (is_dinner_over(philo->params) == false)
	{
		pthread_mutex_lock(&philo->philo_lock);
		if (philo->full == true)
		{
			pthread_mutex_unlock(&philo->philo_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->philo_lock);
		routine_eat(philo);
		safe_printf(philo, SLEEP);
		susleep(philo->params->tt_sleep, philo->params);		
		safe_printf(philo, THINK);
		i++;
	}
	return (NULL);
}

void	routine_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	safe_printf(philo, LEFT_FORK);
	pthread_mutex_lock(philo->right_fork);
	safe_printf(philo, RIGHT_FORK);
	pthread_mutex_lock(&philo->philo_lock);
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->philo_lock);
	philo->times_eaten += 1;
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
}
