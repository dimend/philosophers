/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:39:32 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/23 18:17:07 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void unlock_forks(t_philo *philo)
{
        pthread_mutex_unlock(&philo->fork);
        pthread_mutex_unlock(&philo->next->fork);
}

short int lock_forks(pthread_mutex_t *fork, t_philo *philo)
{
    if (!is_anyone_dead(philo))
        pthread_mutex_lock(fork);
        
    if (is_anyone_dead(philo))
    {
        pthread_mutex_unlock(fork);
        return (1);
    }
    safe_print(philo, "has taken a fork");    
    return (0);
}

short int is_single_philo(t_philo *philo)
{
    usleep(1000 * philo->tt_die);
    safe_print(philo, "died");
    return (1);
}

