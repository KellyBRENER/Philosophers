/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:08:30 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/08 16:48:24 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philosophers.h"
/*finir de rentrer les prints et de modifier eating et thinking et monitor, gerer la mort des thread*/
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
void	dead_process(t_data *data, int i)
{

}
int	calcul_diff(struct timeval *last_meal, struct timeval *current_time)
{
	int	diff;

	diff = ((current_time->tv_sec - last_meal->tv_sec) * 1000) +
		((current_time->tv_usec - last_meal->tv_usec) / 1000);
	return (diff);
}
void	print_action(t_data *data,struct timeval *current_time, int i, int action)
{
	int	time_stamp;

	gettimeofday(current_time, NULL);
	time_stamp = calcul_diff(data->start_time, current_time);
	pthread_mutex_lock(&(data->print));
	if (action = THINK)
		printf("%d ms : %d is thinking", time_stamp, i);
	else if (action = FORK)
		printf("%d ms philo %d has taken a fork", time_stamp, i);
	else if (action = EAT)
		printf("%d ms philo %d is eating", time_stamp, i);
	else if (action == SLEEP)
	else if (action == DEAD)
	else if (action == FULL)
}
int	philo_thinking(t_data *data, int i)
{
	int	left;//fork of the philo
	int	right;//fork of the neighboor
	struct timeval *think;
	int time_stamp;

	left = i;
	if (i < data->nbr_of_philo - 1)
		right = i + 1;
	else
		right = 0;
	print_action(data, think, i, THINK);
	pthread_mutex_lock(&(data->fork[left]));
	print_action(data, think, i, FORK);
	pthread_mutex_lock(&(data->fork[right]));
	print_action(data, think, i, FORK);
	return (0);
}
int	philo_eating(t_data *data, int i)
{
	struct timeval	*begin_meal;
	int	time_stamp;
	int	time_since_last_meal;


	if (philo_thinking(data, i) == -1)
		return (-1);
	gettimeofday(begin_meal, NULL);
	time_stamp = calcul_diff(data->start_time, begin_meal);
	time_since_last_meal = calcul_diff(data->last_meal[i], begin_meal);
	if (time_since_last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&(data->is_dying));
		data->dead = i;
		pthread_mutex_unlock(&(data->fork[left]));
		pthread_mutex_unlock(&(data->fork[right]));
		return (-1);
	}
	time_stamp = calcul_diff(data->start_time, begin_meal);
	data->last_meal[i] = begin_meal;
	pthread_mutex_unlock(&(data->fork[left]));
	pthread_mutex_unlock(&(data->fork[right]));
	(data->meals[i])++;
	return (0);
}

int	philo_sleeping(t_data *data, int i)
{
	struct timeval *sleep;
	int time_stamp;

	gettimeofday(sleep, NULL);
	time_stamp = calcul_diff(data->start_time, sleep);
	if (calcul_diff(data->last_meal[i], sleep) > data->time_to_die)
	{
		pthread_mutex_lock(&(data->is_dying));
		data->dead = i;
		return (-1);
	}
	printf("%d ms : %d is sleeping", time_stamp, i);
	usleep(data->time_to_sleep * 1000);
	gettimeofday(sleep, NULL);
	if (calcul_diff(data->last_meal[i], sleep) > data->time_to_die)
	{
		pthread_mutex_lock(&(data->is_dying));
		data->dead = i;
		return (-1);
	}
	return (0);
}

/*philo_routine() = fonction qui lance la routine : manger, dormir, penser*/
void	*philo_routine(void *philo_data)
{
	t_data *data;
	int	i;

	i = 0;
	data = (t_data *)philo_data;
	while (data->philo[i] != NULL)
		i++;
	i = i - 1;
	while(data->dead == -1)
	{
		if (philo_eating(data, i) == -1)
			return;
		if (philo_sleeping(data, i) == -1)
			return;
		if (philo_thinking(data, i) == -1)
			return;
	}
}
