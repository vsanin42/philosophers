/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:56:46 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/22 10:26:06 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000000L + time.tv_usec);
}

// in ms
long	get_timestamp(struct timeval start)
{
	struct timeval	current;
	// long			start_val;
	// long			current_val;

	gettimeofday(&current, NULL);
	// start_val = (start.tv_sec * 1000) + (start.tv_usec / 1000);
	// current_val = (current.tv_sec * 1000) + (current.tv_usec / 1000);
	return (((current.tv_sec - start.tv_sec) * 1000)
		+ ((current.tv_usec - start.tv_usec) / 1000));
}

int	susleep(long usec)
{
	long	start;
	long	elapsed;

	start = get_current_time();
	while (1)
	{
		elapsed = get_current_time() - start;
		if (elapsed >= usec)
			break;
		// Sleep in small chunks to reduce CPU usage but maintain precision
		if (usec - elapsed > 1000)
			usleep(1000); // Sleep for up to 500 microseconds
		else
			usleep(100);  // Sleep in very small increments toward the end
	}
	return (0);
}

// int	susleep(long usec)
// {
// 	long			start;

// 	start = get_current_time();
// 	while ((get_current_time() - start) < usec)
// 	{
// 		continue ;
// 		// usleep(1);
// 	}
// 	return (0);
// }

// !!! test usleep times using oceano main on school pc
// possibly more than 1000 usec threshold is needed
// int	susleep(long usec)
// {
// 	long			elapsed_usec;
//	long			diff;
//	long			start;
//	//long			end;
//
//	// if (usec == 0)
//	// 	return (0);
//	//elapsed_usec = 0;
//	start = get_current_time();
// 	while (1)
// 	{
// 		elapsed_usec = get_current_time() - start;
//		diff = usec - elapsed_usec;
//		if (diff > 1000)
//			usleep(diff / 2);
//		if (elapsed_usec >= usec)
//			break;
// 	}
// 	return (0);
// }
