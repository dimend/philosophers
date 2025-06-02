/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 14:33:21 by dimendon          #+#    #+#             */
/*   Updated: 2025/06/02 20:00:01 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

short int	check_params(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		if (argv[i] == NULL || argv[i][0] == '\0')
			return (1);
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9' || (argv[i][0] == '0'
					&& argv[i][1] == '\0'))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int	ret;
	int	sign;
	int	i;

	i = 0;
	ret = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		ret = (ret * 10) + str[i] - '0';
		i++;
	}
	return (ret * sign);
}

void	routine_wait(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_mutex);
	pthread_mutex_unlock(&philo->table->print_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = get_time(philo);
	pthread_mutex_unlock(&philo->meal_mutex);
	thinking(philo);
}
