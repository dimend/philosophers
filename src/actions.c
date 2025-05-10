#include "philo.h"

void    death(t_philo *philo)
{
    long timestamp = get_time();
    long death_time = timestamp - philo->last_meal;

    if (death_time > philo->tt_die)
    {
        printf("[%ld ms] %d has died\n", timestamp, philo->t_id);

        pthread_mutex_lock(philo->is_dead_mutex);
        *(philo->is_dead) = 1;
        pthread_mutex_unlock(philo->is_dead_mutex);
    }
}


void    take_fork(t_philo *philo)
{
    long timestamp;

    if (philo->t_id % 2 == 0)
    {
        pthread_mutex_lock(&philo->fork);
        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);

        pthread_mutex_lock(&philo->next->fork);
        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);
    }
    else
    {
        pthread_mutex_lock(&philo->next->fork);
        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);

        pthread_mutex_lock(&philo->fork);
        timestamp = get_time() - philo->start_time;
        printf("[%ld ms] %d has taken a fork\n", timestamp, philo->t_id);
    }
}

void    eating(t_philo *philo)
{
    long timestamp;

    timestamp = get_time() - philo->start_time;
    printf("[%ld ms] %d is eating\n", timestamp, philo->t_id);
    usleep(philo->tt_eat * 1000);
    philo->ate++;
    philo->last_meal = get_time();
}

void    sleeping(t_philo *philo)
{
    long timestamp;

    timestamp = get_time() - philo->start_time;
    printf("[%ld ms] %d is sleeping\n", timestamp, philo->t_id);
    usleep(philo->tt_sleep * 1000);
}

void    thinking(t_philo *philo)
{
    long timestamp;

    timestamp = get_time() - philo->start_time;
    printf("[%ld ms] %d is thinking\n", timestamp, philo->t_id);
}
