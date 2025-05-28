/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:22:30 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/28 16:49:05 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_free(t_philo *head_philo, int n_philo, t_table *table)
{
	int		i;
	t_philo	*current_philo;
	t_philo	*next_philo;
	void	*status;

	i = 0;
	current_philo = head_philo;
	while (i++ < n_philo)
	{
		pthread_join(current_philo->thread, &status);
		current_philo = current_philo->next;
	}
	pthread_mutex_destroy(&table->is_dead_mutex);
	pthread_mutex_destroy(&table->have_eaten_mutex);
	pthread_mutex_destroy(&table->print_mutex);
	i = 0;
	current_philo = head_philo;
	while (i++ < n_philo)
	{
		next_philo = current_philo->next;
		pthread_mutex_destroy(&current_philo->fork);
		free(current_philo);
		current_philo = next_philo;
	}
}

int	main(int argc, char **argv)
{
	t_philo	*head_philo;
	t_table	table;
	long	start_time;
	int		n_threads;

	head_philo = NULL;
	start_time = get_time();
	n_threads = 0;
	if (!(argc == 5 || argc == 6) || check_params(argv) != 0)
	{
		printf("./philo [n_philos] [t_die] [t_eat] [t_sleep] [(opt) max_eat]\n");
		return (1);
	}
	head_philo = init_table(argv, start_time, &table);
	if (head_philo == NULL)
		return (1);
	n_threads = start_threading(head_philo, ft_atoi(argv[1]));
	if (n_threads != ft_atoi(argv[1]))
	{
		printf("Thread creation failed %d \n", n_threads);
		join_free(head_philo, n_threads, &table);
		return (1);
	}
	join_free(head_philo, ft_atoi(argv[1]), &table);
	return (0);
}
