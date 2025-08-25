/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:13:23 by syanak            #+#    #+#             */
/*   Updated: 2025/08/25 10:45:22 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "phliosophers.h"
#include <stdio.h>

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

int	main(int argc, char **argv)
{
	t_data *data;
	t_philo *philosophers;

	philosophers = NULL;
	if (!check_args(argc, argv))
	{
		printf("Error: Invalid arguments\n");
		printf("Usage: ./philo number_of_philosophers time_to_die ");
		printf("time_to_eat time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	data = init_data(data);
	if (!data || !init_forks(data))
		return (1);
	philosophers = init_philosophers(philosophers);
	if (!philosophers)
	{
		cleanup_all(NULL, data);
		return (1);
	}
	assign_all_forks(philosophers, data);
	run_simulation(philosophers, data);
	cleanup_all(philosophers, data);
}