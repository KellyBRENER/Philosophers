/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:08:30 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/02 15:51:20 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*philo_routine() = fonction qui lance la routine : manger, dormir, penser*/
void	*philo_routine(void *data)
{}
/*philo_eating(int time_to_eat) = fonction permettant a un philo de manger*/
/*philo_sleeping(int time_to_sleep) = fonction permettant a un philo de dormir*/
/*philo_thinking() = fonction permettant a un philo de penser*/
int	main(int argc, char **argv)
{
	t_data	*data;

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
	/*creation des fork qui sont des mutex*/
	data->fork = malloc(data->nbr_of_philo * sizeof(pthread_mutex_t));
	if (!data->fork)
		return (1);
	/*creation des philo qui sont des thread*/
	data->philo = malloc(data->nbr_of_philo * sizeof(pthread_t));
	if (!data->philo)
	{
		free(data->fork);
		free(data);
		return (1);
	}

}
