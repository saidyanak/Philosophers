/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:13:23 by syanak            #+#    #+#             */
/*   Updated: 2025/08/25 18:00:14 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	destroy_mutexes(t_data *data)
{
	int	i;

	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->start_mutex);
	if (data->forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
}

void	free_philosophers(t_philo *first, int count)
{
	t_philo	*current;
	t_philo	*next;
	int		i;

	if (!first)
		return ;
	current = first;
	i = 0;
	while (i < count)
	{
		next = current->next;
		free(current);
		current = next;
		i++;
	}
}

void	cleanup_all(t_philo *first, t_data *data)
{
	if (first)
		free_philosophers(first, data->num_philos);
	if (data)
	{
		destroy_mutexes(data);
		free(data);
	}
}

void	assign_all_forks(t_philo *first, t_data *data)
{
	int		i;
	t_philo	*current;

	i = 0;
	current = first;
	while (i < data->num_philos)
	{
		assign_fork(current, data);
		current = current->next;
		i++;
	}
}
