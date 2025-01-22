/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:18:48 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/22 21:56:13 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	routine_conditions(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->gen_lock);
	if (*philo->dead == true)
	{
		pthread_mutex_unlock(&philo->params->gen_lock);
		return (ERROR);
	}
	pthread_mutex_unlock(&philo->params->gen_lock);
	if (philo->times_eaten == philo->must_eat_count) // if monitor thread accesses this, must make it thread safe
	{
		return (ERROR); // technically not an error
	}
	return (0);	
}

void	safe_printf(t_philo *philo, t_state state)
{
	long	stamp;

	stamp = get_timestamp(philo->params->start_time); // no data race since no rewriting start_time
	pthread_mutex_lock(&philo->params->printf_lock);
	if (state == EAT) // add a wrapped check function for dinner done check. try just one above instead at each if
		printf("%ld\t"GREEN"%d "RESET"is eating\n", stamp, philo->id);
	else if (state == SLEEP)
		printf("%ld\t"GREEN"%d "RESET"is sleeping\n", stamp, philo->id);
	else if (state == THINK)
		printf("%ld\t"GREEN"%d "RESET"is thinking\n", stamp, philo->id);
	else if (state == DIED)
		printf("%ld\t"RED"%d died\n"RESET, stamp, philo->id);
	else if (state == LEFT_FORK || state == RIGHT_FORK)
		printf("%ld \t%d has taken a fork\n", stamp, philo->id);
	pthread_mutex_unlock(&philo->params->printf_lock);
}

void	sync_threads(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->gen_lock);
	while (philo->params->all_ready != true)
	{
		pthread_mutex_unlock(&philo->params->gen_lock); // possibly remove unlock/lock here
		susleep(50); // possible bottleneck?
		pthread_mutex_lock(&philo->params->gen_lock);
	}
	pthread_mutex_unlock(&philo->params->gen_lock);
	// alternatively:
	// while (1)
    // {
    //     pthread_mutex_lock(&philos->params->gen_lock);
    //     if (philos->params->all_ready == true)
    //     {
    //         pthread_mutex_unlock(&philos->params->gen_lock);
    //         break;
    //     }
    //     pthread_mutex_unlock(&philos->params->gen_lock);
    //     susleep(100);
    // }
}

void	*routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	sync_threads(philo);
	int i = 0;
	while (i < 100)
	{
		// if (routine_conditions(philo) == ERROR)
		// 	break ;
		safe_printf(philo, SLEEP);
		//susleep(philo->tt_sleep);
		i++;
	}
	return (NULL);
}
