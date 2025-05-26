#include "philo.h"

long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void safe_print(t_philo *philo, const char *message)
{
    long timestamp;
    int dead;

    pthread_mutex_lock(&philo->table->print_mutex);

    pthread_mutex_lock(&philo->table->is_dead_mutex);
    dead = *(&philo->table->is_dead);
    pthread_mutex_unlock(&philo->table->is_dead_mutex);

    if (dead && message != NULL)
    {
        pthread_mutex_unlock(&philo->table->print_mutex);
        return;
    }

    timestamp = get_time() - philo->start_time;

    if (message == NULL)
        printf("%ld %d died\n", timestamp, philo->t_id);
    else
        printf("%ld %d %s\n", timestamp, philo->t_id, message);

    pthread_mutex_unlock(&philo->table->print_mutex);
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
