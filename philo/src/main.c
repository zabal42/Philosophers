/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:58:31 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/15 17:02:53 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	alloc_threads(t_config *cfg, pthread_t **threads, pthread_t **charon)
{
	*threads = malloc(sizeof(pthread_t) * cfg->num_filos);
	*charon = malloc(sizeof(pthread_t) * cfg->num_filos);
	if (!*threads || !*charon)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_filo		*f;
	pthread_t	*threads = NULL;
	pthread_t	*charon = NULL;
	pthread_t	maitre_thread;

	if (init_config(&cfg, argc, argv, &f)
		|| alloc_threads(&cfg, &threads, &charon)
		|| init_filosophers(&cfg, f))
		return (free_all(&cfg, f, threads, charon), 1);
	if (cfg.meals_required > 0)
		pthread_create(&maitre_thread, NULL, maitre, f);
	if (start_simulation(&cfg, f, threads, charon))
		return (free_all(&cfg, f, threads, charon), 1);
	if (cfg.meals_required > 0)
		pthread_join(maitre_thread, NULL);
	free_all(&cfg, f, threads, charon);
	return (0);
}
