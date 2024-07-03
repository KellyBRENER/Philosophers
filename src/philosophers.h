/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbrener- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 14:07:46 by kbrener-          #+#    #+#             */
/*   Updated: 2024/07/03 13:16:16 by kbrener-         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_data
{
	pthread_mutex_t	*fork;
	pthread_t		*philo;
	int				nbr_of_philo;
	int				time_to_die;//temps max qui separe 2 repas
	int				time_to_eat;//duree du repas
	int				time_to_sleep;//duree de la sieste
	int				*meals;
	int				nbr_of_meals;
	int				dead;
	int				all_full;
} t_data;
