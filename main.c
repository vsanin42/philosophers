/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/16 17:31:03 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

pthread_mutex_t	test;

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	//todo
	pthread_mutex_lock(&test);
	printf("Philosopher #%d: Hello!\n", philo->id);
	usleep(1000000);
	pthread_mutex_unlock(&test);
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

	pthread_mutex_init(&test, NULL);
	if (check_args(argc, argv) == ERROR || init_params(&params, argv) == ERROR)
		return (ERROR);
	if (alloc_p_f(&philos, &forks, &params) == ERROR)
		return (ERROR);
	if (init_p_f(philos, forks, &params) == ERROR)
		return (free(philos), free(forks), ERROR);
	//usleep(6000000);
	if (join_threads(philos) == ERROR)
		return (free(philos), free(forks), ERROR);
	pthread_mutex_destroy(&test);
	destroy_forks(forks, params.philos_count, -1);
	free(philos);
	free(forks);
	return (0);
}
