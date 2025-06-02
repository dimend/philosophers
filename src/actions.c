/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:02:37 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/02 20:58:04 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

short int	is_anyone_dead(t_philo *philo)
{
	short int	dead;

	pthread_mutex_lock(&philo->table->is_dead_mutex);
	dead = philo->table->is_dead;
	pthread_mutex_unlock(&philo->table->is_dead_mutex);
	return (dead);
}

short int	take_forks(t_philo *philo)
{
	if (philo == philo->next)
		return (is_single_philo(philo));
	if (lock_forks(&philo->fork, philo))
		return (1);
	if (lock_forks(&philo->next->fork, philo))
		return (1);
	return (0);
}

short int	eating(t_philo *philo)
{
	long	end;

	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time(philo);
	pthread_mutex_unlock(&philo->meal_mutex);
	safe_print(philo, "is eating");
	end = get_time(philo) + philo->tt_eat;
	while (get_time(philo) < end)
		usleep(1000);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->ate++;
	pthread_mutex_unlock(&philo->meal_mutex);
	if (philo->max_eat != -1 && philo->ate == philo->max_eat)
	{
		pthread_mutex_lock(&philo->table->have_eaten_mutex);
		philo->table->have_eaten++;
		if (philo->table->have_eaten == philo->n_philo)
		{
			pthread_mutex_lock(&philo->table->is_dead_mutex);
			philo->table->is_dead = 1;
			pthread_mutex_unlock(&philo->table->is_dead_mutex);
		}
		pthread_mutex_unlock(&philo->table->have_eaten_mutex);
	}
	return (0);
}

short int	sleeping(t_philo *philo)
{
	long	start;
	long	end;

	start = get_time(philo);
	end = start + philo->tt_sleep;
	if (is_anyone_dead(philo))
		return (1);
	safe_print(philo, "is sleeping");
	while (get_time(philo) < end)
	{
		usleep(1000);
	}
	return (0);
}

short int	thinking(t_philo *philo)
{
	safe_print(philo, "is thinking");
	return (0);
}
