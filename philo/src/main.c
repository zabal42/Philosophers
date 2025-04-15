/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:58:31 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/15 13:58:47 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_filo		*f;
	pthread_t	*threads;
	pthread_t	*charon;
	pthread_t	maitre_thread;

	if (init_config(&cfg, argc, argv, &f, &threads, &charon))
		return (1);
	if (init_filosophers(&cfg, f))
	{
		free_all(&cfg, f, threads, charon);
		return (1);
	}
	if (cfg.meals_required > 0)
		pthread_create(&maitre_thread, NULL, maitre, f);
	if (start_simulation(&cfg, f, threads, charon))
	{
		free_all(&cfg, f, threads, charon);
		return (1);
	}
	if (cfg.meals_required > 0)
		pthread_join(maitre_thread, NULL);
	free_all(&cfg, f, threads, charon);
	return (0);
}
