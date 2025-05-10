#include "philo.h"

long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

short int is_anyone_dead(t_philo *philo)
{
    short int dead;

    pthread_mutex_lock(philo->is_dead_mutex);
    dead = *(philo->is_dead);
    pthread_mutex_unlock(philo->is_dead_mutex);

    return (dead);
}
