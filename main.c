/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:28:52 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/06 18:29:15 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	init_mutexes(void)
{
	int	i;

	i = 0;
	while (i < NUM_FILOSOFOS)
	{
		pthread_mutex_init(&tenedores[i], NULL);
		i++;
	}
}

static void	init_philosophers(t_filosofo *filosofos)
{
	int	i;

	i = 0;
	while (i < NUM_FILOSOFOS)
	{
		filosofos[i].id = i + 1;
		filosofos[i].last_meal_time = get_timestamp();
		filosofos[i].tenedor_izq = &tenedores[i];
		filosofos[i].tenedor_der = &tenedores[(i + 1) % NUM_FILOSOFOS];
		pthread_create(&filosofos[i].thread, NULL,
			philosopher_routine, &filosofos[i]);
		i++;
	}
}

static void	cleanup(t_filosofo *filosofos)
{
	int	i;

	i = 0;
	while (i < NUM_FILOSOFOS)
		pthread_join(filosofos[i++].thread, NULL);
	i = 0;
	while (i < NUM_FILOSOFOS)
		pthread_mutex_destroy(&tenedores[i++]);
}

int	main(void)
{
	t_filosofo	filosofos[NUM_FILOSOFOS];
	pthread_t	reaper;

	init_mutexes();
	init_philosophers(filosofos);
	pthread_create(&reaper, NULL, parroco, filosofos);
	pthread_join(reaper, NULL);
	cleanup(filosofos);
	return (0);
}