/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 16:33:19 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/31 21:57:14 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	error_msg(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}

int	ft_strlen(char const *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	res;
	int	sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == ' ' || str[i] == '\f' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\t' || str[i] == '\v')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res > (INT_MAX - (str[i] - '0')) / 10)
			return (-1);
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}

int	safe_printf(t_philo *philo, t_state state)
{
	long	stamp;

	sem_wait(philo->params->sem_printf);
	if (is_dinner_over(philo->params) == true) // thread safe? keep second cond?
		return (sem_post(philo->params->sem_printf), -1);
	stamp = get_timestamp(philo->params->start_time);
	if (state == EAT && !is_dinner_over(philo->params))
		printf("%ld\t"MAG"%d is eating"RESET"\n", stamp, philo->id);
	else if (state == SLEEP && !is_dinner_over(philo->params))
		printf("%ld\t"MAG"%d "RESET"is sleeping\n", stamp, philo->id);
	else if (state == THINK && !is_dinner_over(philo->params))
		printf("%ld\t"MAG"%d "RESET"is thinking\n", stamp, philo->id);
	else if (state == FORK && !is_dinner_over(philo->params))
		printf("%ld\t"MAG"%d "RESET"has taken a fork\n", stamp, philo->id);
	else if (state == DIED && !is_dinner_over(philo->params))
	{
		printf("%ld\t"RED"%d died"RESET"\n", stamp, philo->id);
		post_on_shutdown(philo);
	}
	sem_post(philo->params->sem_printf);
	return (0);
}
