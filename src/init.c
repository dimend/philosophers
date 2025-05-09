/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:24:52 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/09 15:13:41 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    long timestamp;
    long death;
    int eat;
    
    eat = 0;
    while (1)
    {
        if ((philo->max_eat != -1 && eat >= philo->max_eat) || philo->is_dead)
            break;

        timestamp = get_time();
        death = timestamp - philo->last_meal;
        if (death > philo->tt_die)
        {
            printf("[%ld ms] %d has died\n", timestamp, philo->t_id);
            philo->is_dead = 1;
            return (void *)-1;
        }

        if (philo->t_id % 2 == 0)
        {
            pthread_mutex_lock(&philo->fork);
            timestamp = get_time() - philo->start_time;
            printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);

            pthread_mutex_lock(&philo->next->fork);
            timestamp = get_time() - philo->start_time;
            printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);
        }
        else
        {
            pthread_mutex_lock(&philo->next->fork);
            timestamp = get_time() - philo->start_time;
            printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);

            pthread_mutex_lock(&philo->fork);
            timestamp = get_time() - philo->start_time;
            printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);
        }

        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d is eating\n", timestamp, philo->t_id);
        usleep(philo->tt_eat * 1000);
        eat++;
        philo->last_meal = get_time();

        pthread_mutex_unlock(&philo->fork);
        pthread_mutex_unlock(&philo->next->fork);

        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d is sleeping\n", timestamp, philo->t_id);
        usleep(philo->tt_sleep * 1000);

        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d is thinking\n", timestamp, philo->t_id);
    }
    return (NULL);
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
    pthread_mutex_init(&philo->fork, NULL);
    philo->next = NULL;
}

t_philo *init_philos(char **argv, int n_philo)
{
    int i;
    t_philo *head = NULL;
    t_philo *prev = NULL;
    long start_time;
    
    start_time = get_time();
    
    i = -1;
    while (++i < n_philo)
    {
        t_philo *philo = malloc(sizeof(t_philo));
        if (!philo)
            error("Philo malloc fail");
        init_values(philo, i, argv, start_time);
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

