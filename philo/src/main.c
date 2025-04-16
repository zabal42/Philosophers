/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzabal-m <mzabal-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 13:58:31 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/16 10:42:20 by mzabal-m         ###   ########.fr       */
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

static int	is_positive_number(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	check_args(int ac, char **av)
{
	long	value;
	int		i;

	if (ac != 5 && ac != 6)
	{
		printf("Error: number of arguments must be 4 or 5.\n");
		return (1);
	}
	i = 1;
	while (i < ac)
	{
		if (!is_positive_number(av[i]))
		{
			printf("Error: argument %d is not a valid positive number.\n", i);
			return (1);
		}
		value = strtol(av[i], NULL, 10);
		if (value <= 0 || value > 2147483647)
		{
			printf("Error: argument %d is out of valid range.\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_filo		*f;
	pthread_t	*threads;
	pthread_t	*charon;
	pthread_t	maitre_thread;

	threads = NULL;
	charon = NULL;
	if (check_args(argc, argv))
		return (1);
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
