/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:24:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/31 14:33:37 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_semaphores(t_params *params)
{
	int	i;

	i = params->philos_count;
	params->sem_forks = sem_open("/forks", O_CREAT | O_EXCL, 0666, i);
	params->sem_printf = sem_open("/printf", O_CREAT | O_EXCL, 0666, 1);
	params->sem_start = sem_open("/start", O_CREAT | O_EXCL, 0666, 0);
	params->sem_global = sem_open("/global", O_CREAT | O_EXCL, 0666, 1);
	params->sem_shutdown = sem_open("/shutdown", O_CREAT | O_EXCL, 0666, 0);
	params->sem_term = sem_open("/term", O_CREAT | O_EXCL, 0666, 0);
	if (params->sem_forks == SEM_FAILED || params->sem_printf == SEM_FAILED
		|| params->sem_start == SEM_FAILED || params->sem_global == SEM_FAILED
		|| params->sem_shutdown == SEM_FAILED || params->sem_term == SEM_FAILED)
	{
		return (error_msg("Error: failed initializing a semaphore."), ERROR);
	}
	return (0);
}

int	init_params(t_params *params, char **argv, pid_t *pids)
{
	params->philos_count = ft_atoi(argv[1]);
	params->tt_die = ft_atoi(argv[2]) * 1000L;
	params->tt_eat = ft_atoi(argv[3]) * 1000L;
	params->tt_sleep = ft_atoi(argv[4]) * 1000L;
	if (argv[5])
		params->must_eat_count = ft_atoi(argv[5]);
	else
		params->must_eat_count = -2;
	if (secondary_init_checks(params) == ERROR)
		return (ERROR);
	params->start_time = 0;
	params->dinner_over = false;
	params->pids = pids;
	if (init_semaphores(params) == ERROR)
		return (ERROR);
	return (0);
}

int	append_id(char *buffer, int base_len, int id)
{
	char	temp[4];
	int		i;

	i = 0;
	while (id > 0 && i < 3)
	{
		temp[i++] = (id % 10) + '0';
		id /= 10;
	}
	i -= 1;
	while (i >= 0)
	{
		buffer[base_len] = temp[i];
		base_len++;
		i--;
	}
	buffer[base_len] = '\0';
	return (0);
}

int	generate_sem_name(char *buffer, int id)
{	
	char	*base;
	int		base_len;
	int		i;

	i = 0;
	base = "/philo_";
	base_len = ft_strlen(base);
	while (i < base_len)
	{
		buffer[i] = base[i];
		i++;
	}
	buffer[i] = '\0';
	append_id(buffer, i, id);
	return (0); 
}

int	init_philos(t_philo *philos, t_params *params)
{
	int		i;
	char	sem_name[11];

	i = 0;
	while (i < params->philos_count)
	{
		philos[i].id = i + 1;
		philos[i].params = params;
		philos[i].times_eaten = 0;
		philos[i].full = false;
		philos[i].last_meal = 0;
		generate_sem_name(sem_name, philos[i].id);
		philos[i].sem_philo = sem_open(sem_name, O_CREAT, 0666, 1);
		if (philos[i].sem_philo == SEM_FAILED)
			return (error_msg("Error: failed opening philo semaphore."), ERROR);
		i++;
	}
	return (0);
}
