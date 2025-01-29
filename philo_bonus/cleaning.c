/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:17:43 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/29 15:37:28 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	clean_semaphores(t_params *params)
{
	if (sem_close(params->sem_forks) == -1)
		return (error_msg("Error: failed to close forks."), ERROR);
	if (sem_unlink("/forks") == -1)
		return (error_msg("Error: failed to unlink forks"), ERROR);
	if (sem_close(params->sem_printf) == -1)
		return (error_msg("Error: failed to close printf."), ERROR);
	if (sem_unlink("/printf") == -1)
		return (error_msg("Error: failed to unlink printf."), ERROR);
	return (0);
}

int	join_threads(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->params->philos_count)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			return (error_msg("Error: joining threads failed."), ERROR);
		i++;
	}
	// pthread_mutex_lock(&philos->params->gen_lock);	// does this change anything?
	// philos->params->dinner_over = true;
	// pthread_mutex_unlock(&philos->params->gen_lock);
	if (i > 1)
	{
		if (pthread_join(philos->params->monitor, NULL) != 0)
			return (error_msg("Error: joining monitor thread failed."), ERROR);
	}
	return (0);
}
