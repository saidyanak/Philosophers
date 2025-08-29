/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 12:40:51 by syanak            #+#    #+#             */
/*   Updated: 2025/08/27 17:17:20 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	handle_single_philosopher(t_philo *philo, pthread_mutex_t *first_fork)
{
	if (philo->data->num_philos == 1)
	{
		while (1)
		{
			pthread_mutex_lock(&philo->data->death_mutex);
			if (philo->data->stop_simulation)
			{
				pthread_mutex_unlock(&philo->data->death_mutex);
				break ;
			}
			pthread_mutex_unlock(&philo->data->death_mutex);
			usleep(1000);
		}
		pthread_mutex_unlock(first_fork);
		return (1);
	}
	return (0);
}

void	add_fork(pthread_mutex_t **first_fork, pthread_mutex_t **second_fork,
		t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		*first_fork = philo->left_fork;
		*second_fork = philo->right_fork;
	}
	else
	{
		*first_fork = philo->right_fork;
		*second_fork = philo->left_fork;
	}
}

void	eat_action(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	add_fork(&first_fork, &second_fork, philo);
	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");
	if (handle_single_philosopher(philo, first_fork))
		return ;
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

void	sleep_action(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void	think_action(t_philo *philo)
{
	print_status(philo, "is thinking");
	if (philo->id % 2 == 1)
		ft_usleep(1);
}
