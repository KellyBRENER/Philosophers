/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:36:34 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/12 14:13:30 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	time_to_die(t_data *data)
{
	int				i;
	struct timeval	current_time;
	int				time_stamp;

	i = 0;
	while (i < data->nbr_of_philo)
	{
		gettimeofday(&(current_time), NULL);
		pthread_mutex_lock(&(data->check_meals));
		time_stamp = calcul_diff(data->last_meal[i], current_time);
		pthread_mutex_unlock(&(data->check_meals));
		if (time_stamp > data->time_to_die)
		{
			pthread_mutex_lock(&(data->is_dying));
			data->dead = i;
			pthread_mutex_unlock(&(data->is_dying));
			return (0);
		}
		i++;
	}
	return (-1);
}

int	all_philo_full(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philo)
	{
		if (data->nbr_of_meals_min == -1)
			return (-1);
		pthread_mutex_lock(&(data->check_meals));
		if (data->meals[i++] < data->nbr_of_meals_min)
		{
			pthread_mutex_unlock(&(data->check_meals));
			return (-1);
		}
		pthread_mutex_unlock(&(data->check_meals));
	}
	pthread_mutex_lock(&(data->is_dying));
	data->all_full = 0;
	pthread_mutex_unlock(&(data->is_dying));
	return (0);
}

void	*monit_routine(void *info)
{
	t_data			*data;

	data = (t_data *) info;
	while (1)
	{
		if (time_to_die(data) == 0)
			break ;
		if (all_philo_full(data) == 0)
			break ;
	}
	if (data->all_full == 0)
		print_action(data, data->nbr_of_meals_min, FULL);
	else
		print_action(data, data->dead, DEAD);
	return (NULL);
}
