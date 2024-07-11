/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 15:45:39 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/11 16:36:20 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_malloc(t_data *data)
{
	data->meals = malloc(data->nbr_of_philo * sizeof(int));
	if (!data->meals)
		return (clean_all(data));
	memset(data->meals, 0, sizeof(int));
	data->philo = malloc(data->nbr_of_philo * sizeof(pthread_t));
	if (!data->philo)
		return (clean_all(data));
	memset(data->philo, 0, sizeof(pthread_t));
	data->fork = malloc(data->nbr_of_philo * sizeof(pthread_mutex_t));
	if (!data->fork)
		return (clean_all(data));
	memset(data->fork, 0, sizeof(pthread_mutex_t));
	data->last_meal = malloc(data->nbr_of_philo * sizeof(struct timeval));
	if (!data->last_meal)
		return (clean_all(data));
	return (0);
}

void	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&(data->mut_id_philo), NULL);
	while (i < data->nbr_of_philo)
		pthread_mutex_init(&(data->fork[i++]), NULL);
	pthread_mutex_init(&(data->print), NULL);
	pthread_mutex_init(&(data->is_dying), NULL);
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
	data->dead = -1;
	data->all_full = -1;
	data->id_philo = -1;
	if (ft_malloc(data) == -1)
		return (-1);
	init_mutex(data);
	gettimeofday(&(data->start_time), NULL);
	return (0);
}
