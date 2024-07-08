/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:36:34 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/08 16:28:15 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philosophers.h"

void	*monit_routine(void *info)
{
	int	i;
	t_data	*data;
	struct timeval *current_time;
	int	timestamp;

	i = 0;
	data = (t_data *) info;
	while (1)
	{
		while (i < data->nbr_of_philo)
		{
			gettimeofday(current_time, NULL);
			if (calcul_diff(data->last_meal[i], current_time) > data->time_to_die)
			{
				pthread_mutex_lock(&(data->is_dying));
				data->dead = i;
				pthread_mutex_unlock(&(data->is_dying));
				break;
			}
			i++;
		}
		while (--i >= 0)
		{
			if (data->meals[i] < data->nbr_of_meals_min)
				break;
			if (i == 0)
			{
				gettimeofday(current_time, NULL);
				data->all_full = 0;
				break;
			}
		}
		pthread_mutex_lock(&(data->is_dying));
		if (data->dead != -1 || data->all_full != -1)
		{
			timestamp = calcul_diff(data->start_time, current_time);
			pthread_mutex_unlock(&(data->is_dying));
			break;
		}
		pthread_mutex_unlock(&(data->is_dying));
	}
	pthread_mutex_lock(&(data->print));
	if (data->all_full != -1)
		printf("%d ms all philo ate at least %d meals",
			timestamp, data->nbr_of_meals_min);
	else
		printf("%d ms philo %d died", timestamp, i);
	clean_all(data);
}
