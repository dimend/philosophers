/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:24:52 by dimendon          #+#    #+#             */
/*   Updated: 2025/04/29 15:50:57 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    printf("Thread nr: %d\n", philo->t_nbr);
    printf("Time to die: %d\n", philo->tt_die);
    printf("Time to eat: %d\n", philo->tt_eat);
    printf("Time to sleep: %d\n", philo->tt_sleep);
    free(philo);
}

void init_threads(char **argv)
{
    int n_philo;
    pthread_t *threads;
    int i;
    
    n_philo = atoi(argv[1]);
    threads = malloc(n_philo * sizeof(pthread_t));
    if (!threads)
        error("Malloc fail");
    i = -1;
    while (++i < n_philo)
    {
        t_philo *philo = malloc(sizeof(t_philo));
        if (!philo)
            error("Malloc fail");
        philo->t_nbr = i;
        philo->tt_die = atoi(argv[2]);
        philo->tt_eat = atoi(argv[3]);
        philo->tt_sleep = atoi(argv[4]);
        if (pthread_create(&threads[i], NULL, routine, (void *)philo))
        {
            free(philo);
            error("Thread creation failed");
        }
    }
}
