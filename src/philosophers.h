/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:46:55 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/09 11:54:02 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum e_type
{
	THINK,
	SLEEP,
	EAT,
	DEAD,
	FULL,
	FORK
}		t_type;

typedef struct s_data
{
	int				nbr_of_philo;
	pthread_t		*philo;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_of_meals_min;
	struct timeval	*start_time;
	struct timeval	**last_meal;
	int				*meals;
	pthread_t		monitoring;
	pthread_mutex_t	is_dying;
	int				dead;
	int				all_full;
}		t_data;

/*main.c : contain main and init and clean functions*/
void	clean_mutex(pthread_mutex_t *mutex);
int		clean_all(t_data *data);
int		init_data(t_data *data, int argc, char **argv);
void	init_mutex(t_data *data);
int		main(int argc, char **argv);

/*utils.c : all little functions needed in several files*/
int		ft_atoi(const char *nptr);
int		calcul_diff(struct timeval *last_meal, struct timeval *current_time);
int		philo_dead(t_data *data);
int		print_action(t_data *data, int i, int action);

/*philosophers.c : all function needed for the philosophers's routine*/
int		philo_sleep(t_data *data, int i);
int		philo_eat(t_data *data, int i);
int		philo_think_eat(t_data *data, int i);
void	*philo_routine(void *philo_data);

/*monitor.c : all functions needed for the monitoring's routine*/
int		time_to_die(t_data *data);
int		all_philo_full(t_data *data);
void	*monit_routine(void *info);
