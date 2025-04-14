/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimendon <dimendon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:16:32 by dimendon          #+#    #+#             */
/*   Updated: 2025/04/14 18:31:40 by dimendon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* philosophers take turns eating, thinking, sleeping
cant eat and think/sleep or vice-versa

theres forks and they are as many as philosophers
when philosophers eat they need 2 forks
when they finish eating they put them back and sleep
when they awake, they start thinking

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

int main(int argc, char **argv)
{
    
    if(argc == 5 || argc == 6)
    {
        printf("test %s \n", argv[1]);  
    }
    else
        printf("./philo [nbr_of_philosophers] [t_t_die] [t_t_eat] [t_t_sleep] [(optional)[nbr_times_philo_eat]]\n");
}