/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:56:46 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/02 13:18:00 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// gets current time from timeval struct in microseconds.
long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000 + time.tv_usec);
}

// gets the current timestamp: elapsed time from start time and current time.
// calculated in microseconds and converted to milliseconds for the timestamp.
long	get_timestamp(long start)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return ((((current.tv_sec * 1000000) + current.tv_usec) - start) / 1000);
}

// modified usleep function to add precision to sleep durations.
// while elapsed time is less than needed sleep time, usleep for tiny intervals.
// if dinner_over bool becomes true, break to avoid oversleeping.
int	susleep(long usec, t_params *params)
{
	long			start;

	start = get_current_time();
	while ((get_current_time() - start) < usec)
	{
		if (is_dinner_over(params) == true)
			break ;
		usleep(50);
	}
	return (0);
}

// used to synchronize monitor and the monitored threads.
// monitor thread is stuck in a loop until all threads have started
// and incremented the threads_running variable philos_count times.
bool	sync_monitor(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->gen_lock);
	if (philo->params->threads_running == philo->params->philos_count)
	{
		pthread_mutex_unlock(&philo->params->gen_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->params->gen_lock);
	return (false);
}

// all threads are stuck in the following loop until the main thread
// sets the all_ready variable to true.
// it's done after all threads have been created.
void	sync_threads(t_philo *philo)
{
	pthread_mutex_lock(&philo->params->gen_lock);
	while (philo->params->all_ready != true)
	{
		pthread_mutex_unlock(&philo->params->gen_lock);
		susleep(50, philo->params);
		pthread_mutex_lock(&philo->params->gen_lock);
	}
	pthread_mutex_unlock(&philo->params->gen_lock);
}
