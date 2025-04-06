/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prueba_deadlock.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 19:47:37 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/06 21:36:41 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>


#define NUM_FILOSOFOS 5
#define TIME_TO_DIE 100

volatile int	someone_died = 0;
pthread_mutex_t	tenedores[NUM_FILOSOFOS];
long	start_time;

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


void	take_forks(t_filosofo *filo)
{
	if (someone_died)
		return;
	printf("El filosofo %d intenta coger el tenedor ☀️\n", filo->id);
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
}
void	drop_forks(t_filosofo *filo)
{	
	pthread_mutex_unlock(filo->tenedor_izq);
	pthread_mutex_unlock(filo->tenedor_der);
	printf("El filosofo %d quiere dejar el tenedor 🌙\n", filo->id);
}

void	eat(t_filosofo *filo)
{
	if (someone_died)
		return;
	filo->last_meal_time = get_timestamp();
	esperar("Comiendo🍜", filo->id, 500000);
	printf("El filosofo %d ha terminado de comer😎\n", filo->id);
}

void	sleep_philo(t_filosofo *filo)
{
	if (someone_died)
		return;
	esperar("roncando 😴😴", filo->id, 300000);
	printf("El filosofo %d ha terminado de dormir🛌\n", filo->id);
}
void	think(t_filosofo *filo)
{
	if (someone_died)
		return;
	esperar("pensando 🤔", filo->id, 100000);
	printf("El filosofo %d ha terminado de pensar💭\n", filo->id);
}

void	*parroco(void *arg)
{
	t_filosofo	*filos;
	int			i;
	long		tiempo_muerte;
	long		tiempo_real;
	long		tiempo_teorico;
	long		retraso;

	filos = (t_filosofo *)arg;

	while (1)
	{
		i = 0;
		while (i < NUM_FILOSOFOS)
		{
			tiempo_real = get_timestamp();
			if ((tiempo_real - filos[i].last_meal_time) > TIME_TO_DIE)
			{
				someone_died = 1;
				tiempo_muerte = tiempo_real;
				tiempo_teorico = filos[i].last_meal_time + TIME_TO_DIE;
				retraso = tiempo_muerte - tiempo_teorico;

				printf("💀 Filósofo %d ha muerto.\n", filos[i].id);
				printf("⏱ Tiempo teórico: %ld ms | Tiempo real: %ld ms | Retraso: %ld ms\n",
					tiempo_teorico - start_time,
					tiempo_muerte - start_time,
					retraso);
				return (NULL);
			}
			i++;
		}
		usleep(1000);  // evitar sobrecargar la CPU
	}
}

void *philosopher_routine(void *arg)
{
	t_filosofo *filo;
	filo = (t_filosofo *)arg;

	while (!someone_died)
	{
		take_forks(filo);
		eat(filo);
		drop_forks(filo);
		sleep_philo(filo);
		think(filo);
	}
	return (NULL);
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
	start_time = get_timestamp();
	i = 0;
	while(i < NUM_FILOSOFOS)
	{
		filosofos[i].id = i + 1;
		filosofos[i].last_meal_time = get_timestamp();
		filosofos[i].tenedor_izq = &tenedores[i];
		filosofos[i].tenedor_der = &tenedores[(i + 1) % NUM_FILOSOFOS];
		pthread_create(&hilos[i], NULL, philosopher_routine, &filosofos[i]);
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

