/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:10:37 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/09 15:57:09 by kbrener-         ###   ########.fr       */
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
		pthread_mutex_destroy(&(data->print));
		pthread_mutex_destroy(&(data->is_dying));
		if (data->philo)
			free(data->philo);
		memset(data, 0, sizeof(t_data));
		free(data);
		data = NULL;
	}
	return (-1);
}

void	init_philo(t_data *data, int i)
{
	data->philo[i]->philo_id = i;
	data->philo[i]->last_meal = data->start_time;
	data->philo[i]->meals = 0;
	pthread_create(&(data->philo[i]->philo_thread_id),
		NULL, philo_routine, (void *)data->philo[i]);
}
int	init_data(t_data *data, int argc, char **argv)
{
	data->nbr_of_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->nbr_of_meals_min = -1;
	if (argc == 6)
		data->nbr_of_meals_min = ft_atoi(argv[5]);
	data->philo = malloc(data->nbr_of_philo * sizeof(t_philo *));
	if (!data->philo)
		return (-1);
	memset(data->philo, 0, sizeof(t_philo *));
	data->fork = malloc(data->nbr_of_philo * sizeof(pthread_mutex_t));
	if (!data->fork)
		return (-1);
	memset(data->fork, 0, sizeof(pthread_mutex_t));
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
	gettimeofday(&(data->start_time), NULL);
	if (init_data(data, argc, argv) == -1)
		return (1);
	init_mutex(data);
	while (i < data->nbr_of_philo)
		init_philo(data, i++);
	i = 0;
	pthread_create(&(data->monitoring), NULL, monit_routine, (void *) data);
	while (i < data->nbr_of_philo)
		pthread_join(data->philo[i++]->philo_thread_id, NULL);
	pthread_join(data->monitoring, NULL);
	clean_all(data);
	return (0);
}
