/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzabal-m <mzabal-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:54:34 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/16 10:36:18 by mzabal-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

/* ===== STRUCTURAS ===== */

typedef struct s_config
{
	int				num_filos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	int				someone_died;
	long			start_time;
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

/* ===== FUNCIONES ===== */

int		parse_int(char *s);
long	get_time_ms(void);
void	safe_print(t_config *cfg, int id, char *msg);
void	precise_sleep(int ms, t_config *cfg);
int		is_someone_dead(t_config *cfg);
int		init_config(t_config *cfg, int argc, char **argv, t_filo **f);
int		init_filosophers(t_config *cfg, t_filo *f);
int		start_simulation(t_config *cfg, t_filo *f,
			pthread_t *threads, pthread_t *charon);
void	free_all(t_config *cfg, t_filo *f,
			pthread_t *threads, pthread_t *charon);

void	*philo_routine(void *arg);
void	take_forks(t_filo *f);
void	drop_forks(t_filo *f);
void	eat(t_filo *f);

void	*thanatos(void *arg);
void	*maitre(void *arg);
int		count_full_philos(t_filo *f, t_config *cfg);
int		alloc_threads(t_config *cfg, pthread_t **threads, pthread_t **charon);
int		check_args(int ac, char **av);
#endif