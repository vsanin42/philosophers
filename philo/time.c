/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:56:46 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 22:32:05 by vsanin           ###   ########.fr       */
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
		pthread_mutex_unlock(&philo->params->gen_lock); // possibly remove unlock/lock here
		susleep(50, philo->params); // possible bottleneck?
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
