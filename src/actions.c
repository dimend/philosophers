/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:02:37 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/30 17:47:25 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

short int	is_anyone_dead(t_philo *philo)
{
	long		death_time;
	short int	dead;

	pthread_mutex_lock(&philo->table->is_dead_mutex);
	dead = philo->table->is_dead;
	death_time = get_time(philo) - philo->last_meal;
	pthread_mutex_unlock(&philo->table->is_dead_mutex);
	if (dead == 0)
	{
		if (death_time >= philo->tt_die)
		{
			pthread_mutex_lock(&philo->table->is_dead_mutex);
			philo->table->is_dead = 1;
			pthread_mutex_unlock(&philo->table->is_dead_mutex);
			safe_print(philo, NULL);
			dead = 1;
		}
	}
	return (dead);
}

short int	take_forks(t_philo *philo)
{
	if (philo == philo->next)
		return (is_single_philo(philo));
	if (lock_forks(&philo->fork, philo))
		return (1);
	if (lock_forks(&philo->next->fork, philo))
	{
		pthread_mutex_unlock(&philo->fork);
		return (1);
	}
	return (0);
}

short int	eating(t_philo *philo)
{
	long	start;
	long	end;

	start = get_time(philo);
	end = start + philo->tt_eat;
	if (is_anyone_dead(philo))
		return (1);
	safe_print(philo, "is eating");
	while (get_time(philo) < end)
	{
		if (is_anyone_dead(philo))
		{
			unlock_forks(philo);
			return (1);
		}
		usleep(1000);
	}
	philo->last_meal = get_time(philo);
	philo->ate++;
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
		if (is_anyone_dead(philo))
			return (1);
		usleep(1000);
	}
	return (0);
}

short int	thinking(t_philo *philo)
{
	if (is_anyone_dead(philo))
		return (1);
	safe_print(philo, "is thinking");
	if (philo->n_philo % 2 != 0 && philo->t_id % 2 == 0 && philo->ate > 0)
		usleep(500 * philo->tt_eat);
	return (0);
}
