/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:07:46 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/08 16:47:03 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>

typedef enum
{
	THINK,
	SLEEP,
	EAT,
	DEAD,
	FULL,
	FORK,
} action;

typedef struct s_data//malloc
{
	int				nbr_of_philo;
	pthread_t		*philo;//malloc
	pthread_mutex_t	*fork;//malloc
	pthread_mutex_t print;
	int				time_to_die;//temps max qui separe 2 repas
	int				time_to_eat;//duree du repas
	int				time_to_sleep;//duree de la sieste
	int				nbr_of_meals_min;
	struct timeval	*start_time;
	struct timeval	**last_meal;//malloc
	int				*meals;//nbr of succeed meals and malloced
	pthread_t		monitoring;
	pthread_mutex_t	is_dying;
	int				dead;
	int				all_full;
} t_data;

int	ft_atoi(const char *nptr);
int	calcul_diff(struct timeval *last_meal, struct timeval *current_time);
int	philo_eating(t_data *data, int i);
int	philo_sleeping(t_data *data, int i);
int	philo_thinking(t_data *data, int i);
void	*philo_routine(void *philo_data);
void	*monit_routine(void *info);

