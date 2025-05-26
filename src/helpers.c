/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:39:32 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/26 20:21:20 by dimendon         ###   ########.fr       */
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

short int check_and_update_max_eat(t_philo *philo)
{
    if (philo->max_eat == -1 || philo->ate != philo->max_eat)
        return 0;

    pthread_mutex_lock(&philo->table->have_eaten_mutex);
    *(&philo->table->have_eaten) += 1;
    if (*(&philo->table->have_eaten) == philo->n_philo)
    {
        pthread_mutex_lock(&philo->table->is_dead_mutex);
        *(&philo->table->is_dead) = 1;
        pthread_mutex_unlock(&philo->table->is_dead_mutex);
        pthread_mutex_unlock(&philo->table->have_eaten_mutex);
        return (1);
    }
    pthread_mutex_unlock(&philo->table->have_eaten_mutex);
    return (0);
}
