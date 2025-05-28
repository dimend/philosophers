/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:33:18 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/28 14:33:19 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;
	long			time;

	time = 0;
	if (gettimeofday(&tv, NULL) >= 0)
		time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	else
		time = -1;
	return (time);
}

void	safe_print(t_philo *philo, const char *message)
{
	long	timestamp;
	int		dead;

	pthread_mutex_lock(&philo->table->print_mutex);
	pthread_mutex_lock(&philo->table->is_dead_mutex);
	dead = *(&philo->table->is_dead);
	pthread_mutex_unlock(&philo->table->is_dead_mutex);
	if (dead && message != NULL)
	{
		pthread_mutex_unlock(&philo->table->print_mutex);
		return ;
	}
	timestamp = get_time() - philo->start_time;
	if (message == NULL)
		printf("%ld %d died\n", timestamp, philo->t_id);
	else
		printf("%ld %d %s\n", timestamp, philo->t_id, message);
	pthread_mutex_unlock(&philo->table->print_mutex);
}
