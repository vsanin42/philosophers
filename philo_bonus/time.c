/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:56:46 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/02 14:15:15 by vsanin           ###   ########.fr       */
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

// posts philos_count time on the shutdown semaphore.
// sleeps 10 ms to allow all shutdown threads to wake up.
void	post_on_shutdown(t_philo *philo)
{
	int	i;

	i = 0;
	while (i++ < philo->params->philos_count)
		sem_post(philo->params->sem_shutdown);
	usleep(10000);
}

// posts philos_count time on the full semaphore.
// sleeps 10 ms to allow all posts to reach.
void	post_on_full(t_philo *philo)
{
	int	i;

	i = 0;
	while (i++ < philo->params->philos_count)
		sem_post(philo->params->sem_full);
	usleep(10000);
}
