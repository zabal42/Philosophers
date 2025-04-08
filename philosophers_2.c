/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzabal-m <mzabal-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:37:02 by mzabal-m          #+#    #+#             */
/*   Updated: 2025/04/08 12:57:20 by mzabal-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:00:00 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/07 10:00:00 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

/* =================== STRUCTURAS =================== */
typedef struct s_config
{
	int		num_filos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	long	start_time;
	volatile int someone_died;
	pthread_mutex_t *forks;
	pthread_mutex_t print_mutex;
	pthread_mutex_t death_mutex;
}	t_config;

typedef struct s_filo
{
	int			id;
	long		last_meal_time;
	t_config	*cfg;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	pthread_mutex_t meal_mutex;
}	t_filo;

/* =================== FUNCIONES DE TIEMPO =================== */

int	ft_strlen(char *str)
{
	int	n;
	n = 0;
	while (str[n])
		n++;
	return (n);
}
long	get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	safe_print(t_config *cfg, int id, char *msg)
{
	pthread_mutex_lock(&cfg->print_mutex);
	if (!cfg->someone_died)
	{
		write(1, msg, ft_strlen(msg));
		write(1, ": Philosopher ", 14);
		char c = '0' + id;
		write(1, &c, 1);
		write(1, "\n", 1);
	}
	pthread_mutex_unlock(&cfg->print_mutex);
}

void	precise_sleep(int ms, t_config *cfg)
{
	long start = get_time_ms();
	while (!cfg->someone_died && (get_time_ms() - start < ms))
		usleep(500);
}

/* =================== FUNCIONES DE FILOSOFO =================== */
void	take_forks(t_filo *f)
{
	if (f->id % 2 == 0)
		pthread_mutex_lock(f->left_fork);
	else
		pthread_mutex_lock(f->right_fork);
	
	if (f->id % 2 == 0)
		pthread_mutex_lock(f->right_fork);
	else
		pthread_mutex_lock(f->left_fork);
}

void	drop_forks(t_filo *f)
{
	pthread_mutex_unlock(f->left_fork);
	pthread_mutex_unlock(f->right_fork);
}

void	eat(t_filo *f)
{
	pthread_mutex_lock(&f->meal_mutex);
	f->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&f->meal_mutex);
	safe_print(f->cfg, f->id, "is eating");
	precise_sleep(f->cfg->time_to_eat, f->cfg);
}

void	*philo_routine(void *arg)
{
	t_filo *f = (t_filo *)arg;
	if (f->id % 2 == 0)
		usleep(1000);
	while (!f->cfg->someone_died)
	{
		take_forks(f);
		eat(f);
		drop_forks(f);
		safe_print(f->cfg, f->id, "is sleeping");
		precise_sleep(f->cfg->time_to_sleep, f->cfg);
		safe_print(f->cfg, f->id, "is thinking");
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_filo *f = (t_filo *)arg;
	while (!f->cfg->someone_died)
	{
		pthread_mutex_lock(&f->meal_mutex);
		if (get_time_ms() - f->last_meal_time > f->cfg->time_to_die)
		{
			pthread_mutex_unlock(&f->meal_mutex);
			f->cfg->someone_died = 1;
			write(1, "A philosopher has died.\n", 25);
			return (NULL);
		}
		pthread_mutex_unlock(&f->meal_mutex);
		usleep(1000);
	}
	return (NULL);
}

/* =================== VALIDACION Y MAIN =================== */
int	parse_int(char *s)
{
	int i = 0;
	int res = 0;
	while (s[i] >= '0' && s[i] <= '9')
		res = res * 10 + (s[i++] - '0');
	return (res);
}

int	main(int argc, char **argv)
{
	if (argc != 5)
		return (1);
	t_config cfg;
	cfg.num_filos = parse_int(argv[1]);
	cfg.time_to_die = parse_int(argv[2]);
	cfg.time_to_eat = parse_int(argv[3]);
	cfg.time_to_sleep = parse_int(argv[4]);
	cfg.someone_died = 0;
	cfg.start_time = get_time_ms();
	pthread_mutex_init(&cfg.print_mutex, NULL);
	pthread_mutex_init(&cfg.death_mutex, NULL);
	cfg.forks = malloc(sizeof(pthread_mutex_t) * cfg.num_filos);
	t_filo *f = malloc(sizeof(t_filo) * cfg.num_filos);
	pthread_t *threads = malloc(sizeof(pthread_t) * cfg.num_filos);
	pthread_t *monitors = malloc(sizeof(pthread_t) * cfg.num_filos);

	int i = 0;
	while (i < cfg.num_filos)
	{
		pthread_mutex_init(&cfg.forks[i], NULL);
		f[i].id = i + 1;
		f[i].cfg = &cfg;
		f[i].last_meal_time = get_time_ms();
		f[i].left_fork = &cfg.forks[i];
		f[i].right_fork = &cfg.forks[(i + 1) % cfg.num_filos];
		pthread_mutex_init(&f[i].meal_mutex, NULL);
		pthread_create(&threads[i], NULL, philo_routine, &f[i]);
		pthread_create(&monitors[i], NULL, monitor, &f[i]);
		i++;
	}
	i = 0;
	while (i < cfg.num_filos)
	{
		pthread_join(threads[i], NULL);
		pthread_join(monitors[i], NULL);
		i++;
	}
	free(cfg.forks);
	free(f);
	free(threads);
	free(monitors);
	return (0);
}
