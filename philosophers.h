/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:27 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/20 22:58:29 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define SUCCESS 0
# define ERROR 1
# define _GNU_SOURCE

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>
//# include <stdbool.h>

typedef struct s_params
{
	int	philos_count;
	int	tt_die;
	int	tt_eat;
	int	tt_sleep;
	int	must_eat_count;
	int	dead_status;
}		t_params;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	t_params		*params;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				*dead;
	int				times_eaten;
}					t_philo;

/* utils.c */
void	error_msg(char *msg);
int		ft_isdigit(int c);
int		ft_strlen(char const *str);
int		ft_atoi(const char *str);

/* inits.c */
int		alloc_p_f(t_philo **philos, pthread_mutex_t **forks, t_params *params);
int		init_params(t_params *params, char **argv);
void	init_philo(int i, pthread_mutex_t *frk, t_params *prm, t_philo *phl);
int		init_forks(t_params *params, pthread_mutex_t *forks);
int		init_p_f(t_philo *philos, pthread_mutex_t *forks, t_params *params);

/* checks.c */
int		check_first(char *arg);
int		check_arg(char *arg);
int		check_args(int argc, char **argv);

/* time.c */


/* main.c */
void	*routine(void *arg);
void	destroy_forks(pthread_mutex_t *forks, int fork_count, int stop_index);
int		join_threads(t_philo *philos);

#endif