/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:37:02 by mzabal-m          #+#    #+#             */
/*   Updated: 2025/04/15 13:33:40 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

/* =================== STRUCTURAS =================== */

typedef struct s_config
{
	int				num_filos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start_time;
	int				someone_died;
	int				meals_required;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
}	t_config;

typedef struct s_filo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	t_config		*cfg;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
}	t_filo;

/* =================== FUNCIONES AUXILIARES =================== */

int	is_someone_dead(t_config *cfg)
{
	int	result;

	pthread_mutex_lock(&cfg->death_mutex);
	result = cfg->someone_died;
	pthread_mutex_unlock(&cfg->death_mutex);
	return (result);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	safe_print(t_config *cfg, int id, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&cfg->print_mutex);
	timestamp = get_time_ms() - cfg->start_time;
	if (!is_someone_dead(cfg) || (msg && msg[0] == 'd'))
		printf("[%ld ms] Philosopher %d %s\n", timestamp, id, msg);
	pthread_mutex_unlock(&cfg->print_mutex);
}

void	precise_sleep(int ms, t_config *cfg)
{
	long	start = get_time_ms();

	while (!is_someone_dead(cfg) && (get_time_ms() - start < ms))
		usleep(500);
}

/* =================== FUNCIONES DE FILOSOFO =================== */

void	take_forks(t_filo *f)
{
	if (f->cfg->num_filos == 1)
	{
		pthread_mutex_lock(f->left_fork);
		precise_sleep(f->cfg->time_to_die + 10, f->cfg);
		pthread_mutex_unlock(f->left_fork);
		return ;
	}

	pthread_mutex_t	*first = f->left_fork;
	pthread_mutex_t	*second = f->right_fork;

	if (first > second)
	{
		first = f->right_fork;
		second = f->left_fork;
	}
	pthread_mutex_lock(first);
	pthread_mutex_lock(second);
}

void	drop_forks(t_filo *f)
{
	if (f->cfg->num_filos == 1)
	{
		return ;
	}
	pthread_mutex_unlock(f->left_fork);
	pthread_mutex_unlock(f->right_fork);
}

void	eat(t_filo *f)
{
	pthread_mutex_lock(&f->meal_mutex);
	f->last_meal_time = get_time_ms();
	f->meals_eaten++;
	pthread_mutex_unlock(&f->meal_mutex);
	safe_print(f->cfg, f->id, "is eating");
	precise_sleep(f->cfg->time_to_eat, f->cfg);
}

void	*philo_routine(void *arg)
{
	t_filo	*f = (t_filo *)arg;
	int		forks_taken;

	if (f->id % 2 == 0)
		usleep(1000);

	while (!is_someone_dead(f->cfg))
	{
		forks_taken = 0;
		take_forks(f);
		forks_taken = 1;

		if (is_someone_dead(f->cfg))
		{
			if (forks_taken)
				drop_forks(f);
			break;
		}

		eat(f);

		if (is_someone_dead(f->cfg))
		{
			drop_forks(f);
			break;
		}

		drop_forks(f);
		safe_print(f->cfg, f->id, "is sleeping");
		precise_sleep(f->cfg->time_to_sleep, f->cfg);
		safe_print(f->cfg, f->id, "is thinking");
	}
	return (NULL);
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
	t_filo		*f;
	t_config	*cfg;

	f = (t_filo *)arg;
	cfg = f[0].cfg;
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
/* =================== MAIN =================== */

int	parse_int(char *s)
{
	int	i = 0;
	int	res = 0;

	while (s[i] >= '0' && s[i] <= '9')
		res = res * 10 + (s[i++] - '0');
	return (res);
}

int	init_config(t_config *cfg, int argc, char **argv,
				t_filo **f, pthread_t **t1, pthread_t **t2)
{
	if (argc != 5 && argc != 6)
		return (1);
	cfg->num_filos = parse_int(argv[1]);
	cfg->time_to_die = parse_int(argv[2]);
	cfg->time_to_eat = parse_int(argv[3]);
	cfg->time_to_sleep = parse_int(argv[4]);
	cfg->someone_died = 0;
	if (argc == 6)
		cfg->meals_required = parse_int(argv[5]);
	else
		cfg->meals_required = -1;
	cfg->start_time = get_time_ms();
	pthread_mutex_init(&cfg->print_mutex, NULL);
	pthread_mutex_init(&cfg->death_mutex, NULL);
	cfg->forks = malloc(sizeof(pthread_mutex_t) * cfg->num_filos);
	*f = malloc(sizeof(t_filo) * cfg->num_filos);
	*t1 = malloc(sizeof(pthread_t) * cfg->num_filos);
	*t2 = malloc(sizeof(pthread_t) * cfg->num_filos);
	if (!cfg->forks || !*f || !*t1 || !*t2)
		return (1);
	return (0);
}
int	init_filosophers(t_config *cfg, t_filo *f)
{
	int	i;

	i = 0;
	while (i < cfg->num_filos)
	{
		pthread_mutex_init(&cfg->forks[i], NULL);
		f[i].id = i + 1;
		f[i].meals_eaten = 0;
		f[i].cfg = cfg;
		f[i].last_meal_time = get_time_ms();
		f[i].left_fork = &cfg->forks[i];
		f[i].right_fork = &cfg->forks[(i + 1) % cfg->num_filos];
		pthread_mutex_init(&f[i].meal_mutex, NULL);
		i++;
	}
	return (0);
}
int	start_simulation(t_config *cfg, t_filo *f,
					pthread_t *threads, pthread_t *charon)
{
	int	i;

	i = 0;
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
	int	i;

	i = 0;
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
