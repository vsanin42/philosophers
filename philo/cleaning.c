/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 15:17:43 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 22:27:20 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// main clean function that destroys all mutexes and frees allocated memory.
void	big_clean(t_philo *philos, pthread_mutex_t *forks, t_params *params)
{
	destroy_forks(forks, params, -1);
	destroy_philo_locks(philos, -1);
	free(philos);
	free(forks);
}

// destroy mutexes representing all forks, along with shared mutexes
// like printf and general mutexes.
// stop index is set to a number to a rollback destruction
// if an error occurred at init. it's -1 in case of a regular call.
void	destroy_forks(pthread_mutex_t *forks, t_params *params, int stop_index)
{
	int	i;
	int	fork_count;

	fork_count = params->philos_count;
	i = 0;
	if (stop_index >= 0)
		error_msg("Error: mutex initialization failed.");
	if (stop_index == -1)
		stop_index = fork_count;
	while (i < stop_index)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
	pthread_mutex_destroy(&params->printf_lock);
	pthread_mutex_destroy(&params->gen_lock);
}

// destroy all "personal" mutexes belonging to threads.
// stop index is set to a number to a rollback destruction
// if an error occurred at init. it's -1 in case of a regular call.
void	destroy_philo_locks(t_philo *philo, int stop_index)
{
	int	i;
	int	philo_count;

	philo_count = philo->params->philos_count;
	i = 0;
	if (stop_index >= 0)
		error_msg("Error: mutex initialization failed.");
	if (stop_index == -1)
		stop_index = philo_count;
	while (i < stop_index)
	{
		pthread_mutex_destroy(&philo[i].philo_lock);
		i++;
	}
}

// join all threads by traversing the philos array.
// once all the threads have joined, the dinner_over bool can be changed back.
// that allows the monitor thread to return and be joined.
// it's done only if there are more than 1 threads.
// in case of 1 thread a monitor isn't created.
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
	pthread_mutex_lock(&philos->params->gen_lock);
	philos->params->dinner_over = true;
	pthread_mutex_unlock(&philos->params->gen_lock);
	if (i > 1)
	{
		if (pthread_join(philos->params->monitor, NULL) != 0)
			return (error_msg("Error: joining monitor thread failed."), ERROR);
	}
	return (0);
}

// allocate both philos and forks.
int	alloc_p_f(t_philo **philos, pthread_mutex_t **forks, t_params *params)
{
	*philos = malloc(sizeof(t_philo) * params->philos_count);
	if (!*philos)
	{
		error_msg("Error: malloc failed for philos.");
		return (ERROR);
	}
	*forks = malloc(sizeof(pthread_mutex_t) * params->philos_count);
	if (!*forks)
	{
		free(*philos);
		error_msg("Error: malloc failed for forks.");
		return (ERROR);
	}
	return (SUCCESS);
}
