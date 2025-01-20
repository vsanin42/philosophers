/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/20 19:43:58 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (*philo->dead)
		{
			// printf("%d %d died\n", get_elapsed_time(), philo->id); PRINT IN A DIFFERENT SPOT
			break ;
		}
		if (philo->times_eaten == philo->params->must_eat_count) // 5th param exists and equals how many times a philo ate
		{
			break ; // exit loop if philo ate required number of times
		}
		printf("%d %d is thinking\n", get_elapsed_time(), philo->id);
	}
	return (NULL);
}

void	destroy_forks(pthread_mutex_t *forks, int fork_count, int stop_index)
{
	int	i;

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
	destroy_forks(forks, params.philos_count, -1);
	free(philos);
	free(forks);
	return (0);
}
