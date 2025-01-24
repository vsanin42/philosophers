/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:53:38 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/24 15:30:51 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_first(char *arg)
{
	while (*arg == '0')
		arg++;
	if (*arg == '\0')
		return (ERROR);
	else
		return (SUCCESS);
}

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
	if (check_first(*argv) == ERROR)
	{
		error_msg("Error: there must be at least 1 philosopher.");
		return (ERROR);
	}
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
	if (params->philos_count == -1 || params->tt_die == -1
		|| params->tt_eat == -1 || params->tt_sleep == -1)
		return (error_msg("Error: integer overflow occurred."), ERROR);
	if (params->tt_die < 6e4 || params->tt_eat < 6e4 || params->tt_sleep < 6e4)
		return (error_msg("Error: time to die/eat/sleep is too short."), ERROR);
	if (params->philos_count > 200)
		return (error_msg("Error: too many philosophers."), ERROR);
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
