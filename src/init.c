/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:24:52 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/09 18:18:14 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)
    {
        pthread_mutex_lock(philo->is_dead_mutex);
        if (*(philo->is_dead))  // If the philosopher is dead, stop the loop
        {
            pthread_mutex_unlock(philo->is_dead_mutex);
            break;
        }
        pthread_mutex_unlock(philo->is_dead_mutex);

        take_fork(philo);  // Philosopher tries to take forks

        pthread_mutex_lock(philo->is_dead_mutex);
        if (*(philo->is_dead))  // Check if dead after taking forks
        {
            pthread_mutex_unlock(philo->is_dead_mutex);
            pthread_mutex_unlock(&philo->fork);
            pthread_mutex_unlock(&philo->next->fork);
            break;
        }
        pthread_mutex_unlock(philo->is_dead_mutex);

        eating(philo);

        pthread_mutex_unlock(&philo->fork);
        pthread_mutex_unlock(&philo->next->fork);

        pthread_mutex_lock(philo->is_dead_mutex);
        if (*(philo->is_dead))  // Check if dead after eating
        {
            pthread_mutex_unlock(philo->is_dead_mutex);
            break;
        }
        pthread_mutex_unlock(philo->is_dead_mutex);

        sleeping(philo);

        pthread_mutex_lock(philo->is_dead_mutex);
        if (*(philo->is_dead))  // Check if dead after sleeping
        {
            pthread_mutex_unlock(philo->is_dead_mutex);
            break;
        }
        pthread_mutex_unlock(philo->is_dead_mutex);

        thinking(philo);
    }

    return NULL;
}



void init_values(t_philo *philo,  int n_philo, char **argv, long start_time)
{
    philo->t_id = n_philo + 1;
    philo->tt_die = ft_atoi(argv[2]);
    philo->tt_eat = ft_atoi(argv[3]);
    philo->tt_sleep = ft_atoi(argv[4]);
    philo->start_time = start_time;
    philo->last_meal = start_time;
    philo->is_dead = 0;
    if (argv[5])
        philo->max_eat = ft_atoi(argv[5]);
    else
        philo->max_eat = -1;
    philo->ate = 0;
    pthread_mutex_init(&philo->fork, NULL);
    philo->next = NULL;
}

t_philo *init_philos(char **argv, int n_philo, long start_time)
{
    t_philo *head = NULL;
    t_philo *prev = NULL;
    t_philo *philo = NULL;
    int *shared_is_dead = malloc(sizeof(int));
    pthread_mutex_t *shared_mutex = malloc(sizeof(pthread_mutex_t));
    *shared_is_dead = 0;
    pthread_mutex_init(shared_mutex, NULL);

    while (n_philo-- > 0)
    {
        philo = malloc(sizeof(t_philo));
        if (!philo)
            error("Philo malloc fail");
        init_values(philo, n_philo, argv, start_time);

        philo->is_dead = shared_is_dead;
        philo->is_dead_mutex = shared_mutex;
        
        if (!head)
            head = philo;
        if (prev)
            prev->next = philo;
        prev = philo;
        if (pthread_create(&philo->thread, NULL, routine, (void *)philo) != 0)
            error("Thread creation failed");
    }
    if(prev)
        prev->next = head;
    return (head);
}

