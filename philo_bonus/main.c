/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/02 15:21:13 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// simple check of the full bool.
bool	is_philo_full(t_philo *philo)
{
	sem_wait(philo->sem_philo);
	if (philo->full == true)
	{
		sem_post(philo->sem_philo);
		return (true);
	}
	sem_post(philo->sem_philo);
	return (false);
}

// waits for children to terminate.
int	wait_for_children(t_params *params)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	while (i < params->philos_count)
	{
		if (waitpid(params->pids[i], &status, 0) == -1)
			return (error_msg("Error: failed in waitpid."), ERROR);
		i++;
	}
	return (0);
}

// start of the dinner simulation.
// special case for one philosopher.
// set start time before forking to make sure it's the same everywhere.
// fork processes, let child processes start their routine. 
int	start_dinner(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	if (params->philos_count == 1)
		return (process_single(philos));
	params->start_time = get_current_time();
	while (i < params->philos_count)
	{
		params->pids[i] = fork();
		if (params->pids[i] == -1)
			return (error_msg("Error: forking failed."), ERROR);
		if (params->pids[i] == 0)
			process_routine(&philos[i], philos);
		i++;
	}
	return (0);
}

// waits` for all philos to eat enough times.
// once everyone has eaten, signal termination to everyone.
// dying philosophers also post to sem_full to make sure this function exits.
int	wait_for_full(t_params *params)
{
	int	i;

	i = 0;
	if (params->must_eat_count < 0)
		return (ERROR);
	while (i++ < params->philos_count)
		sem_wait(params->sem_full);
	while (i-- >= 0)
		sem_post(params->sem_shutdown);
	return (0);
}

// main. philos and pids arrays are on stack, therefore no malloc was used
// aside from calls in sem_open().
// 1. unlink semaphores at launch to prevent errors.
// 2. check arguments, initialize parameters and philosopher structs.
// 3. perform the dinner simulation.
// 4. if must_eat_count was specified, wait for all philos to get full
// 4.1. if specified, but someone died, they also post to full semaphore.
// 5. if not specified, wait for termination of philos in case of death.
// 6. after all processes exit, clean semaphores and return.
int	main(int argc, char **argv)
{
	t_params	params;
	t_philo		philos[MAX_PHILOS];
	pid_t		pids[MAX_PHILOS];

	unlink_sems_at_launch();
	if (check_args(argc, argv) == ERROR || init_params(&params, argv, pids))
		return (ERROR);
	init_philos(philos, &params);
	if (start_dinner(philos, &params) == ERROR)
		return (clean_semaphores(philos, &params, 1));
	wait_for_full(&params);
	wait_for_children(&params);
	if (clean_semaphores(philos, &params, 0) == ERROR)
		return (ERROR);
	return (0);
}
