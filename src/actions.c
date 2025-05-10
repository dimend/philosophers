#include "philo.h"

short int is_anyone_dead(t_philo *philo)
{
    short int dead;

    pthread_mutex_lock(philo->is_dead_mutex);
    dead = *(philo->is_dead);
    pthread_mutex_unlock(philo->is_dead_mutex);

    if (dead == 0)
    {
        long timestamp = get_time() - philo->start_time;
        long death_time = timestamp - (philo->last_meal - philo->start_time);

        if (death_time > philo->tt_die)
        {
            printf("[%ld ms] %d has died\n", timestamp, philo->t_id);

            pthread_mutex_lock(philo->is_dead_mutex);
            *(philo->is_dead) = 1;
            pthread_mutex_unlock(philo->is_dead_mutex);

            dead = 1;
        }
    }

    return (dead);
}

short int take_fork(t_philo *philo)
{
    long timestamp;

    if (is_anyone_dead(philo))
        return (1);

    if (philo->t_id % 2 == 0)
    {
        pthread_mutex_lock(&philo->fork);
        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);

        if (is_anyone_dead(philo))
        {
            pthread_mutex_unlock(&philo->fork);
            return (1);
        }

        pthread_mutex_lock(&philo->next->fork);
        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);
    }
    else
    {
        pthread_mutex_lock(&philo->next->fork);
        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);

        if (is_anyone_dead(philo))
        {
            pthread_mutex_unlock(&philo->next->fork);
            return (1);
        }

        pthread_mutex_lock(&philo->fork);
        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);
    }

    return (0);
}

short int eating(t_philo *philo)
{
    long timestamp = get_time() - philo->start_time;
    long remaining_time = philo->tt_eat * 1000;

    if (is_anyone_dead(philo))
        return (1);
    printf("[%ld ms] %d is eating\n", timestamp, philo->t_id);
    while (remaining_time > 0)
    {
        if (is_anyone_dead(philo))
            return (1);
        usleep(1000);
        remaining_time -= 1000;
    }

    philo->ate++;
    philo->last_meal = get_time();
    return (0);
}


short int sleeping(t_philo *philo)
{
    long timestamp = get_time() - philo->start_time;
    long remaining_time = philo->tt_sleep * 1000;

    if (is_anyone_dead(philo))
        return (1);
    printf("[%ld ms] %d is sleeping\n", timestamp, philo->t_id);
    while (remaining_time > 0)
    {
        if (is_anyone_dead(philo))
            return (1);
        usleep(1000);
        remaining_time -= 1000;
    }
    return (0);
}

short int thinking(t_philo *philo)
{
    long timestamp = get_time() - philo->start_time;

    if (is_anyone_dead(philo))
        return (1);

    printf("[%ld ms] %d is thinking\n", timestamp, philo->t_id);
    return (0);
}
