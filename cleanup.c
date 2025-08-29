/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:54:44 by syanak            #+#    #+#             */
/*   Updated: 2025/08/29 13:07:45 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	destroy_mutexes(t_data *data, t_philo *first)
{
	int	i;
	t_philo *current;

	current = first;
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->start_mutex);
	i = 0;
	while (i < data->num_philos)
	{
		if (current->right_fork)
			pthread_mutex_destroy(current->right_fork);
		current = current->next;
		i++;
	}
}

void	free_philosophers(t_philo *first, int count)
{
	t_philo	*current;
	t_philo	*next;
	int		i;

	current = first;
	i = 0;
	while (i < count)
	{
		next = current->next;
		free(current->right_fork);
		free(current);
		current = next;
		i++;
	}
}

void	cleanup_all(t_philo *first, t_data *data)
{
	int	num_philo;
	
	num_philo = data->num_philos;
	if (data)
	{
		destroy_mutexes(data, first);
		free(data);
	}
	if (first)
		free_philosophers(first, num_philo);
}

void	wait_for_all(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->start_mutex);
	while (!philo->data->start_flag)
	{
		pthread_mutex_unlock(&philo->data->start_mutex);
		usleep(10);
		pthread_mutex_lock(&philo->data->start_mutex);
	}
	pthread_mutex_unlock(&philo->data->start_mutex);
}
