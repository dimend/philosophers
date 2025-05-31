/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:39:32 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/31 23:07:52 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork);
	philo->using_fork = 0;
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_lock(&philo->next->fork);
	philo->next->using_fork = 0;
	pthread_mutex_unlock(&philo->next->fork);
}

short int	try_take_fork(t_philo *philo)
{
	short int	success;

	success = 0;
	pthread_mutex_lock(&philo->fork);
	if (philo->using_fork == 0)
	{
		philo->using_fork = 1;
		success = 1;
	}
	else
	{
		success = 0;
	}
	pthread_mutex_unlock(&philo->fork);
	return (success);
}

short int	is_single_philo(t_philo *philo)
{
	usleep(1000 * philo->tt_die);
	pthread_mutex_lock(&philo->table->is_dead_mutex);
	philo->table->is_dead = 1;
	pthread_mutex_unlock(&philo->table->is_dead_mutex);
	safe_print(philo, NULL);
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

t_philo	*create_philos_loop(char **argv, long start_time, t_table *table,
		t_philo **head)
{
	t_philo	*philo;
	int		i;
	int		n_philo;

	philo = NULL;
	n_philo = ft_atoi(argv[1]);
	i = n_philo;
	while (i-- > 0)
	{
		philo = create_philo(i, argv, start_time, table);
		if (!philo)
		{
			cleanup(*head, n_philo - i - 1, table, 0);
			return (NULL);
		}
		if (!*head)
			*head = philo;
	}
	return (philo);
}
