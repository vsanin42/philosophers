/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 17:24:53 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 22:09:29 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// initialize common semaphores used by all child processes.
// forks: main semaphore keeping track of available forks.
// printf: binary semaphore protecting the printing function.
// start: synchronization semaphore to make sure processes start at once.
// currently not active. !!!
// global: general semaphore used for checking stopping conditions.
// shutdown: semaphore for communicating to everyone it's time to shutdown.
// 1. shutdown thread of each process is waiting for resource to be posted.
// 2. as soon as someone dies, philos_count resources are posted to it.
// 3. shutdown thread sets dinner_over bool to true to terminate the process.
// without shutdown the process wouldn't know when to terminate if someone died.
// full: every full philo posts to it. once philos_count philos have posted,
// the checking loop breaks and resources are posted on shutdown to terminate.
// without signalling fullness the processes can't terminate
// because the shutdown thread won't return and can't be joined.
int	init_semaphores(t_params *params)
{
	int	i;

	i = params->philos_count;
	params->sem_forks = sem_open("/forks", O_CREAT | O_EXCL, 0666, i);
	params->sem_printf = sem_open("/printf", O_CREAT | O_EXCL, 0666, 1);
	params->sem_start = sem_open("/start", O_CREAT | O_EXCL, 0666, 0); // test with/without. if worse then delete
	params->sem_global = sem_open("/global", O_CREAT | O_EXCL, 0666, 1);
	params->sem_shutdown = sem_open("/shutdown", O_CREAT | O_EXCL, 0666, 0);
	params->sem_full = sem_open("/full", O_CREAT | O_EXCL, 0666, 0);
	if (params->sem_forks == SEM_FAILED || params->sem_printf == SEM_FAILED
		|| params->sem_start == SEM_FAILED || params->sem_global == SEM_FAILED
		|| params->sem_shutdown == SEM_FAILED || params->sem_full == SEM_FAILED)
	{
		return (error_msg("Error: failed initializing a semaphore."), ERROR);
	}
	return (0);
}

// initialize parameters of the dinner and store them in a shared params struct.
// first 4 parameters are input-based. times are converted to microseconds.
// if the last argument doesn't exist, set must_eat_count to negative value
// to be able to know if fullness should be actively monitored.
// perform some additional checks not done in the checks.c file.
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

// convert philo's id into chars and write them to the end of the buffer
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

// overwrite buffer with base string + append an id to create a unique sem_name
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

// initialize the array of philos.
// each philo gets its own semaphore that acts like a mutex.
// generate_sem_name() creates a semaphore name for each sem_open call
// based on philo's id.
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
		philos[i].sem_philo = sem_open(sem_name, O_CREAT | O_EXCL, 0666, 1);
		if (philos[i].sem_philo == SEM_FAILED)
			return (error_msg("Error: failed opening philo semaphore."), ERROR);
		i++;
	}
	return (0);
}
