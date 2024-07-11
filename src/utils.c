/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 11:11:10 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/11 15:50:02 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *nptr)
{
	int	nb;
	int	i;
	int	sign;

	i = 0;
	sign = 1;
	nb = 0;
	while (nptr[i] == ' ' || nptr[i] == '\f' || nptr[i] == '\n'
		|| nptr[i] == '\r' || nptr[i] == '\t'
		|| nptr[i] == '\v')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb = nb * 10 + nptr[i] - 48;
		i++;
	}
	return (nb * sign);
}

int	calcul_diff(struct timeval last_meal, struct timeval current_time)
{
	int	diff;

	diff = ((current_time.tv_sec - last_meal.tv_sec) * 1000)
		+ ((current_time.tv_usec - last_meal.tv_usec) / 1000);
	return (diff);
}

int	philo_dead(t_data *data)
{
	pthread_mutex_lock(&(data->is_dying));
	if (data->dead != -1)
	{
		pthread_mutex_unlock(&(data->is_dying));
		return (-1);
	}
	pthread_mutex_unlock(&(data->is_dying));
	return (0);
}

void	print_action(t_data *data, int i, int action)
{
	struct timeval	current_time;
	int				time_stamp;

	gettimeofday(&(current_time), NULL);
	time_stamp = calcul_diff(data->start_time, current_time);
	pthread_mutex_lock(&(data->print));
	if (action == THINK)
		printf("%d ms : %d is thinking", time_stamp, i);
	else if (action == FORK)
		printf("%d ms philo %d has taken a fork", time_stamp, i);
	else if (action == EAT)
		printf("%d ms philo %d is eating", time_stamp, i);
	else if (action == SLEEP)
		printf("%d ms philo %d is sleeping", time_stamp, i);
	else if (action == DEAD)
		printf("%d ms philo %d is dead", time_stamp, i);
	else if (action == FULL)
		printf("%d ms all philo ate at least %d meals", time_stamp, i);
	pthread_mutex_unlock(&(data->print));
}
