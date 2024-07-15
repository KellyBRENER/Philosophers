/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:10:37 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/15 15:06:54 by kbrener-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	clean_mutex(pthread_mutex_t *fork, int nb_mutex)
{
	int	i;

	i = 0;
	while (fork && i < nb_mutex)
	{
		pthread_mutex_destroy(fork);
		fork++;
		i++;
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
			clean_mutex(data->fork, data->nbr_of_philo);
			free(data->fork);
		}
		pthread_mutex_destroy(&(data->mut_id_philo));
		pthread_mutex_destroy(&(data->print));
		pthread_mutex_destroy(&(data->is_dying));
		pthread_mutex_destroy(&(data->check_meals));
		if (data->last_meal)
			free(data->last_meal);
		memset(data, 0, sizeof(t_data));
		free(data);
		data = NULL;
	}
	return (-1);
}

void	*one_philo(void *philo_data)
{
	t_data	*data;

	data = (t_data *)philo_data;
	print_action(data, 0, THINK);
	pthread_mutex_lock(&(data->fork[0]));
	print_action(data, 0, FORK);
	usleep(data->time_to_die * 1000);
	print_action(data, 0, DEAD);
	pthread_mutex_unlock(&(data->fork[0]));
	return (NULL);
}

void	run_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philo)
	{
		data->last_meal[i] = data->start_time;
		pthread_create(&(data->philo[i]), NULL, philo_routine, (void *)data);
		i++;
	}
	i = 0;
	pthread_create(&(data->monitoring), NULL, monit_routine, (void *) data);
	pthread_join(data->monitoring, NULL);
	while (i < data->nbr_of_philo)
		pthread_join(data->philo[i++], NULL);
	clean_all(data);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc < 5 || argc > 6)
		return (printf("five to six arguments necessary\n"), 1);
	if (check_arg(argv) == -1)
		return (printf("invalid argument\n"), 1);
	data = malloc(sizeof(t_data));
	memset(data, 0, sizeof(t_data));
	if (init_data(data, argc, argv) == -1)
		return (1);
	if (data->nbr_of_philo == 1)
	{
		pthread_create(&(data->philo[0]), NULL, one_philo, (void *)data);
		pthread_join(data->philo[0], NULL);
		clean_all(data);
	}
	else
		run_philo(data);
	return (0);
}
