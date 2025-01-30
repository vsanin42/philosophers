/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:17:43 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/30 18:14:52 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	clean_param_sems(t_params *params)
{
	if (sem_close(params->sem_forks) == -1)
		return (error_msg("Error: failed to close forks."), ERROR);
	if (sem_unlink("/forks") == -1)
		return (error_msg("Error: failed to unlink forks"), ERROR);
	if (sem_close(params->sem_printf) == -1)
		return (error_msg("Error: failed to close printf."), ERROR);
	if (sem_unlink("/printf") == -1)
		return (error_msg("Error: failed to unlink printf."), ERROR);
	if (sem_close(params->sem_start) == -1)
		return (error_msg("Error: failed to close start."), ERROR);
	if (sem_unlink("/start") == -1)
		return (error_msg("Error: failed to unlink start."), ERROR);
	if (sem_close(params->sem_global) == -1)
		return (error_msg("Error: failed to close global."), ERROR);
	if (sem_unlink("/global") == -1)
		return (error_msg("Error: failed to unlink global."), ERROR);
	return (0);
}

int	clean_philo_sems(t_philo *philos)
{
	int		i;
	char	sem_name[11];
	i = 0;
	while (i < philos->params->philos_count)
	{
		generate_sem_name(sem_name, philos[i].id);
		if (sem_close(philos[i].sem_philo) == -1)
			return (error_msg("Error: failed to close global."), ERROR);
		if (sem_unlink(sem_name) == -1)
			return (error_msg("Error: failed to unlink global."), ERROR);
		i++;
	}
	return (0);
}

int	clean_semaphores(t_philo *philos, t_params *params)
{
	if (clean_param_sems(params) == ERROR)
		return (ERROR);
	if (clean_philo_sems(philos) == ERROR)
		return (ERROR);
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
