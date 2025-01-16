/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:24:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/16 17:30:59 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

int	init_params(t_params *params, char **argv)
{
	params->philos_count = ft_atoi(argv[1]);
	params->tt_die = ft_atoi(argv[2]);
	params->tt_eat = ft_atoi(argv[3]);
	params->tt_sleep = ft_atoi(argv[4]);
	if (argv[5])
		params->eat_count = ft_atoi(argv[5]);
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

int	init_p_f(t_philo *philos, pthread_mutex_t *forks, t_params *params)
{
	int	i;

	i = 0;
	while (i < params->philos_count)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (destroy_forks(forks, params->philos_count, i), ERROR);
		i++;
	}
	i = 0;
	while (i < params->philos_count)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = &forks[i];
		if (params->philos_count > 1)
			philos[i].right_fork = &forks[(i + 1) % params->philos_count];
		else
			philos[i].right_fork = NULL;
		philos[i].params = params;
		if (pthread_create(&philos[i].thread, NULL, &routine, &philos[i]) != 0)
			return (ERROR);
		// pthread_detach(philos[i].thread);
		i++;
	}
	return (0);
}
