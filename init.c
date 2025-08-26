/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:13:23 by syanak            #+#    #+#             */
/*   Updated: 2025/08/25 18:00:10 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_data	*init_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;
	data->stop_simulation = 0;
	data->all_ate_enough = 0;
	data->start_flag = 0;
	data->start_time = get_time();
	data->end_time = 0;
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);
	pthread_mutex_init(&data->meal_mutex, NULL);
	pthread_mutex_init(&data->start_mutex, NULL);
	return (data);
}

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->num_philos)
		pthread_mutex_init(&data->forks[i++], NULL);
	return (1);
}

t_philo	*create_philo(int id, t_data *data)
{
	t_philo	*new;

	new = malloc(sizeof(t_philo));
	if (!new)
		return (NULL);
	new->id = id;
	new->meals_eaten = 0;
	new->last_meal_time = data->start_time;
	new->next = NULL;
	new->prev = NULL;
	new->data = data;
	return (new);
}

t_philo	*init_philosophers(t_data *data)
{
	t_philo	*head;
	t_philo	*current;
	t_philo	*new;
	int		i;

	head = create_philo(1, data);
	if (!head)
		return (NULL);
	current = head;
	i = 2;
	while (i <= data->num_philos)
	{
		new = create_philo(i, data);
		if (!new)
			return (NULL);
		current->next = new;
		new->prev = current;
		current = new;
		i++;
	}
	current->next = head;
	head->prev = current;
	return (head);
}

void	assign_fork(t_philo *philo, t_data *data)
{
	philo->left_fork = &data->forks[philo->id - 1];
	if (philo->id == data->num_philos)
		philo->right_fork = &data->forks[0];
	else
		philo->right_fork = &data->forks[philo->id];
}
