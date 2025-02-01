/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 23:30:23 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// simple check of the full bool.
bool	is_philo_full(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_lock);
	if (philo->full == true)
	{
		pthread_mutex_unlock(&philo->philo_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->philo_lock);
	return (false);
}

// monitors all running threads.
// starts after all threads have incremented the threads_running variable.
// while the dinner is not over (someone is not full/nobody is dead),
// check death status of all philos in a loop.
// if philo is dead, print death message and set dinner_over bool to true.
void	*monitor(void *arg)
{
	int		i;
	int		philo_num;
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo_num = philo->params->philos_count;
	while (sync_monitor(philo) == false)
		susleep(100, philo->params);
	while (is_dinner_over(philo->params) == false)
	{
		i = 0;
		while (i < philo_num && !is_dinner_over(philo->params))
		{
			if (is_philo_dead(philo + i) == true)
			{
				pthread_mutex_lock(&philo->params->gen_lock); // this order is mega weird, think about it.
				philo->params->dinner_over = true;
				pthread_mutex_unlock(&philo->params->gen_lock);
				safe_printf(philo + i, DIED);
				// break to avoid double died messages?
			}
			i++;
		}
	}
	return (NULL);
}

// start of the dinner simulation.
// special case for one philosopher.
// create the necessary number of threads. create monitor thread.
// set start time. flip the boolean allowing the threads to start.
// return and wait for threads to join.
int	start_dinner(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	if (params->philos_count == 1)
	{
		if (pthread_create(&philos[0].thread, NULL, &philone, &philos[0]) != 0)
			return (ERROR);
		return (0); // join threads here? they're joined right after in the caller
	}
	while (i < params->philos_count)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
			return (ERROR);
		i++;
	}
	if (pthread_create(&params->monitor, NULL, &monitor, philos) != 0)
		return (ERROR);
	params->start_time = get_current_time();
	pthread_mutex_lock(&params->gen_lock);
	params->all_ready = true;
	pthread_mutex_unlock(&params->gen_lock);
	return (0);
}

// main.
// 1. check arguments, initialize parameters.
// 2. allocate forks and philosophers structs, initialize them.
// 3. perform the dinner simulation.
// 4. join threads.
// 5. after all threads are joined, clean everything and return.
int	main(int argc, char **argv)
{
	t_params		params;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	if (check_args(argc, argv) == ERROR || init_params(&params, argv) == ERROR)
		return (ERROR);
	if (alloc_p_f(&philos, &forks, &params) == ERROR)
		return (ERROR);
	if (init_p_f(philos, forks, &params) == ERROR)
		return (free(philos), free(forks), ERROR);
	if (start_dinner(philos, &params) == ERROR)
		return (big_clean(philos, forks, &params), ERROR);
	if (join_threads(philos) == ERROR)
		return (big_clean(philos, forks, &params), ERROR);
	big_clean(philos, forks, &params);
	return (0);
}
