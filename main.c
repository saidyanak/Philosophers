/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:13:23 by syanak            #+#    #+#             */
/*   Updated: 2025/08/25 14:09:07 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "phliosophers.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] <= 13 && str[i] >= 9))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10;
		result += str[i] - '0';
		i++;
	}
	return (result * sign);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long time)
{
	long long	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(500);
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->death_mutex);
	if (!philo->data->stop_simulation)
	{
		printf("%lld %d %s\n", get_time() - philo->data->start_time, philo->id,
			status);
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

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
	data->start_time = get_time();
	data->end_time = 0;
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);
	pthread_mutex_init(&data->meal_mutex, NULL);
	return (data);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
		return (0);
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		if (ft_atoi(argv[i]) <= 0)
			return (0);
		i++;
	}
	return (1);
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

void	destroy_mutexes(t_data *data)
{
	int	i;

	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
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

void	assign_fork(t_philo *philo, t_data *data)
{
	philo->left_fork = &data->forks[philo->id - 1];
	if (philo->id == data->num_philos)
		philo->right_fork = &data->forks[0];
	else
		philo->right_fork = &data->forks[philo->id];
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

int	check_philosopher_death(t_philo *philo)
{
	long long	current_time;

	current_time = get_time();
	pthread_mutex_lock(&philo->data->meal_mutex);
	if (current_time - philo->last_meal_time > philo->data->time_to_die)
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
		if (current->meals_eaten < first->data->must_eat_count)
			ate_enough = 0;
		pthread_mutex_unlock(&first->data->meal_mutex);
		if (!ate_enough)
			break ;
		current = current->next;
		i++;
	}
	return (ate_enough);
}

void	*monitor_routine(void *arg)
{
	t_philo	*first;
	t_philo	*current;
	int		i;

	first = (t_philo *)arg;
	while (1)
	{
		current = first;
		i = 0;
		while (i < first->data->num_philos)
		{
			if (check_philosopher_death(current))
				return (NULL);
			current = current->next;
			i++;
		}
		if (check_all_ate_enough(first))
		{
			pthread_mutex_lock(&first->data->death_mutex);
			first->data->stop_simulation = 1;
			pthread_mutex_unlock(&first->data->death_mutex);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->stop_simulation)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		// Eat
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		if (philo->data->num_philos == 1)
		{
			// Tek filozof durumunda sonsuz bekle,
			// monitor thread death'i detect edecek while (1)
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
			pthread_mutex_unlock(philo->left_fork);
			break ;
		}
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		print_status(philo, "is eating");
		pthread_mutex_lock(&philo->data->meal_mutex);
		philo->last_meal_time = get_time();
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->data->meal_mutex);
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		// Sleep
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
		// Think
		print_status(philo, "is thinking");
	}
	return (NULL);
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

int	run_simulation(t_philo *first, t_data *data)
{
	pthread_t	monitor;

	(void)data;
	if (!start_threads(first))
		return (0);
	// Monitor thread'i her durumda başlat (tek filozof dahil)
	if (pthread_create(&monitor, NULL, monitor_routine, first))
		return (0);
	pthread_join(monitor, NULL);
	join_threads(first);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data *data;
	t_philo *philosophers;

	if (!check_args(argc, argv))
	{
		printf("Error: Invalid arguments\n");
		printf("Usage: ./philo number_of_philosophers time_to_die ");
		printf("time_to_eat time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	data = init_data(argc, argv);
	if (!data || !init_forks(data))
		return (1);
	philosophers = init_philosophers(data);
	if (!philosophers)
	{
		cleanup_all(NULL, data);
		return (1);
	}
	assign_all_forks(philosophers, data);
	run_simulation(philosophers, data);
	cleanup_all(philosophers, data);
	return (0);
}