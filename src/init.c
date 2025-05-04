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
    printf("Philo ID: %d\n", philo->t_id);
    printf("Time to die: %d\n", philo->tt_die);
    printf("Time to eat: %d\n", philo->tt_eat);
    printf("Time to sleep: %d\n", philo->tt_sleep);
    free(philo);
    return (NULL);
}
void init_values(t_philo *philo,  int n_philo, char **argv)
{
    philo->t_id = n_philo;
    philo->tt_die = ft_atoi(argv[2]);
    philo->tt_eat = ft_atoi(argv[3]);
    philo->tt_sleep = ft_atoi(argv[4]);
    pthread_mutex_init(&philo->fork, NULL);
    philo->next = NULL;
}

t_philo *init_philos(char **argv, int n_philo)
{
    int i;
    t_philo *head = NULL;
    t_philo *prev = NULL;
    
    i = -1;
    while (++i < n_philo)
    {
        t_philo *philo = malloc(sizeof(t_philo));
        if (!philo)
            error("Philo malloc fail");
        init_values(philo, i, argv);
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
