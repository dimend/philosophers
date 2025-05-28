/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:24:52 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/28 18:42:24 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	thinking(philo);
	if(philo->t_id != 1 && philo->n_philo%2 != 0)
	{
		if (philo->t_id % 2 == 0)
			usleep( (1000 * philo->tt_eat)+500 );
		else
			usleep(1000*philo->tt_eat);
	}
	else if(philo->t_id%2 == 0)
		usleep(500 * philo->tt_eat);
	while (1)
	{
		if (take_forks(philo) == 0)
		{
			if (eating(philo) == 0)
				unlock_forks(philo);
			else
			{
				unlock_forks(philo);
				break ;
			}
		}
		if (check_and_update_max_eat(philo))
			break ;
		if (sleeping(philo) != 0)
			break ;
		if (thinking(philo) != 0)
			break ;
	}
}

t_philo	*init_table(char **argv, long start_time, t_table *table)
{
	t_philo	*head_philo;

	head_philo = NULL;
	table->is_dead = 0;
	table->have_eaten = 0;
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&table->is_dead_mutex, NULL) != 0)
		return (NULL);
	if (pthread_mutex_init(&table->have_eaten_mutex, NULL) != 0)
		return (NULL);
	head_philo = init_philos(argv, ft_atoi(argv[1]), start_time, table);
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
	philo->previous = NULL;
}

t_philo	*init_philos(char **argv, int n_philo, long start_time, t_table *table)
{
	t_philo	*head;
	t_philo	*prev;
	t_philo	*philo;
	int		i;

	head = NULL;
	prev = NULL;
	philo = NULL;
	i = n_philo;
	while (i-- > 0)
	{
		philo = create_philo(i, argv, start_time, table);
		if (!philo)
			return (NULL);
		if (!head)
			head = philo;
		if (prev)
		{
			prev->next = philo;
			philo->previous = prev;
		}
		prev = philo;
	}
	if (prev && head)
	{
		prev->next = head;
		head->previous = prev;
	}
	return (head);
}

short int	start_threading(t_philo *head, int n_philo)
{
	t_philo	*philo;
	int		i;

	philo = head;
	i = 0;
	while (i < n_philo)
	{
		if (pthread_create(&philo->thread, NULL, (void *)routine,
				(void *)philo) != 0)
			return (i);
		philo = philo->next;
		i++;
	}
	return (i);
}
