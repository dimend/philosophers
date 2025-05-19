/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:39:32 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/19 18:50:03 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void unlock_forks(t_philo *philo)
{
    if (philo->n_philo%2 != 0)
        pthread_mutex_unlock(&philo->next->fork);
    else
    {
        if (philo->t_id%2 == 0)
            pthread_mutex_unlock(&philo->next->fork);
        else
            pthread_mutex_unlock(&philo->previous->fork);  
    }
    pthread_mutex_unlock(&philo->fork);
}

short int lock_forks(pthread_mutex_t *fork, t_philo *philo, long *timestamp)
{
    pthread_mutex_lock(fork);
    *timestamp = get_time() - philo->start_time;
    if (is_anyone_dead(philo))
    {
        pthread_mutex_unlock(fork);
        return (1);
    }
    safe_print(philo, "has taken a fork", *timestamp);
    return (0);
}

short int is_single_philo(t_philo *philo, long timestamp)
{
    usleep(1000 * philo->tt_die);
    safe_print(philo, "died", timestamp + philo->tt_die);
    pthread_mutex_unlock(&philo->fork);
    return (1);
}

short int grab_fork(t_philo *philo, long *timestamp)
{

    // check odd or even total ammount
    if (philo->n_philo % 2 != 0)
        return (lock_forks(&philo->next->fork, philo, timestamp));
    else
    {
        //sync between odd or even philos
        if (philo->t_id % 2 == 0)
            return (lock_forks(&philo->next->fork, philo, timestamp));
        else
            return (lock_forks(&philo->previous->fork, philo, timestamp));
    }
}
