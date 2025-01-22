/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/22 21:54:42 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

int	start_dinner(t_philo *philos, t_params *params)
{
	int	i;

	i = 0;
	if (params->must_eat_count == 0) // needed at all? maybe it will simply not run
		return (ERROR);
	//else if (params->philos_count == 1)
		// todo
	while (i < params->philos_count)
	{
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
			return (ERROR);
		i++;
	}
	params->start_time = get_current_time();
	pthread_mutex_lock(&params->gen_lock);
	params->all_ready = true;
	pthread_mutex_unlock(&params->gen_lock);
	if (join_threads(philos) == ERROR)
		return (ERROR);
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
		return (free(philos), free(forks), ERROR);
	destroy_forks(forks, &params, -1);
	free(philos);
	free(forks);
	return (0);
}
