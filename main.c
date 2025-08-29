/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:13:23 by syanak            #+#    #+#             */
/*   Updated: 2025/08/25 17:59:46 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_usage(void)
{
	printf("Error: Invalid arguments\n");
	printf("Usage: ./philo number_of_philosophers time_to_die ");
	printf("time_to_eat time_to_sleep ");
	printf("[number_of_times_each_philosopher_must_eat]\n");
}

int	main(int argc, char **argv)
{
	t_data	*data;
	t_philo	*philosophers;

	if (!check_args(argc, argv))
	{
		print_usage();
		return (1);
	}
	data = init_data(argc, argv);
	if (!data)
		return (1);
	philosophers = init_philosophers(data);
	if (!philosophers)
	{
		cleanup_all(NULL, data);
		return (1);
	}
	run_simulation(philosophers);
	cleanup_all(philosophers, data);
	return (0);
}
