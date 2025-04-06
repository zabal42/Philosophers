/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prueba_deadlock.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:47:37 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/06 12:29:05 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>


#define NUM_FILOSOFOS 5
#define NUM_REPETICIONES 1
#define TIME_TO_DIE 1000

volatile int	someone_died = 0;
pthread_mutex_t	tenedores[NUM_FILOSOFOS];

typedef	struct s_filosofo
{
	int				id;
	pthread_mutex_t	*tenedor_izq;
	pthread_mutex_t	*tenedor_der;
	long			last_meal_time;
} t_filosofo;

void	esperar(const char *accion, int id, int tiempo)
{
	printf("el filosofo %d esta %s\n", id, accion);
	usleep(tiempo);
}

long	get_timestamp(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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
	filo->last_meal_time = get_timestamp();
	esperar("Comiendo🍜", filo->id, 500000);
	pthread_mutex_unlock(filo->tenedor_izq);
	pthread_mutex_unlock(filo->tenedor_der);

	printf("El filosofo %d ha terminado de comer😎\n", filo->id);
	return (NULL);
}
void *parroco(void *arg)
{
	t_filosofo	*filos;
	int			i;
	filos = (t_filosofo *)arg;
	
	while (1)
	{
		i = 0;
		while (i < NUM_FILOSOFOS)
		{
			if ((get_timestamp() - filos[i].last_meal_time) > TIME_TO_DIE)
			{
				printf("💀 Filósofo %d ha muerto\n", filos[i].id);
				someone_died = 1;
				return (NULL);
			}
			i++;
		}
		usleep(1000);
	}
}

int	main(void)
{
	pthread_t	hilos[NUM_FILOSOFOS];
	pthread_t	rip;
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
		filosofos[i].last_meal_time = get_timestamp();
		filosofos[i].tenedor_izq = &tenedores[i];
		filosofos[i].tenedor_der = &tenedores[(i + 1) % NUM_FILOSOFOS];
		pthread_create(&hilos[i], NULL, cosas_de_filosofos, &filosofos[i]);
		i++;
	}
	pthread_create(&rip, NULL, parroco, filosofos);
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
	pthread_join(rip, NULL);
	return (0);
}

