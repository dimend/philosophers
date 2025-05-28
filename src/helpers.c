/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:39:32 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/28 18:00:25 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void unlock_forks(t_philo *philo)
{
    pthread_mutex_unlock(&philo->fork);
    pthread_mutex_unlock(&philo->next->fork);
}

short int	lock_forks(pthread_mutex_t *fork, t_philo *philo)
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

short int	is_single_philo(t_philo *philo)
{
	usleep(1000 * philo->tt_die);
	safe_print(philo, "died");
	return (1);
}

short int	check_and_update_max_eat(t_philo *philo)
{
	if (philo->max_eat == -1 || philo->ate != philo->max_eat)
		return (0);
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

t_philo	*create_philo(int id, char **argv, long start_time, t_table *table)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	init_values(philo, id, argv, start_time);
	philo->table = table;
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
	{
		free(philo);
		return (NULL);
	}
	return (philo);
}
