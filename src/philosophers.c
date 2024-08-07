/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:08:30 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/15 13:55:14 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	philo_sleep(t_data *data, int i)
{
	if (philo_dead(data) == -1)
		return (-1);
	print_action(data, i, SLEEP);
	usleep(data->time_to_sleep * 1000);
	if (philo_dead(data) == -1)
		return (-1);
	return (0);
}

int	philo_eat(t_data *data, int i)
{
	if (philo_dead(data) == -1)
		return (-1);
	print_action(data, i, EAT);
	usleep(data->time_to_eat * 1000);
	pthread_mutex_lock(&(data->check_meals));
	data->meals[i]++;
	pthread_mutex_unlock(&(data->check_meals));
	if (philo_dead(data) == -1)
		return (-1);
	return (0);
}

int	philo_take_fork(t_data *data, int i, int first, int second)
{
	pthread_mutex_lock(&(data->fork[first]));
	if (philo_dead(data) == -1)
	{
		pthread_mutex_unlock(&(data->fork[first]));
		return (-1);
	}
	print_action(data, i, FORK);
	pthread_mutex_lock(&(data->fork[second]));
	if (philo_dead(data) == -1)
	{
		pthread_mutex_unlock(&(data->fork[first]));
		pthread_mutex_unlock(&(data->fork[second]));
		return (-1);
	}
	print_action(data, i, FORK);
	if (philo_eat(data, i) == -1)
	{
		pthread_mutex_unlock(&(data->fork[first]));
		pthread_mutex_unlock(&(data->fork[second]));
		return (-1);
	}
	pthread_mutex_unlock(&(data->fork[first]));
	pthread_mutex_unlock(&(data->fork[second]));
	return (0);
}

int	philo_think_eat(t_data *data, int i)
{
	int	first;
	int	second;

	if (i % 2 == 0)
	{
		first = i;
		second = i + 1;
	}
	else
	{
		first = i + 1;
		second = i;
	}
	if (i == data->nbr_of_philo - 1)
	{
		first = i;
		second = 0;
	}
	print_action(data, i, THINK);
	usleep(3000);
	if (philo_dead(data) == -1)
		return (-1);
	if (philo_take_fork(data, i, first, second) == -1)
		return (-1);
	return (0);
}

/*philo_routine() = fonction qui lance la routine : manger, dormir, penser*/
void	*philo_routine(void *philo_data)
{
	t_data	*data;
	int		i;

	i = 0;
	data = (t_data *)philo_data;
	pthread_mutex_lock(&(data->mut_id_philo));
	i = ++data->id_philo;
	pthread_mutex_unlock(&(data->mut_id_philo));
	while (1)
	{
		if (philo_dead(data) == -1)
			return (NULL);
		if (philo_think_eat(data, i) == -1)
			return (NULL);
		if (philo_dead(data) == -1)
			return (NULL);
		if (philo_sleep(data, i) == -1)
			return (NULL);
	}
	return (NULL);
}
