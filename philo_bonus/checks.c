/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:53:38 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/02 13:43:20 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// checking argv for parameters not convertible by ft_atoi to a real number.
// negative values are also not accepted.
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

// checking argv for correct arg count and via the helper function.
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

// secondary input checks not done at the check_args stage.
// errors:
// 1. input exceeded INT_MAX (ft_atoi returns -1 to indicate this case).
// 2. times are < 60 ms.
// 3. philos_count is > 200.
// 4. 0 values that don't make sense.
int	secondary_init_checks(t_params *params)
{
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

// simple checker for dinner_over bool status.
bool	is_dinner_over(t_params *params)
{
	sem_wait(params->sem_global);
	if (params->dinner_over == true)
	{
		sem_post(params->sem_global);
		return (true);
	}
	sem_post(params->sem_global);
	return (false);
}

// key check if a philosopher is dead.
// a full philosopher can't be dead.
// a philos is dead if tt_die has elapsed since dinner start or last meal.
bool	is_philo_dead(t_philo *philo)
{
	long	current_time;
	long	ttd;

	ttd = philo->params->tt_die;
	if (is_philo_full(philo) == true)
		return (false);
	current_time = get_current_time();
	sem_wait(philo->sem_philo);
	if (((philo->last_meal > 0)
			&& (current_time - philo->last_meal > ttd))
		|| ((philo->last_meal == 0)
			&& (current_time - philo->params->start_time > ttd)))
	{
		sem_post(philo->sem_philo);
		return (true);
	}
	sem_post(philo->sem_philo);
	return (false);
}
