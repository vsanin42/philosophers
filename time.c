/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:56:46 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/22 21:15:54 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000 + time.tv_usec);
}

// in ms
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

// for now ok, still causes delays despite all.
// maybe sleep for all time but the last ms. idk
// extremely cpu dependent, test on school pc
// + simulation could end during sleep, add a breaking condition
int	susleep(long usec)
{
	long			start;

	start = get_current_time();
	while ((get_current_time() - start) < usec)
	{
		// continue ;
		// is simulation finished?
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
