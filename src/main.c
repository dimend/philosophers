/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:22:30 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/30 17:49:33 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* void set_previous_fork(t_philo *philo)
{
	int i;
	t_philo *current;

	current = philo;
	i = philo->n_philo;
	
    while (i > 0)
	{
		current->next->previous_fork = &current->fork;
		i--;
		current = current->next;
	}
} */

int	main(int argc, char **argv)
{
	t_philo	*head_philo;
	t_table	table;
	long	start_time;
	int		n_threads;

	head_philo = NULL;
	start_time = get_time(NULL);
	n_threads = 0;
	if (!(argc == 5 || argc == 6) || check_params(argv) != 0)
	{
		printf("./philo [n_philos] [t_die] [t_eat] [t_sleep] [(opt) max_eat]\n");
		return (1);
	}
	head_philo = init_table(argv, start_time, &table);
	if (head_philo == NULL)
		return (1);
	set_previous_fork(head_philo);
	n_threads = start_threading(head_philo, ft_atoi(argv[1]));
	if (n_threads == -1)
		return (1);
	cleanup(head_philo, ft_atoi(argv[1]), &table, 1);
	return (0);
}
