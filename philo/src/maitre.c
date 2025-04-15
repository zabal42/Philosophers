/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maitre.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:04:08 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/15 14:04:27 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	count_full_philos(t_filo *f, t_config *cfg)
{
	int	i;
	int	full;

	full = 0;
	i = 0;
	while (i < cfg->num_filos)
	{
		pthread_mutex_lock(&f[i].meal_mutex);
		if (f[i].meals_eaten >= cfg->meals_required)
			full++;
		pthread_mutex_unlock(&f[i].meal_mutex);
		i++;
	}
	return (full);
}

void	*maitre(void *arg)
{
	t_filo		*f = (t_filo *)arg;
	t_config	*cfg = f[0].cfg;

	while (!is_someone_dead(cfg))
	{
		if (count_full_philos(f, cfg) == cfg->num_filos)
		{
			pthread_mutex_lock(&cfg->death_mutex);
			cfg->someone_died = 1;
			pthread_mutex_unlock(&cfg->death_mutex);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
