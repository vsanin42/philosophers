/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:27 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/24 19:00:40 by vsanin           ###   ########.fr       */
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
# include <stdbool.h>

# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define RESET		"\033[0m"

typedef struct s_params
{
	int				philos_count;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				must_eat_count;
	int				threads_running;
	long			start_time;
	bool			dead_status;
	bool			all_ready;
	bool			dinner_over;
	pthread_mutex_t	printf_lock;
	pthread_mutex_t	gen_lock;
	pthread_mutex_t	mon_lock;
	pthread_t		monitor; // join this
}		t_params;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	long			start_time; // remove bc it's in params
	t_params		*params;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	philo_lock;
	bool			*dead; // ?
	bool			full;
	int				times_eaten;
	long			last_meal;
}					t_philo;

typedef enum e_state
{
	EAT,
	SLEEP,
	THINK,
	LEFT_FORK,
	RIGHT_FORK,
	DIED,
}	t_state;

/* utils.c */
void	error_msg(char *msg);
int		ft_isdigit(int c);
int		ft_strlen(char const *str);
int		ft_atoi(const char *str);
int		alloc_p_f(t_philo **philos, pthread_mutex_t **forks, t_params *params);

/* inits.c */
int		init_params(t_params *params, char **argv);
int		init_philo(int i, pthread_mutex_t *frk, t_params *prm, t_philo *phl);
int		init_forks(t_params *params, pthread_mutex_t *forks);
int		init_p_f(t_philo *philos, pthread_mutex_t *forks, t_params *params);
int		init_param_mutexes(t_params *params);

/* checks.c */
int		check_first(char *arg);
int		check_arg(char *arg);
int		check_args(int argc, char **argv);
int		secondary_init_checks(t_params *params);
bool	is_dinner_over(t_params *params);

/* time.c */
long	get_current_time(void);
long	get_timestamp(long start);
int		susleep(long usec, t_params *params);
void	sync_threads(t_philo *philo);
bool	sync_monitor(t_philo *philo);

/* routine.c */
void	safe_printf(t_philo *philo, t_state state);
void	*monitor(void *arg);
void	*philone(void *arg);
void	*routine(void *arg);
void	routine_eat(t_philo *philo);

/* cleaning.c */
void	big_clean(t_philo *philos, pthread_mutex_t *forks, t_params *params);
void	destroy_forks(pthread_mutex_t *forks, t_params *params, int stop_index);
void	destroy_philo_locks(t_philo *philo, int stop_index);
int		join_threads(t_philo *philos);

/* main.c */
bool	is_philo_dead(t_philo *philo);
int		start_dinner(t_philo *philos, t_params *params);

#endif