/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:17:43 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/31 15:49:50 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	close_param_sems(t_params *params)
{
	if (sem_close(params->sem_forks) == -1)
		return (error_msg("Error: failed to close forks."), ERROR);
	if (sem_close(params->sem_printf) == -1)
		return (error_msg("Error: failed to close printf."), ERROR);
	if (sem_close(params->sem_start) == -1)
		return (error_msg("Error: failed to close start."), ERROR);
	if (sem_close(params->sem_global) == -1)
		return (error_msg("Error: failed to close global."), ERROR);
	if (sem_close(params->sem_shutdown) == -1)
		return (error_msg("Error: failed to close shutdown."), ERROR);
	if (sem_close(params->sem_term) == -1)
		return (error_msg("Error: failed to close term."), ERROR);
	return (0);
}

int	unlink_param_sems(void)
{
	if (sem_unlink("/forks") == -1)
		return (error_msg("Error: failed to unlink forks"), ERROR);
	if (sem_unlink("/printf") == -1)
		return (error_msg("Error: failed to unlink printf."), ERROR);
	if (sem_unlink("/start") == -1)
		return (error_msg("Error: failed to unlink start."), ERROR);
	if (sem_unlink("/global") == -1)
		return (error_msg("Error: failed to unlink global."), ERROR);
	if (sem_unlink("/shutdown") == -1)
		return (error_msg("Error: failed to unlink shutdown."), ERROR);
	if (sem_unlink("/term") == -1)
		return (error_msg("Error: failed to unlink term."), ERROR);
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
			return (error_msg("Error: failed to close philo sem."), ERROR);
		if (sem_unlink(sem_name) == -1)
			return (error_msg("Error: failed to unlink philo sem."), ERROR);
		i++;
	}
	return (0);
}

int	clean_semaphores(t_philo *philos, t_params *params, int status)
{
	if (close_param_sems(params) == ERROR)
		return (ERROR);
	if (unlink_param_sems() == ERROR)
		return (ERROR);
	if (clean_philo_sems(philos) == ERROR)
		return (ERROR);
	if (status == 1)
		return (ERROR);
	return (0);
}

void	unlink_sems_at_launch(void)
{
	sem_unlink("/forks");
	sem_unlink("/printf");
	sem_unlink("/start");
	sem_unlink("/global");
	sem_unlink("/shutdown");
	sem_unlink("/term");
}
