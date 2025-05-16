#include "philo.h"

long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void safe_print(t_philo *philo, const char *message, long timestamp)
{
    int dead;

    pthread_mutex_lock(philo->print_mutex);
    pthread_mutex_lock(philo->is_dead_mutex);

    dead = *(philo->is_dead);
    pthread_mutex_unlock(philo->is_dead_mutex);
    if (dead == 1)
    {
        pthread_mutex_unlock(philo->print_mutex);
        return;
    }
        
    if (!message)
    {
        printf("%ld %d died\n", timestamp, philo->t_id);
        pthread_mutex_lock(philo->is_dead_mutex);
        *(philo->is_dead) = 1;
        pthread_mutex_unlock(philo->is_dead_mutex);
        pthread_mutex_unlock(philo->print_mutex);
        return;
    }
    printf("%ld %d %s\n", timestamp, philo->t_id, message);
    pthread_mutex_unlock(philo->print_mutex);
}

short int take_fork(t_philo *philo)
{
    if (is_anyone_dead(philo))
        return (1);

    if (forks(philo))
        return (1);

    return 0;
}

void start_threading(t_philo *head, int n_philo)
{
    t_philo *philo = head;
    int i = 0;

    while (i < n_philo)
    {
        if (pthread_create(&philo->thread, NULL, routine, (void *)philo) != 0)
            error("Thread creation failed");
        philo = philo->next;
        i++;
    }
}


