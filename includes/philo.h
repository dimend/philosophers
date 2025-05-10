/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 17:55:15 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/09 18:21:09 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <string.h>    // memset
# include <stdio.h>     // printf
# include <stdlib.h>    // malloc, free
# include <unistd.h>    // write, usleep
# include <sys/time.h>  // gettimeofday
# include <pthread.h>   // pthread_create, pthread_detach, pthread_join
                        // pthread_mutex_init, pthread_mutex_destroy
                        // pthread_mutex_lock, pthread_mutex_unlock

typedef struct s_philo
{
    int t_id;
    int tt_die;
    int tt_eat;
    int tt_sleep;
    int max_eat;
    int ate;
    long start_time;
    long last_meal;
    pthread_t thread;
    pthread_mutex_t fork;
    struct s_philo *next;

    int *is_dead;
    pthread_mutex_t *is_dead_mutex;
} t_philo;

short int   check_params(char **argv);
int         ft_atoi(const char *str);
t_philo     *init_philos(char **argv, int n_philos, long start_time);
void        error(char *message);
long        get_time(void);
short int   take_fork(t_philo *philo);
short int   eating(t_philo *philo);
short int   sleeping(t_philo *philo);
short int   thinking(t_philo *philo);
short int   is_anyone_dead(t_philo *philo);
void *routine(void *arg);

#endif