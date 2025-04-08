/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prueba_hilos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzabal-m <mzabal-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:07:34 by mzabal-m          #+#    #+#             */
/*   Updated: 2025/04/01 14:21:01 by mzabal-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_HILOS 10

void	*rutina_hilo(void *arg)
{
	int	id;
	int	i;

	id = *(int *)arg;
	i = 1;
	while (i <= 5)
	{
		printf("Hilo %d: %d\n", id, i);
		usleep(200000);
		i++;
	}
	return(NULL);
}

int	main(void)
{
	pthread_t	hilos[NUM_HILOS];
	int			ids[NUM_HILOS];
	int			i;

	i = 0;
	while(i < NUM_HILOS)
	{
		ids[i] = i + 1;
		pthread_create(&hilos[i], NULL, rutina_hilo, &ids[i]);
		printf("Principal: he creado el hilo %d\n", ids[i]);
		i++;
	}
	i = 0;
	while (i < NUM_HILOS)
	{
		pthread_join(hilos[i], NULL);
		i++;
	}
	
	printf("Principal: todos los hilos han terminado\n");
	return(0);
}