/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:17:43 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 18:27:48 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// closing semaphores inside params struct
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
	if (sem_close(params->sem_full) == -1)
		return (error_msg("Error: failed to close full."), ERROR);
	return (0);
}

// unlinking closed semaphores inside params struct.
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
	if (sem_unlink("/full") == -1)
		return (error_msg("Error: failed to unlink full."), ERROR);
	return (0);
}

// generating semaphore name for each philo's id, closing and unlinking it.
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

// cleanup of semaphores.
// 1. closing semaphores in params struct.
// 2. unlinking semaphores in params struct.
// 3. cleaning philos semaphores.
// status in case a specific return value is needed.
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

// unlink semaphores at launch to prevent errors if the previous
// execution ended with a signal and semaphores didn't get unlinked.
// unlinks all possibly created semaphores in philosopher structs
// by generating all possible names.
void	unlink_sems_at_launch(void)
{
	int		i;
	char	sem_name[11];

	i = 1;
	sem_unlink("/forks");
	sem_unlink("/printf");
	sem_unlink("/start");
	sem_unlink("/global");
	sem_unlink("/shutdown");
	sem_unlink("/full");
	while (i <= 200)
	{
		generate_sem_name(sem_name, i);
		sem_unlink(sem_name);
		i++;
	}
}
