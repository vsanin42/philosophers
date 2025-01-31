/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:27 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/31 23:14:43 by vsanin           ###   ########.fr       */
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
# include <sys/wait.h>
# include <pthread.h>
# include <limits.h>
# include <stdbool.h>
# include <semaphore.h>
# include <fcntl.h>

# define RED		"\033[31m"
# define GREEN		"\033[32m"
# define MAG		"\033[35m"
# define RESET		"\033[0m"

typedef struct s_params
{
	int				philos_count;
	long			tt_die;
	long			tt_eat;
	long			tt_sleep;
	int				must_eat_count;
	long			start_time;
	bool			dinner_over;
	pid_t			*pids;
	sem_t			*sem_forks;
	sem_t			*sem_printf;
	sem_t			*sem_start;
	sem_t			*sem_global;
	sem_t			*sem_shutdown;
	sem_t			*sem_full;
}		t_params;

typedef struct s_philo
{
	int				id;
	pthread_t		th_monitor_self;
	pthread_t		th_shutdown;
	t_params		*params;
	bool			full;
	int				times_eaten;
	long			last_meal;
	sem_t			*sem_philo;
}					t_philo;

typedef enum e_state
{
	EAT,
	SLEEP,
	THINK,
	FORK,
	DIED,
}	t_state;

/* utils.c */
void	error_msg(char *msg);
int		ft_isdigit(int c);
int		ft_strlen(char const *str);
int		ft_atoi(const char *str);
int		safe_printf(t_philo *philo, t_state state);

/* inits.c */
int		append_id(char *buffer, int base_len, int id);
int		generate_sem_name(char *buffer, int id);
int		init_semaphores(t_params *params);
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

/* routine.c */
int		process_single(t_philo *philo);
int		process_eat(t_philo *philo);
void	process_routine(t_philo *philo, t_philo *philo_start);
void	process_offset(t_philo *philo);
void	process_think(t_philo *philo, bool print_flag);

/* cleaning.c */
int		unlink_param_sems(void);
int		close_param_sems(t_params *params);
int		clean_philo_sems(t_philo *philos);
int		clean_semaphores(t_philo *philos, t_params *params, int status);
void	unlink_sems_at_launch(void);

/* process.c */
void	*shutdown(void *arg);
void	*monitor_self(void *arg);
void	process_terminate(t_philo *philo, t_philo *philo_start);
void	post_on_shutdown(t_philo *philo);
int		create_philo_threads(t_philo *philo);

/* main.c */
bool	is_philo_full(t_philo *philo);
int		wait_for_full(t_params *params);
int		wait_for_children(t_params *params);
int		start_dinner(t_philo *philos, t_params *params);

#endif