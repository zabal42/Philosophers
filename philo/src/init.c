/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:59:23 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/15 16:53:26 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_config(t_config *cfg, int argc, char **argv, t_filo **f)
{
	if (argc != 5 && argc != 6)
		return (1);
	cfg->num_filos = parse_int(argv[1]);
	cfg->time_to_die = parse_int(argv[2]);
	cfg->time_to_eat = parse_int(argv[3]);
	cfg->time_to_sleep = parse_int(argv[4]);
	cfg->meals_required = -1;
	if (argc == 6)
		cfg->meals_required = parse_int(argv[5]);
	cfg->someone_died = 0;
	cfg->start_time = get_time_ms();
	pthread_mutex_init(&cfg->print_mutex, NULL);
	pthread_mutex_init(&cfg->death_mutex, NULL);
	cfg->forks = malloc(sizeof(pthread_mutex_t) * cfg->num_filos);
	*f = malloc(sizeof(t_filo) * cfg->num_filos);
	if (!cfg->forks || !*f)
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
