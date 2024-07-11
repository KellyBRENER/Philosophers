/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:10:37 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/11 16:18:39 by kbrener-         ###   ########.fr       */
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
		if (data->meals)
			free(data->meals);
		if (data->philo)
			free(data->philo);
		if (data->fork)
		{
			clean_mutex(data->fork);
			free(data->fork);
		}
		pthread_mutex_destroy(&(data->mut_id_philo));
		pthread_mutex_destroy(&(data->print));
		pthread_mutex_destroy(&(data->is_dying));
		if (data->last_meal)
			free(data->last_meal);
		memset(data, 0, sizeof(t_data));
		free(data);
		data = NULL;
	}
	return (-1);
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
	if (init_data(data, argc, argv) == -1)
		return (1);
	while (i < data->nbr_of_philo)
	{
		data->last_meal[i] = data->start_time;
		pthread_create(&data->philo[i++], NULL, philo_routine, (void *)data);
	}
	i = 0;
	pthread_create(&(data->monitoring), NULL, monit_routine, (void *) data);
	pthread_join(data->monitoring, NULL);
	while (i < data->nbr_of_philo)
		pthread_join(data->philo[i++], NULL);
	clean_all(data);
	return (0);
}
