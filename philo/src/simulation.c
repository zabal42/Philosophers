/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:15:47 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/15 14:16:08 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_config *cfg, t_filo *f,
			pthread_t *threads, pthread_t *charon)
{
	int	i = 0;

	while (i < cfg->num_filos)
	{
		if (pthread_create(&threads[i], NULL, philo_routine, &f[i]) != 0)
			return (1);
		if (pthread_create(&charon[i], NULL, thanatos, &f[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < cfg->num_filos)
	{
		pthread_join(threads[i], NULL);
		pthread_join(charon[i], NULL);
		i++;
	}
	return (0);
}

void	free_all(t_config *cfg, t_filo *f,
		pthread_t *threads, pthread_t *charon)
{
	int	i = 0;

	while (i < cfg->num_filos)
	{
		pthread_mutex_destroy(&cfg->forks[i]);
		pthread_mutex_destroy(&f[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&cfg->print_mutex);
	pthread_mutex_destroy(&cfg->death_mutex);
	free(cfg->forks);
	free(f);
	free(threads);
	free(charon);
}
