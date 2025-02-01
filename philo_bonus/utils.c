/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 16:33:19 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/01 18:09:46 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// error output.
void	error_msg(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
}

// basic is digit checker.
int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	return (0);
}

// basic strlen function.
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

// basic atoi function. if projected number exceeds INT_MAX, return -1.
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

// safe printing mechanism.
// a binary semaphore allows only one print at a time, preventing mixed output.
// if dinner is over, return. otherwise get the timestamp.
// print the appropriate statement based on state enum.
// check if dinner is over at every step to increase precision.
// if someone is dead, print message + post to shutdown immediately.
// if must_eat_count was specified, post to full, it will post to shutdown too.
int	safe_printf(t_philo *philo, t_state state)
{
	long	stamp;

	sem_wait(philo->params->sem_printf);
	if (is_dinner_over(philo->params) == true) // 2nd cond was state != DIED
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
		if (philo->params->must_eat_count > 0)
			post_on_full(philo);
		else
			post_on_shutdown(philo);
	}
	sem_post(philo->params->sem_printf);
	return (0);
}
