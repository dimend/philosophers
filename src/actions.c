#include "philo.h"

short int is_anyone_dead(t_philo *philo)
{
    long death_time;
    short int dead;

    pthread_mutex_lock(philo->is_dead_mutex);
    dead = *(philo->is_dead);
    death_time = get_time() - philo->last_meal;
    pthread_mutex_unlock(philo->is_dead_mutex);

    if (dead == 0)
    {
        if (death_time >= philo->tt_die)
        {
            pthread_mutex_lock(philo->is_dead_mutex);
            *(philo->is_dead) = 1;
            pthread_mutex_unlock(philo->is_dead_mutex);
            safe_print(philo, NULL);
            dead = 1;
        }
    }
    return (dead);
}

short int take_forks(t_philo *philo)
{
    pthread_mutex_t *first;
    pthread_mutex_t *second;

    if (philo == philo->next)
        return (is_single_philo(philo));

    if((philo->ate - philo->next->ate) > 1)
    {
        usleep(1000);
    }
        

    if(philo->t_id % 2 == 0)
    {
        first = &philo->fork;
        second = &philo->next->fork;
    } else {
        second = &philo->fork;
        first = &philo->next->fork;
    }

    if (lock_forks(first, philo))
        return (1);

    if (lock_forks(second, philo))
    {
        pthread_mutex_unlock(first);
        return (1);
    }

    return (0);
}

short int eating(t_philo *philo)
{
    long start = get_time();
    long end = start + philo->tt_eat;

    philo->ate++;


    philo->last_meal = start;
    if (is_anyone_dead(philo))
        return (1);

    safe_print(philo, "is eating");
    while (get_time() < end)
    {
        if (is_anyone_dead(philo))
        {
            unlock_forks(philo);
            return (1);
        }
            
        usleep(1000);
    }
    return (0);
}

short int sleeping(t_philo *philo)
{
    long start = get_time();
    long end = start + philo->tt_sleep;

    if (is_anyone_dead(philo))
        return (1);

    safe_print(philo, "is sleeping");
    while (get_time() < end)
    {
        if (is_anyone_dead(philo))
            return (1);

        usleep(1000);
    }
    return (0);
}

short int thinking(t_philo *philo)
{
    if(philo->n_philo%2 != 0 && philo->t_id%2 ==0 )
        usleep(3000);

    if (is_anyone_dead(philo))
        return (1);

    safe_print(philo, "is thinking");
    return (0);
}
