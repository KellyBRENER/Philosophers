/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:10:37 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/09 11:51:43 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	clean_mutex(pthread_mutex_t *mutex)
{
	while (mutex)
	{
		mutex++;
		pthread_mutex_destroy(mutex - 1);
	}
}

int	clean_all(t_data *data)
{
	if (data)
	{
		if (data->fork)
		{
			clean_mutex(data->fork);
			free(data->fork);
		}
		if (data->philo)
			free(data->philo);
		pthread_mutex_destroy(&(data->print));
		pthread_mutex_destroy(&(data->is_dying));
		if (data->last_meal)
			free(data->last_meal);
		if (data->meals)
			free(data->meals);
		memset(data, 0, sizeof(data));
		free(data);
		data = NULL;
	}
	return (-1);
}

int	init_data(t_data *data, int argc, char **argv)
{
	data->nbr_of_philo = ft_atoi(argv[2]);
	data->time_to_die = ft_atoi(argv[3]);
	data->time_to_eat = ft_atoi(argv[4]);
	data->time_to_sleep = ft_atoi(argv[5]);
	data->nbr_of_meals_min = -1;
	if (argc == 6)
		data->nbr_of_meals_min = ft_atoi(argv[6]);
	data->philo = malloc(data->nbr_of_philo * sizeof(pthread_t));
	if (!data->philo)
		return (clean_all(data));
	memset(data->philo, 0, sizeof(data->philo));
	data->fork = malloc(data->nbr_of_philo * sizeof(pthread_mutex_t));
	if (!data->fork)
		return (clean_all(data));
	memset(data->fork, 0, sizeof(data->fork));
	data->last_meal = malloc(data->nbr_of_philo * sizeof(struct timeval *));
	if (!data->last_meal)
		return (clean_all(data));
	data->meals = malloc(data->nbr_of_philo * sizeof(int));
	if (!data->meals)
		return (clean_all(data));
	memset(data->meals, 0, sizeof(data->meals));
	data->dead = -1;
	data->all_full = -1;
	return (0);
}

void	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&(data->print), NULL);
	pthread_mutex_init(&(data->is_dying), NULL);
	while (i < data->nbr_of_philo)
		pthread_mutex_init(&(data->fork[i++]), NULL);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;

	i = 0;
	if (argc < 5 || argc > 6)
		return (1);
	data = malloc(sizeof(t_data));
	memset(data, 0, sizeof(t_data));
	gettimeofday(data->start_time, NULL);
	if (init_data(data, argc, argv) == -1)
		return (1);
	init_mutex(data);
	while (i < data->nbr_of_philo)
	{
		data->last_meal[i++] = data->start_time;
		pthread_create(&data->philo[i++], NULL, philo_routine, (void *)data);
	}
	i = 0;
	pthread_create(&(data->monitoring), NULL, monit_routine, (void *) data);
	while (i < data->nbr_of_philo)
		pthread_join(data->philo[i++], NULL);
	pthread_join(data->monitoring, NULL);
	clean_all(data);
	return (0);
}
