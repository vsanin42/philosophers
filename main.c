/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/16 13:15:05 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_params(t_params *params, char **argv)
{
	params->philos_count = ft_atoi(argv[0]);
	params->tt_die = ft_atoi(argv[1]);
	params->tt_eat = ft_atoi(argv[2]);
	params->tt_sleep = ft_atoi(argv[3]);
	if (argv[4])
		params->eat_count = ft_atoi(argv[4]);
	else
		params->eat_count = -2;
	if (params->philos_count == -1 || params->tt_die == -1
		|| params->tt_eat == -1 || params->tt_sleep == -1)
	{
		error_msg("Error: integer overflow occurred.");
		return (ERROR);
	}
	return (0);
}

int	routine()
{
	
}

int	init_philos(t_philo *philos, pthread_mutex_t *forks, t_params *params)
{
	int	i;

	i = 0;
	while (i < params->philos_count)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (free(forks), ERROR);
		i++;
	}
	i = 0;
	while (i < params->philos_count)
	{
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % params->philos_count];
		philos[i].params = params;
		if (pthread_create(&philos[i].thread, NULL, &routine, NULL) != 0)
			return (ERROR);
		i++;
	}
	return (0);
}

void	destroy_forks(pthread_mutex_t *forks, int fork_count)
{
	int	i;

	i = 0;
	while (i < fork_count)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

int	alloc_p_and_f(t_philo **philos, pthread_mutex_t **forks, t_params *params)
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

int	main(int argc, char **argv)
{
	t_params		params;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	if (check_args(argc, argv) == ERROR || init_params(&params, argv) == ERROR)
		return (ERROR);
	if (alloc_p_and_f(&philos, &forks, &params) == ERROR)
		return (ERROR);
	if (init_philos(philos, forks, &params) == ERROR)
		return (free(philos), free(forks), ERROR);
	destroy_forks(forks, params.philos_count);
	free(philos);
	free(forks);
	return (0);
}
