/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:27 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/29 15:37:42 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define SUCCESS 0
# define ERROR 1
# define MAX_PHILOS 200
# define _GNU_SOURCE

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>
# include <stdbool.h>
# include <semaphore.h>
# include <fcntl.h>

# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define RESET		"\033[0m"

typedef struct s_params
{
	int				philos_count;
	long			tt_die;
	long			tt_eat;
	long			tt_sleep;
	int				must_eat_count;
	int				threads_running;
	long			start_time;
	bool			all_ready;
	bool			dinner_over;
	pid_t			*pids;
	pthread_t		monitor; // join this. init at start_dinner
	sem_t			*sem_forks;
	sem_t			*sem_printf;
}		t_params;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	t_params		*params;
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

typedef struct s_dinner
{
	t_philo		*philos;
	t_params	*params;
	int			*pids;
}				t_dinner;

/* utils.c */
void	error_msg(char *msg);
int		ft_isdigit(int c);
int		ft_strlen(char const *str);
int		ft_atoi(const char *str);
void	safe_printf(t_philo *philo, t_state state);

/* inits.c */
int		init_params(t_params *params, char **argv, pid_t *pids);
int		init_philos(t_philo *philos, t_params *params);

/* checks.c */
int		check_arg(char *arg);
int		check_args(int argc, char **argv);
int		secondary_init_checks(t_params *params);
bool	is_dinner_over(t_params *params);
bool	is_philo_dead(t_philo *philo);

/* time.c */
long	get_current_time(void);
long	get_timestamp(long start);
int		susleep(long usec, t_params *params);
void	sync_threads(t_philo *philo);
bool	sync_monitor(t_philo *philo);

/* routine.c */
void	*philone(void *arg);
void	*routine(void *arg);
void	routine_eat(t_philo *philo);
void	routine_think(t_philo *philo, bool print_flag);
void	routine_offset(t_philo *philo);

/* cleaning.c */
int		clean_semaphores(t_params *params);
int		join_threads(t_philo *philos);

/* main.c */
bool	is_philo_full(t_philo *philo);
void	*monitor(void *arg);
int		start_dinner(t_philo *philos, t_params *params);

#endif