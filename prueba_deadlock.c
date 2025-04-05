/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prueba_deadlock.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:47:37 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/04 16:16:54 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_FILOSOFOS 5
#define NUM_REPETICIONES 1

pthread_mutex_t	tenedores[NUM_FILOSOFOS];

typedef	struct s_filosofo
{
	int		id;
	pthread_mutex_t	*tenedor_izq;
	pthread_mutex_t	*tenedor_der;
} t_filosofo;

void	esperar(const char *accion, int id, int tiempo)
{
	printf("el filosofo %d esta %s\n", id, accion);
	usleep(tiempo);
}

void	*cosas_de_filosofos(void *arg)
{
	t_filosofo	*filo;
	filo = (t_filosofo *)arg;
	
	printf("El filosofo %d quiere comer 😋\n", filo->id);
	if(filo->id % 2 == 0)
	{
		pthread_mutex_lock(filo->tenedor_izq);
		printf("Filósofo %d (par) ha cogido el tenedor izquierdo 🍴\n", filo->id);
		pthread_mutex_lock(filo->tenedor_der);
		printf("Filósofo %d (par) ha cogido el tenedor derecho 🍴\n", filo->id);
	}
	else
	{
		pthread_mutex_lock(filo->tenedor_der);
		printf("Filósofo %d (impar) ha cogido el tenedor derecho 🍴\n", filo->id);
		pthread_mutex_lock(filo->tenedor_izq);
		printf("Filósofo %d (impar) ha cogido el tenedor izquierdo 🍴\n", filo->id);
	}
	esperar("Comiendo🍜", filo->id, 500000);
	pthread_mutex_unlock(filo->tenedor_izq);
	pthread_mutex_unlock(filo->tenedor_der);

	printf("El filosofo %d ha terminado de comer😎\n", filo->id);
	return (NULL);
}

int	main(void)
{
	pthread_t	hilos[NUM_FILOSOFOS];
	t_filosofo	filosofos[NUM_FILOSOFOS];
	int			i;

	i = 0;
	while (i < NUM_FILOSOFOS)
	{
		pthread_mutex_init(&tenedores[i], NULL);
		i++;
	}
	
	i = 0;
	while(i < NUM_FILOSOFOS)
	{
		filosofos[i].id = i + 1;
		filosofos[i].tenedor_izq = &tenedores[i];
		filosofos[i].tenedor_der = &tenedores[(i + 1) % NUM_FILOSOFOS];
		pthread_create(&hilos[i], NULL, cosas_de_filosofos, &filosofos[i]);
		i++;
	}
	i = 0;
	while (i < NUM_FILOSOFOS)
	{
		pthread_join(hilos[i], NULL);
		i++;
	}
	i = 0;
	while (i < NUM_FILOSOFOS)
	{
		pthread_mutex_destroy(&tenedores[i]);
		i++;
	}
	printf("todos los filosofos han terminado sus movidas. 👏👏👏\n");
	return (0);
}

