/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 22:56:46 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/20 23:31:05 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_elapsed_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
}

int	susleep(int microsec)
{
	struct timeval	start;
	struct timeval	end;

	if (gettimeofday(&start, NULL) == -1)
		return (error_msg("Error: gettimeofday() failed."), ERROR);
	while (1)
	{
		if (gettimeofday(&end, NULL) == -1)
			return (error_msg("Error: gettimeofday() failed."), ERROR);
		if (end.tv_usec == start.tv_usec + microsec)
			break;
	}
	return (0);
}
