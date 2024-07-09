/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:08:30 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/09 15:39:35 by kbrener-         ###   ########.fr       */
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
	usleep(data->time_to_eat);
	data->philo[i]->meals++;
	if (philo_dead(data) == -1)
		return (-1);
	return (0);
}

int	philo_think_eat(t_data *data, int i)
{
	int	left;
	int	right;

	left = i;
	if (i < data->nbr_of_philo - 1)
		right = i + 1;
	else
		right = 0;
	if (philo_dead(data) == -1)
		return (-1);
	print_action(data, i, THINK);
	pthread_mutex_lock(&(data->fork[left]));
	if (philo_dead(data) == -1)
		return (-1);
	print_action(data, i, FORK);
	pthread_mutex_lock(&(data->fork[right]));
	if (philo_dead(data) == -1)
		return (-1);
	print_action(data, i, FORK);
	if (philo_eat(data, i) == -1)
		return (-1);
	pthread_mutex_unlock(&(data->fork[left]));
	pthread_mutex_unlock(&(data->fork[right]));
	return (0);
}

/*philo_routine() = fonction qui lance la routine : manger, dormir, penser*/
void	*philo_routine(void *philo_data)
{
	t_philo	*philo;
	int		i;

	i = philo->philo_id;
	philo = (t_data *)philo_data;
	while (philo_dead(philo->data) != -1)
	{
		if (philo_think_eat(philo->data, i) == -1)
			return (NULL);
		if (philo_dead(philo->data) == -1)
			return (NULL);
		if (philo_sleep(philo->data, i) == -1)
			return (NULL);
	}
	return (NULL);
}
