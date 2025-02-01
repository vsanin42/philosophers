/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:54:35 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 22:02:54 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// monitors its own process only.
// if philo is full, return, allowing pthread_join to join it.
// if philo is dead, print death message (along with posting on shutdown)
// and set own dinner_over bool to true.
void	*monitor_self(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (is_dinner_over(philo->params) == false) // while 1 ?
	{
		if (is_philo_full(philo) == true)
			return (NULL);
		if (is_philo_dead(philo) == true)
		{
			safe_printf(philo, DIED);
			sem_wait(philo->params->sem_global); // move to post to shutdown?
			philo->params->dinner_over = true;
			sem_post(philo->params->sem_global);
		}
	}
	return (NULL);
}

// waits until a resource is posted on shutdown semaphore.
// that happens either after death or after everyone is full.
// set dinner_over bool for that process to true to allow termination. 
void	*shutdown(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	sem_wait(philo->params->sem_shutdown);
	sem_wait(philo->params->sem_global);
	philo->params->dinner_over = true;
	sem_post(philo->params->sem_global);
	return (NULL);
}

// terminates the process.
// waits for two created threads to join.
// close all semaphores and exit.
void	process_terminate(t_philo *philo, t_philo *philo_start)
{
	int	i;

	i = -1;
	pthread_join(philo->th_monitor_self, NULL);
	pthread_join(philo->th_shutdown, NULL);
	// unlink before closing? (wtf)
	close_param_sems(philo->params);
	while (++i < philo->params->philos_count)
		sem_close(philo_start[i].sem_philo);
	exit(0);
}

// create the self monitoring and the shutdown threads for each philosopher.
int	create_philo_threads(t_philo *philo)
{
	if (pthread_create(&philo->th_monitor_self, NULL, &monitor_self, philo))
		return (error_msg("Error creating monitor_self thread."), ERROR);
	if (pthread_create(&philo->th_shutdown, NULL, &shutdown, philo) != 0)
		return (error_msg("Error creating shutdown thread."), ERROR);
	return (0);
}
