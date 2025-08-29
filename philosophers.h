/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syanak <syanak@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 13:54:08 by syanak            #+#    #+#             */
/*   Updated: 2025/08/29 13:08:10 by syanak           ###   ########.fr       */
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
	int				start_flag;
	long long		start_time;
	long long		end_time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	start_mutex;
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

long long			get_time(void);

void				ft_usleep(long long time);
void				*monitor_routine(void *arg);
void				assign_all_forks(t_philo *first, t_data *data);
void				get_forks_order(t_philo *philo, pthread_mutex_t **first,
						pthread_mutex_t **second);
void				eat_action(t_philo *philo);
void				sleep_action(t_philo *philo);
void				think_action(t_philo *philo);
void				join_threads(t_philo *first);
void				*philosopher_routine(void *arg);
void	destroy_mutexes(t_data *data, t_philo *first);
void				free_philosophers(t_philo *first, int count);
void				cleanup_all(t_philo *first, t_data *data);
void				print_status(t_philo *philo, char *status);

t_philo				*create_philo(int id, t_data *data);
t_philo				*init_philosophers(t_data *data);
void				wait_for_all(t_philo *philo);
int					should_stop_simulation(t_philo *philo);
int					run_simulation(t_philo *first);
int					ft_atoi(const char *str);
int					handle_single_philosopher(t_philo *philo,
						pthread_mutex_t *first_fork);
int					check_args(int argc, char **argv);
int					check_philosopher_death(t_philo *philo);
int					check_all_ate_enough(t_philo *first);
int					start_threads(t_philo *first);

t_data				*init_data(int argc, char **argv);

#endif
