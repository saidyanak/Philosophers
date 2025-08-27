/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:24:19 by syanak            #+#    #+#             */
/*   Updated: 2025/08/27 13:58:09 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_philosopher_death(t_philo *philo)
{
	long long	current_time;
	long long	time_since_meal;

	current_time = get_time();
	pthread_mutex_lock(&philo->data->meal_mutex);
	time_since_meal = current_time - philo->last_meal_time;
	if (time_since_meal >= philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->data->meal_mutex);
		pthread_mutex_lock(&philo->data->death_mutex);
		if (!philo->data->stop_simulation)
		{
			philo->data->stop_simulation = 1;
			printf("%lld %d died\n", current_time - philo->data->start_time,
				philo->id);
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->meal_mutex);
	return (0);
}

int	check_all_ate_enough(t_philo *first)
{
	t_philo	*current;
	int		i;
	int		ate_enough;

	if (first->data->must_eat_count == -1)
		return (0);
	current = first;
	i = 0;
	ate_enough = 1;
	while (i < first->data->num_philos)
	{
		pthread_mutex_lock(&first->data->meal_mutex);
		if (!(current->meals_eaten >= first->data->must_eat_count))
			ate_enough = 0;
		pthread_mutex_unlock(&first->data->meal_mutex);
		if (!ate_enough)
			break ;
		current = current->next;
		i++;
	}
	return (ate_enough);
}

int	check_all_philosophers(t_philo *first)
{
	t_philo	*current;
	int		i;

	current = first;
	i = 0;
	while (i < first->data->num_philos)
	{
		if (check_philosopher_death(current))
			return (1);
		current = current->next;
		i++;
	}
	return (0);
}

int	handle_simulation_end(t_philo *first)
{
	if (check_all_ate_enough(first))
	{
		pthread_mutex_lock(&first->data->death_mutex);
		first->data->stop_simulation = 1;
		pthread_mutex_unlock(&first->data->death_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	*first;

	first = (t_philo *)arg;
	while (1)
	{
		if (check_all_philosophers(first))
			return (NULL);
		if (handle_simulation_end(first))
			return (NULL);
		usleep(10);
	}
	return (NULL);
}
