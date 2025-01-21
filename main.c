/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/21 20:17:17 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	routine_conditions(t_philo *philo)
{
	if (*philo->dead)
	{
		// printf("%d %d died\n", get_timestamp(), philo->id); PRINT IN A DIFFERENT SPOT
		return (ERROR);
	}
	if (philo->times_eaten == philo->params->must_eat_count) // 5th param exists and equals how many times a philo ate
	{
		return (ERROR); // exit loop if philo ate required number of times
	}
	return (0);	
}

void	*routine(void *arg)
{
	t_philo			*philo;
	struct timeval	start;

	philo = (t_philo *)arg;
	gettimeofday(&start, NULL);
	// while (1)
	// {
		// if (routine_conditions(philo) == ERROR)
		// 	break ;
	int i = 0;
	(void)philo;
	while (i < 100)
	{	
		pthread_mutex_lock(&philo->params->printf_lock);
		printf("%ld \t%d is thinking\n", get_timestamp(start), philo->id);
		pthread_mutex_unlock(&philo->params->printf_lock);
		//susleep(1000000);
		susleep(philo->tt_die);
		i++;
	}
	// }
	return (NULL);
}

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
	if (join_threads(philos) == ERROR)
		return (free(philos), free(forks), ERROR);
	destroy_forks(forks, &params, -1);
	free(philos);
	free(forks);
	return (0);
}
