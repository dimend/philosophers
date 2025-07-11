/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:24:52 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/02 20:43:10 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	routine_wait(philo);
	if (philo->t_id % 2 == 0)
		usleep(2000);
	while (1)
	{
		if (take_forks(philo) == 0)
		{
			if (eating(philo) == 0)
				unlock_forks(philo);
			else
				break ;
		}
		if (sleeping(philo) != 0)
			break ;
		if (thinking(philo) != 0)
			break ;
	}
	if (philo->using_fork == 1)
		pthread_mutex_unlock(&philo->fork);
}

t_philo	*init_table(char **argv, long start_time, t_table *table)
{
	t_philo	*head_philo;

	table->is_dead = 0;
	table->have_eaten = 0;
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&table->is_dead_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&table->print_mutex);
		return (NULL);
	}
	if (pthread_mutex_init(&table->have_eaten_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&table->print_mutex);
		pthread_mutex_destroy(&table->is_dead_mutex);
		return (NULL);
	}
	head_philo = init_philos(argv, ft_atoi(argv[1]), start_time, table);
	if (!head_philo)
	{
		pthread_mutex_destroy(&table->print_mutex);
		pthread_mutex_destroy(&table->is_dead_mutex);
		pthread_mutex_destroy(&table->have_eaten_mutex);
	}
	return (head_philo);
}

void	init_values(t_philo *philo, int n_philo, char **argv, long start_time)
{
	philo->n_philo = ft_atoi(argv[1]);
	philo->t_id = philo->n_philo - n_philo;
	philo->tt_die = ft_atoi(argv[2]);
	philo->tt_eat = ft_atoi(argv[3]);
	philo->tt_sleep = ft_atoi(argv[4]);
	philo->start_time = start_time;
	philo->last_meal = start_time;
	if (argv[5])
		philo->max_eat = ft_atoi(argv[5]);
	else
		philo->max_eat = -1;
	philo->ate = 0;
	philo->next = NULL;
	philo->using_fork = 0;
}

t_philo	*init_philos(char **argv, int n_philo, long start_time, t_table *table)
{
	t_philo	*head;
	t_philo	*philo;
	int		i;
	t_philo	*new;

	head = NULL;
	philo = NULL;
	i = n_philo;
	while (i-- > 0)
	{
		new = create_philo(i, argv, start_time, table);
		if (!new)
		{
			cleanup(head, n_philo - i - 1, table, 0);
			return (NULL);
		}
		if (!head)
			head = new;
		else
			philo->next = new;
		philo = new;
	}
	if (philo && head)
		philo->next = head;
	return (head);
}

short int	start_threading(t_philo *head, int n_philo, t_table *table)
{
	t_philo	*philo;

	philo = head;
	pthread_mutex_lock(&philo->table->print_mutex);
	while (n_philo-- > 0)
	{
		if (pthread_create(&philo->thread, NULL, (void *)routine,
				(void *)philo) != 0)
		{
			printf("Thread creation failed\n");
			cleanup(head, philo->n_philo - n_philo - 1, philo->table, 1);
			return (-1);
		}
		philo = philo->next;
	}
	pthread_mutex_unlock(&philo->table->print_mutex);
	if (pthread_create(&table->death_monitor_thread, NULL, death_monitor,
			(void *)table) != 0)
	{
		printf("Failed to create death monitor thread\n");
		join_threads(head, head->n_philo);
		return (-1);
	}
	return (head->n_philo);
}
