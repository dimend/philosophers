#include "philo.h"

short int is_anyone_dead(t_philo *philo)
{
    short int dead;
    long death_time;

    pthread_mutex_lock(philo->is_dead_mutex);
    dead = *(philo->is_dead);
    pthread_mutex_unlock(philo->is_dead_mutex);
    if (dead == 0)
    {
        death_time = get_time() - philo->last_meal;
        if (death_time >= philo->tt_die)
        {
            safe_print(philo, NULL , death_time);
            dead = 1;
        }
    }
    return (dead);
}

short int forks(t_philo *philo)
{
    long timestamp;

    if (philo->t_id % 2 == 0)
        usleep(1000);

    //lock own fork
    timestamp = get_time() - philo->start_time;
    if(lock_forks(&philo->fork, philo, &timestamp))
        return (1);

    //check if single philo
    if (philo == philo->next)
        return (is_single_philo(philo, timestamp));

    //grab second fork
    if (grab_fork(philo, &timestamp))
        return (1);

    return (0);
}

short int eating(t_philo *philo)
{
    long start = get_time();
    long end = start + philo->tt_eat;

    philo->ate++;
    philo->last_meal = start;

    safe_print(philo, "is eating", start - philo->start_time);
    while (get_time() < end)
    {
        if (is_anyone_dead(philo))
            return (1);
        usleep(500);
    }
    return (0);
}

short int sleeping(t_philo *philo)
{
    long start = get_time();
    long end = start + philo->tt_sleep;

    if (is_anyone_dead(philo))
        return (1);

    safe_print(philo, "is sleeping", start - philo->start_time);
    while (get_time() < end)
    {
        if (is_anyone_dead(philo))
            return (1);

        usleep(500);
    }
    return (0);
}

short int thinking(t_philo *philo)
{
    long timestamp = get_time() - philo->start_time;

    if (is_anyone_dead(philo))
        return (1);

    safe_print(philo, "is thinking", timestamp);
    return (0);
}
