/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/24 18:59:08 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	is_philo_dead(t_philo *philo)
{
	long	current_time;

	pthread_mutex_lock(&philo->philo_lock);
	if (philo->full == true)
	{
		pthread_mutex_unlock(&philo->philo_lock);
		return (false);
	}
	pthread_mutex_unlock(&philo->philo_lock);
	current_time = get_current_time();
	pthread_mutex_lock(&philo->philo_lock);
	if (((philo->last_meal > 0) // might need a value getter if this proves to be too long
		&& (current_time - philo->last_meal > philo->params->tt_die))
		|| ((philo->last_meal == 0)
		&& (current_time - philo->params->start_time > philo->params->tt_die)))
	{
		pthread_mutex_unlock(&philo->philo_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->philo_lock);
	return (false);
}

int	start_dinner(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	if (params->must_eat_count == 0) // needed at all? maybe it will simply not run
		return (ERROR);
	else if (params->philos_count == 1)
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
	if (pthread_create(&params->monitor, NULL, &monitor, philos) != 0) // where to join monitor thread?
			return (ERROR);
	params->start_time = get_current_time();
	pthread_mutex_lock(&params->gen_lock);
	params->all_ready = true;
	pthread_mutex_unlock(&params->gen_lock);
	return (0);
}

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
