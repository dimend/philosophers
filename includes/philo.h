/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:55:15 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/31 23:14:32 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <unistd.h>

typedef struct s_table
{
	int				is_dead;
	int				have_eaten;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	is_dead_mutex;
	pthread_mutex_t	have_eaten_mutex;
}					t_table;

typedef struct s_philo
{
	int				n_philo;
	int				t_id;
	int				tt_die;
	int				tt_eat;
	int				tt_sleep;
	int				max_eat;
	int				ate;
	long			start_time;
	long			last_meal;
	pthread_t		thread;
	short int		using_fork;
	pthread_mutex_t	fork;
	struct s_philo	*next;

	t_table			*table;
}					t_philo;

short int			check_params(char **argv);
long				ft_atoi(const char *str);

t_philo				*init_table(char **argv, long start_time, t_table *table);
t_philo				*init_philos(char **argv, int n_philo, long start_time,
						t_table *table);
t_philo				*create_philos_loop(char **argv, long start_time,
						t_table *table, t_philo **head);
t_philo				*create_philo(int id, char **argv, long start_time,
						t_table *table);
void				init_values(t_philo *philo, int n_philo, char **argv,
						long start_time);
short int			start_threading(t_philo *head, int n_philo);
void				routine(void *arg);

short int			is_anyone_dead(t_philo *philo);
short int			eating(t_philo *philo);
short int			sleeping(t_philo *philo);
short int			thinking(t_philo *philo);

long				get_time(t_philo *philo);
void				safe_print(t_philo *philo, const char *message);

short int			is_single_philo(t_philo *philo);
short int			try_take_fork(t_philo *philo);
short int			take_forks(t_philo *philo);
void				unlock_forks(t_philo *philo);
short int			check_and_update_max_eat(t_philo *philo);

void				cleanup(t_philo *head_philo, int n_philo, t_table *table,
						int join_threads_flag);

#endif