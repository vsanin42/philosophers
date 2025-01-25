/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:53:38 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/25 16:17:05 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_arg(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i + 1] == '\0')
			return (error_msg("Error: non-numeric character found."), ERROR);
		i++;
	}
	while (arg[i] != '\0')
	{
		if (!ft_isdigit(arg[i]))
			return (error_msg("Error: non-numeric character found."), ERROR);
		i++;
	}
	if (arg[0] == '-')
		return (error_msg("Error: negative number found."), ERROR);
	return (SUCCESS);
}

int	check_args(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		error_msg("Error: incorrect number of arguments. Expected: 4 to 5.");
		return (ERROR);
	}
	argv++;
	while (*argv)
	{
		if (check_arg(*argv) == ERROR)
			return (ERROR);
		argv++;
	}
	return (SUCCESS);
}

int	secondary_init_checks(t_params *params)
{
	// printf("ttdie: %ld\n", params->tt_die);
	// printf("tteat: %ld\n", params->tt_eat);
	// printf("ttsleep: %ld\n", params->tt_sleep);
	if (params->philos_count == -1 || params->tt_die == -1000
		|| params->tt_eat == -1000 || params->tt_sleep == -1000)
		return (error_msg("Error: integer overflow occurred."), ERROR);
	if (params->tt_die < 6e4 || params->tt_eat < 6e4 || params->tt_sleep < 6e4)
		return (error_msg("Error: time to die/eat/sleep is too short."), ERROR);
	if (params->philos_count > 200)
		return (error_msg("Error: too many philosophers."), ERROR);
	if (params->philos_count == 0)
		return (error_msg("Error: there cannot be 0 philosophers."), ERROR);
	if (params->must_eat_count == 0)
		return (error_msg("Error: philosophers can't eat 0 times."), ERROR);
	return (0);
}

bool	is_dinner_over(t_params *params)
{
	pthread_mutex_lock(&params->gen_lock);
	if (params->dinner_over == true)
	{
		pthread_mutex_unlock(&params->gen_lock);
		return (true);
	}
	pthread_mutex_unlock(&params->gen_lock);
	return (false);
}

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
