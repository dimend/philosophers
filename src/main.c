/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:22:30 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/02 20:19:23 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philos_death(t_philo *philo)
{
	long	now;
	long	last_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_mutex);
	now = get_time(philo);
	if ((now - last_meal) >= philo->tt_die)
	{
		safe_print(philo, NULL);
		pthread_mutex_lock(&philo->table->is_dead_mutex);
		philo->table->is_dead = 1;
		pthread_mutex_unlock(&philo->table->is_dead_mutex);
		return (1);
	}
	return (0);
}

int	check_all_philos(t_table *table)
{
	t_philo	*philo;
	t_philo	*start;

	philo = table->head_philo;
	start = philo;
	while (1)
	{
		if (check_philos_death(philo))
			return (1);
		philo = philo->next;
		if (philo == start)
			break ;
	}
	return (0);
}

void	*death_monitor(void *arg)
{
	t_table	*table;
	int		n_philo;

	table = (t_table *)arg;
	n_philo = table->head_philo->n_philo;
	while (1)
	{
		pthread_mutex_lock(&table->have_eaten_mutex);
		if (table->have_eaten == n_philo)
		{
			pthread_mutex_unlock(&table->have_eaten_mutex);
			break ;
		}
		pthread_mutex_unlock(&table->have_eaten_mutex);
		if (check_all_philos(table))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo	*head_philo;
	t_table	table;
	long	start_time;
	int		n_threads;

	head_philo = NULL;
	n_threads = 0;
	if (!(argc == 5 || argc == 6) || check_params(argv) != 0)
	{
		printf("./philo [n_philos] [t_die] [t_eat] [t_sleep] [(opt) max_eat]\n");
		return (1);
	}
	start_time = get_time(NULL);
	head_philo = init_table(argv, start_time, &table);
	if (head_philo == NULL)
		return (1);
	table.head_philo = head_philo;
	n_threads = start_threading(head_philo, ft_atoi(argv[1]), &table);
	if (n_threads == -1)
		return (1);
	cleanup(head_philo, ft_atoi(argv[1]), &table, 1);
	return (0);
}
