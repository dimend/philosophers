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
        if (death_time > philo->tt_die)
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

    if(philo->t_id % 2 == 0)
        usleep(1000);
    pthread_mutex_lock(&philo->fork);
    timestamp = get_time() - philo->start_time;
    if (is_anyone_dead(philo))
    {
        pthread_mutex_unlock(&philo->fork);
        return (1);
    }
    safe_print(philo, "has taken a fork", timestamp);
    if (philo == philo->next)
    {
        safe_print(philo, NULL, timestamp);
        pthread_mutex_unlock(&philo->fork);
        return (1);
    }
    pthread_mutex_lock(&philo->next->fork);
    timestamp = get_time() - philo->start_time;
    safe_print(philo, "has taken a fork", timestamp);
    return (0);
}

short int eating(t_philo *philo)
{
    long timestamp = get_time() - philo->start_time;
    long remaining_time = philo->tt_eat * 500;

    if (is_anyone_dead(philo))
        return (1);

    safe_print(philo, "is eating", timestamp);
    while (remaining_time > 0)
    {
        if (is_anyone_dead(philo))
            return (1);
        usleep(500);
        remaining_time -= 500;
    }
    philo->ate++;
    philo->last_meal = get_time();
    return (0);
}


short int sleeping(t_philo *philo)
{
    long timestamp = get_time() - philo->start_time;
    long remaining_time = philo->tt_sleep * 500;

    if (is_anyone_dead(philo))
        return (1);

    safe_print(philo, "is sleeping", timestamp);
    while (remaining_time > 0)
    {
        if (is_anyone_dead(philo))
            return (1);

        usleep(500);
        remaining_time -= 500;
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
