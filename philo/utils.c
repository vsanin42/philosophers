/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 16:33:19 by vsanin            #+#    #+#             */
/*   Updated: 2025/02/02 13:18:31 by vsanin           ###   ########.fr       */
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
// a dedicated printf mutex allows only one print at a time.
// if dinner is over, return. otherwise get the timestamp.
// print the appropriate statement based on state enum.
// check if dinner is over at every step to increase precision.
void	safe_printf(t_philo *philo, t_state state)
{
	long	stamp;

	stamp = get_timestamp(philo->params->start_time);
	if (is_dinner_over(philo->params) == true && state != DIED)
		return ;
	pthread_mutex_lock(&philo->params->printf_lock);
	if (state == EAT && !is_dinner_over(philo->params))
		printf("%ld\t"GREEN"%d is eating"RESET"\n", stamp, philo->id);
	else if (state == SLEEP && !is_dinner_over(philo->params))
		printf("%ld\t"GREEN"%d "RESET"is sleeping\n", stamp, philo->id);
	else if (state == THINK && !is_dinner_over(philo->params))
		printf("%ld\t"GREEN"%d "RESET"is thinking\n", stamp, philo->id);
	else if ((state == LEFT_FORK || state == RIGHT_FORK)
		&& !is_dinner_over(philo->params))
		printf("%ld\t"GREEN"%d "RESET"has taken a fork\n", stamp, philo->id);
	else if (state == DIED)
		printf("%ld\t"RED"%d died"RESET"\n", stamp, philo->id);
	pthread_mutex_unlock(&philo->params->printf_lock);
}
