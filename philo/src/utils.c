/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikelzabal <mikelzabal@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:05:04 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/15 14:05:18 by mikelzabal       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_int(char *s)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (s[i] >= '0' && s[i] <= '9')
		res = res * 10 + (s[i++] - '0');
	return (res);
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
	long	start;

	start = get_time_ms();
	while (!is_someone_dead(cfg) && (get_time_ms() - start < ms))
		usleep(500);
}
