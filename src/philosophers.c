/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:08:30 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/05 16:17:03 by kbrener-         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

# include "philosophers.h"

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
int	philo_eating(t_data *data, int i)
{
	int	left;//fork of the philo
	int	right;//fork of the neighboor
	struct timeval	*begin_meal;
	int	time_stamp;
	int	time_since_last_meal;

	left = i;
	if (i < data->nbr_of_philo - 1)
		right = i + 1;
	else
		right = 0;
	pthread_mutex_lock(&(data->fork[left]));
	gettimeofday(begin_meal, NULL);
	time_stamp = calcul_diff(data->start_time, begin_meal);
	printf("%d ms %d has taken a fork", time_stamp, i);
	pthread_mutex_lock(&(data->fork[right]));
	gettimeofday(begin_meal, NULL);
	time_stamp = calcul_diff(data->start_time, begin_meal);
	printf("%d ms %d has taken a fork", time_stamp, i);
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
	printf("at %d : philo n°%d is eating", time_stamp, i);
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

int	philo_thinking(t_data *data, int i)
{
	struct timeval *think;
	int time_stamp;

	gettimeofday(think, NULL);
	time_stamp = calcul_diff(data->start_time, think);
	if (calcul_diff(data->last_meal[i], think) > data->time_to_die)
	{
		pthread_mutex_lock(&(data->is_dying));
		data->dead = i;
		return (-1);
	}
	printf("%d ms : %d is thinking", time_stamp, i);
	return (0);
}
/*philo_routine() = fonction qui lance la routine : manger, dormir, penser*/
void	*philo_routine(void *philo_data)
{
	t_data *data;
	int	i;
	struct timeval	time_before;

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

void	*check_dead(void *info)
{
	int	i;
	t_data	*data;

	i = 0;
	data = (t_data *) info;
	while (1)
	{
		pthread_mutex_lock(&(data->is_dying));
		if (data->dead != -1)
			break;
		pthread_mutex_unlock(&(data->is_dying));
		usleep(1000);
	}
	while (i < data->nbr_of_philo)
	{
		pthread_detach(data->philo[i]);
		i++;
	}
	clean_all(data);
}
/*philo_eating(int time_to_eat) = fonction permettant a un philo de manger*/
/*philo_sleeping(int time_to_sleep) = fonction permettant a un philo de dormir*/
/*philo_thinking() = fonction permettant a un philo de penser*/
int	main(int argc, char **argv)
{
	t_data	*data;
	int	i;

	i = 0;
	data = malloc(sizeof(t_data));
	memset(data, 0, sizeof(t_data));
	if (argc < 5 || argc > 6)
		return (1);
	gettimeofday(data->stat_time, NULL);
	data->nbr_of_meals_min = -1;
	if (argc == 6)
		data->nbr_of_meals_min = ft_atoi(argv[6]);
/*recuperation du nbr de philo*/
	data->nbr_of_philo = ft_atoi(argv[2]);
	data->time_to_die = ft_atoi(argv[3]);
	data->time_to_eat = ft_atoi(argv[4]);
	data->time_to_sleep = ft_atoi(argv[5]);
	data->meals = malloc(data->nbr_of_philo * sizeof(int));
	if (!data->meals)
		return (clean_all(data));
	/*creation des fork qui sont des mutex*/
	data->fork = malloc(data->nbr_of_philo * sizeof(pthread_mutex_t));
	if (!data->fork)
		return (clean_all(data));
	memset(data->fork, 0, sizeof(data->fork));
	while (i < data->nbr_of_philo)
	{
		pthread_mutex_init(&(data->fork[i]), NULL);
		i++;
	}
	i = 0;
	/*creation des philo qui sont des thread*/
	data->philo = malloc(data->nbr_of_philo * sizeof(pthread_t));
	if (!data->philo)
		return (clean_all(data));
	memset(data->philo, 0, sizeof(data->philo));
	data->dead = -1;
	pthread_mutex_init(&(data->is_dying), NULL);
	data->last_meal = malloc(data->nbr_of_philo * sizeof(struct timeval *));
	if (!data->last_meal)
		return (clean_all(data));
	while (i < data->nbr_of_philo)
	{
		data->last_meal[i] = data->start_time;
		i++;
	}
	while (i < data->nbr_of_philo)
	{
		pthread_create(&data->philo[i], NULL, philo_routine, (void *)data);
		i++;
	}
	i = 0;
	pthread_create(&(data->monitoring), NULL, check_dead, (void *) data);
	while (i < data->nbr_of_philo)
	{
		pthread_join(data->philo[i], NULL);
		i++;
	}
	clean_all(data);
	return (0);
}
