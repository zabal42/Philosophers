/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filo_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzabal-m <mzabal-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:00:27 by mikelzabal        #+#    #+#             */
/*   Updated: 2025/04/16 11:10:50 by mzabal-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_filo *f)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	first = f->left_fork;
	second = f->right_fork;
	if (f->cfg->num_filos == 1)
	{
		pthread_mutex_lock(f->left_fork);
		safe_print(f->cfg, f->id, "has taken a fork");
		precise_sleep(f->cfg->time_to_die + 10, f->cfg);
		pthread_mutex_unlock(f->left_fork);
		return ;
	}
	if (first > second)
	{
		first = f->right_fork;
		second = f->left_fork;
	}
	pthread_mutex_lock(first);
	safe_print(f->cfg, f->id, "has taken a fork");
	pthread_mutex_lock(second);
	safe_print(f->cfg, f->id, "has taken a fork");
}

void	drop_forks(t_filo *f)
{
	if (f->cfg->num_filos == 1)
		return ;
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
	t_filo	*f;

	f = (t_filo *)arg;
	if (f->id % 2 == 0)
		usleep(1000);
	while (!is_someone_dead(f->cfg))
	{
		take_forks(f);
		if (is_someone_dead(f->cfg))
		{
			drop_forks(f);
			break ;
		}
		eat(f);
		if (is_someone_dead(f->cfg))
		{
			drop_forks(f);
			break ;
		}
		drop_forks(f);
		safe_print(f->cfg, f->id, "is sleeping");
		precise_sleep(f->cfg->time_to_sleep, f->cfg);
		safe_print(f->cfg, f->id, "is thinking");
	}
	return (NULL);
}
