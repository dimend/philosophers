/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:33:18 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/02 20:07:59 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(t_philo *philo)
{
	struct timeval	tv;
	long			time;

	time = 0;
	if (gettimeofday(&tv, NULL) >= 0)
		time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	else
	{
		if (philo == NULL)
		{
			printf("Failed getting time in main.");
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("Failed getting time during routine.");
			cleanup(philo, philo->n_philo, philo->table, 1);
			exit(EXIT_FAILURE);
		}
	}
	return (time);
}

void	safe_print(t_philo *philo, const char *message)
{
	long	timestamp;
	int		dead;

	pthread_mutex_lock(&philo->table->print_mutex);
	pthread_mutex_lock(&philo->table->is_dead_mutex);
	dead = philo->table->is_dead;
	pthread_mutex_unlock(&philo->table->is_dead_mutex);
	if (dead && message != NULL)
	{
		pthread_mutex_unlock(&philo->table->print_mutex);
		return ;
	}
	timestamp = get_time(philo) - philo->start_time;
	if (message == NULL)
		printf("%ld %d died\n", timestamp, philo->t_id + 1);
	else
		printf("%ld %d %s\n", timestamp, philo->t_id + 1, message);
	pthread_mutex_unlock(&philo->table->print_mutex);
}

void	join_threads(t_philo *head_philo, int n_philo)
{
	int		i;
	t_philo	*current;
	void	*status;

	current = head_philo;
	i = 0;
	while (i < n_philo)
	{
		pthread_join(current->thread, &status);
		current = current->next;
		i++;
	}
}

void	cleanup(t_philo *head_philo, int n_philo, t_table *table,
		int join_threads_flag)
{
	int		i;
	t_philo	*current;
	t_philo	*next;

	if (join_threads_flag)
	{
		join_threads(head_philo, n_philo);
		pthread_join(table->death_monitor_thread, NULL);
	}
	pthread_mutex_destroy(&table->is_dead_mutex);
	pthread_mutex_destroy(&table->have_eaten_mutex);
	pthread_mutex_destroy(&table->print_mutex);
	current = head_philo;
	i = 0;
	while (i < n_philo)
	{
		pthread_mutex_destroy(&current->fork);
		pthread_mutex_destroy(&current->meal_mutex);
		next = current->next;
		free(current);
		current = next;
		i++;
	}
}

t_philo	*create_philo(int id, char **argv, long start_time, t_table *table)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	init_values(philo, id + 1, argv, start_time);
	philo->table = table;
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
	{
		free(philo);
		return (NULL);
	}
	if (pthread_mutex_init(&philo->meal_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&philo->fork);
		free(philo);
		return (NULL);
	}
	return (philo);
}
