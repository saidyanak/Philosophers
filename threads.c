/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 12:41:20 by syanak            #+#    #+#             */
/*   Updated: 2025/08/29 13:08:18 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	should_stop_simulation(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->data->death_mutex);
	stop = philo->data->stop_simulation;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (stop);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_for_all(philo);
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->meal_mutex);
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat);
	while (1)
	{
		if (should_stop_simulation(philo))
			break ;
		eat_action(philo);
		if (should_stop_simulation(philo))
			break ;
		sleep_action(philo);
		if (should_stop_simulation(philo))
			break ;
		think_action(philo);
	}
	return (NULL);
}

int	run_simulation(t_philo *first)
{
	pthread_t	monitor;

	if (!start_threads(first))
		return (0);
	if (pthread_create(&monitor, NULL, monitor_routine, first))
		return (0);
	pthread_mutex_lock(&first->data->start_mutex);
	first->data->start_flag = 1;
	pthread_mutex_unlock(&first->data->start_mutex);
	pthread_join(monitor, NULL);
	join_threads(first);
	return (1);
}

int	start_threads(t_philo *first)
{
	t_philo	*current;
	int		i;

	current = first;
	i = 0;
	while (i < first->data->num_philos)
	{
		if (pthread_create(&current->thread, NULL, philosopher_routine,
				current))
			return (0);
		current = current->next;
		i++;
	}
	return (1);
}

void	join_threads(t_philo *first)
{
	t_philo	*current;
	int		i;

	current = first;
	i = 0;
	while (i < first->data->num_philos)
	{
		pthread_join(current->thread, NULL);
		current = current->next;
		i++;
	}
}
