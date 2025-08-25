/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phliosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:13:47 by syanak            #+#    #+#             */
/*   Updated: 2025/08/25 13:56:39 by syanak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				stop_simulation;
	int				all_ate_enough;
	long long		start_time;
	long long		end_time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	*forks;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_philo	*next;
	struct s_philo	*prev;
	t_data			*data;
}					t_philo;

// Fonksiyon prototipleri
long long			get_time(void);
void				ft_usleep(long long time);
void				print_status(t_philo *philo, char *status);
int					check_philosopher_death(t_philo *philo);
int					check_all_ate_enough(t_philo *first);
void				*philosopher_routine(void *arg);
int					start_threads(t_philo *first);
void				join_threads(t_philo *first);

#endif