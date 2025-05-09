/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:22:30 by dimendon          #+#    #+#             */
/*   Updated: 2025/05/09 18:28:24 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* philosophers take turns eating, thinking, sleeping
cant eat and think/sleep or vice-versa

theres forks and they are as many as philosophers
when philosophers eat they need 2 forks
when they finish eating they put them back and sleep
when they wake up, they start thinking

the sim stops when one of them dies

a philosopher needs to eat and should never starve
cant communicate with eachother
dont know if someone is about to die
philosophers should avoid dying */


/* ARGS
number_of_philosophers (also forks) - 
time_to_die (in ms) - if they dont eat in that time they die
time_to_eat - how long they eat and hold 2 forks
time_to_sleep
[number_of_times_each_philosopher_must_eat] (optional) 
- if all philosphers eat this ammount of time the sim stops */

/* each philospher has a number
they all sit between philosopher n+1 and n-1 */

/* any state change of a philosopher must be formatted as follows:
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died
A displayed state message should not overlap with another message.
A death must be displayed within 10 ms
*/

#include "philo.h"

void join_free(t_philo *head_philo, int n_philo)
{
    int i;
    t_philo *current_philo;
    t_philo *next_philo;
    void *status;

    int *shared_is_dead = head_philo->is_dead;
    
    i = -1;
    current_philo = head_philo;

    while (++i < n_philo)
    {
        pthread_mutex_lock(head_philo->is_dead_mutex);
        if (*shared_is_dead)
        {
            pthread_mutex_unlock(head_philo->is_dead_mutex);
            printf("Someone died.\n");
            break;
        }
        pthread_mutex_unlock(head_philo->is_dead_mutex);
        current_philo = current_philo->next;
    }

    i = -1;
    current_philo = head_philo;
    while (++i < n_philo)
    {
        pthread_join(current_philo->thread, &status);
        next_philo = current_philo->next;
        pthread_mutex_destroy(&current_philo->fork);
        free(current_philo);

        current_philo = next_philo;
    }
}

void error(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    t_philo *head_philo;
    long start_time;
    
    start_time = get_time();
    head_philo = NULL;
    if ((argc == 5 || argc == 6) && check_params(argv) == 0)
    {
        head_philo = init_philos(argv, ft_atoi(argv[1]), start_time);
        join_free(head_philo, ft_atoi(argv[1]));
    }
    else
    {
        printf("./philo [nbr_of_philosophers] [t_t_die] [t_t_eat] [t_t_sleep] [(optional)[nbr_times_philo_eat]]\n");
    }

    return (0);
}
