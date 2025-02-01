/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:56:46 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 17:57:44 by vsanin           ###   ########.fr       */
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

// int	susleep(long usec)
// {
// 	long	start;
// 	long	elapsed;

// 	start = get_current_time();
// 	while (1)
// 	{
// 		elapsed = get_current_time() - start;
// 		if (elapsed >= usec)
// 			break;
// 		// Sleep in small chunks to reduce CPU usage but maintain precision
// 		if (usec - elapsed > 1000)
// 			usleep(1000); // Sleep for up to 500 microseconds
// 		else
// 			usleep(100);  // Sleep in very small increments toward the end
// 	}
// 	return (0);
// }

// modified usleep function to add precision to sleep durations.
// while elapsed time is less than needed sleep time, usleep for tiny intervals.
// if dinner_over bool becomes true, break to avoid oversleeping.
int	susleep(long usec, t_params *params)
{
	long			start;

	start = get_current_time();
	while ((get_current_time() - start) < usec)
	{
		// continue ;
		if (is_dinner_over(params) == true)
			break ;
		usleep(50);
	}
	return (0);
}

// !!! test usleep times using oceano main on school pc
// possibly more than 1000 usec threshold is needed
// int	susleep(long usec)
// {
// 	long			elapsed_usec;
//	long			diff;
//	long			start;
//
//	start = get_current_time();
// 	while (get_current_time() - start < usec)
// 	{
//		// is simulation finished?
// 		elapsed_usec = get_current_time() - start;
//		diff = usec - elapsed_usec;
//		if (diff > 1000)
//			usleep(diff / 2);
//		else
//			while (get_current_time() - start < usec)
//				continue ;
// 	}
// 	return (0);
// }

// posts philos_count time on the shutdown semaphore.
// sleeps 10 ms to allow all shutdown threads to wake up.
void	post_on_shutdown(t_philo *philo)
{
	int	i;

	i = 0;
	while (i++ < philo->params->philos_count)
		sem_post(philo->params->sem_shutdown);
	usleep(10000); // usleep !
}

// posts philos_count time on the full semaphore.
// sleeps 10 ms to allow all posts to reach.
void	post_on_full(t_philo *philo)
{
	int	i;

	i = 0;
	while (i++ < philo->params->philos_count)
		sem_post(philo->params->sem_full);
	usleep(10000); // usleep !
}
