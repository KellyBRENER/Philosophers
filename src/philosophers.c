/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:08:30 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/03 13:31:40 by kbrener-         ###   ########.fr       */
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
int	philo_eating(t_data *data, int i, int **time_before)
{
	int	left;//fork of the philo
	int	right;//fork of the neighboor
	int	time_after[2];

	left = i;
	if (i < data->nbr_of_philo - 1)
		right = i + 1;
	else
		right = 0;
	if ((data->fork[right]) == 0)//second fork don't exist
	{
		ft_printf("not enough fork");
		return (-1);
	}
	pthread_mutex_lock(&(data->fork[left]));
	pthread_mutex_lock(&(data->fork[right]));
	gettimeofday(&time_after[0], &time_after[1]);
	if (calcul_diff(*time_before, time_after) > data->time_to_die)
	{
		pthread_mutex_unlock(&(data->fork[left]));
		pthread_mutex_unlock(&(data->fork[right]));
		return (-1);
	}
	(data->meals[i])++;
	pthread_mutex_unlock(&(data->fork[left]));
	pthread_mutex_unlock(&(data->fork[right]));
	*time_before = time_after;
	return (0);
}

void	philo_sleeping(t_data *data, int i)
{
	if (data->time_to_sleep > data->time_to_die)
	{
		ft_printf("philo n°%d died because time to sleep longer than time to die", i);
		data->dead = i;
		return ;
	}
	usleep(data->time_to_sleep * 1000);
}

void	philo_thinking(t_data *data, int i)
{

}
/*philo_routine() = fonction qui lance la routine : manger, dormir, penser*/
void	*philo_routine(void *data)
{
	t_data *philo_data;
	int	i;
	struct timeval	time_before;

	i = 0;
	gettimeofday(&time_before, NULL);
	philo_data = (t_data *)data;
	while (philo_data->philo[i] != NULL)
		i++;
	i = i - 1;
	while(philo_data->meals[i] < philo_data->nbr_of_meals ||
		philo_data->nbr_of_meals == -1)
	{

		if (philo_data->dead != -1 || philo_eating(data, i, &time_before) == -1)
		{
			ft_printf("philo n°%d died", philo_data->dead);
			break;
		}
		else
		{
			philo_sleeping(philo_data, i);
			philo_thinking(philo_data, i);
		}
	}
	clean_all(philo_data);
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
/*recuperation du nbr de philo*/
	if (argc < 5 || argc > 6)
		return (1);
	data->nbr_of_meals = -1;
	if (argc == 6)
		data->nbr_of_meals = ft_atoi(argv[6]);
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
	while (i < data->nbr_of_philo)
	{
		pthread_create(&data->philo[i], NULL, philo_routine, (void *)data);
		i++;
	}
}
