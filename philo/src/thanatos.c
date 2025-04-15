/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thanatos.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:02:10 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/15 14:03:17 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_someone_dead(t_config *cfg)
{
	int	result;

	pthread_mutex_lock(&cfg->death_mutex);
	result = cfg->someone_died;
	pthread_mutex_unlock(&cfg->death_mutex);
	return (result);
}

void	*thanatos(void *arg)
{
	t_filo	*f = (t_filo *)arg;

	while (1)
	{
		pthread_mutex_lock(&f->meal_mutex);
		if (get_time_ms() - f->last_meal_time > f->cfg->time_to_die)
		{
			pthread_mutex_unlock(&f->meal_mutex);
			pthread_mutex_lock(&f->cfg->death_mutex);
			if (!f->cfg->someone_died)
			{
				f->cfg->someone_died = 1;
				pthread_mutex_unlock(&f->cfg->death_mutex);
				safe_print(f->cfg, f->id, "died 💀");
				return (NULL);
			}
			pthread_mutex_unlock(&f->cfg->death_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&f->meal_mutex);
		usleep(1000);
	}
	return (NULL);
}
