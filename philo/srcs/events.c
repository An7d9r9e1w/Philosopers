#include <phil.h>

int	event(t_phil *phil, char *msg, int msec)
{
	struct timeval	cur;

	if (phil->params->alive && phil->meals != phil->params->eat_limit)
	{
		if (gettimeofday(&cur, 0) == -1)
			return (errorphil(phil, EGETTIME, -1));
		if (gettimediff(&cur, &phil->last_meal) > phil->params->life_time)
		{
			phil->params->alive = 0;
			mprint(gettimediff(&cur, &phil->params->start),
				phil->id, "died", &phil->params->write);
			return (-1);
		}
		if (mprint(gettimediff(&cur, &phil->params->start),
				phil->id, msg, &phil->params->write) == -1)
			return (errorphil(phil, EWRITE, -1));
		if (msec && msleep(&cur, msec) == -1)
			return (errorphil(phil, ESLEEP, -1));
		return (0);
	}
	return (-1);
}

int	takeforks(t_phil *phil)
{
	int	i;

	i = 0;
	while (i < 2 && phil->params->alive)
	{
		if (phil->forks[i]->last_phil != phil->id)
		{
			if (pthread_mutex_lock(&phil->forks[i]->mutex))
				return (errorphil(phil, ELMUTEX, -1));
			if (event(phil, "has taken fork", 0) == -1)
				return (-1);
			phil->forks[i++]->last_phil = phil->id;
			if (phil->forks[0] == phil->forks[1])
			{
				if (usleep((phil->params->life_time + 1) * 1000) == -1)
					return (errorphil(phil, ESLEEP, -1));
				break ;
			}
		}
	}
	return (0);
}

int	putforks(t_phil *phil)
{
	if (pthread_mutex_unlock(&phil->forks[0]->mutex)
		|| pthread_mutex_unlock(&phil->forks[1]->mutex))
		return (errorphil(phil, EULMUTEX, -1));
	return (0);
}

int	eat(t_phil *phil)
{
	if (*phil->forks != phil->forks[1]
		&& gettimeofday(&phil->last_meal, 0) == -1)
		return (errorphil(phil, EGETTIME, -1));
	event(phil, "is eating", phil->params->eat_time);
	if (phil->meals != phil->params->eat_limit)
		phil->meals++;
	return (0);
}

void	*startroutine(void *vphil)
{
	t_phil		*phil;

	phil = (t_phil *)vphil;
	while (phil->params->alive && phil->meals != phil->params->eat_limit)
	{
		if (takeforks(phil) == -1
			|| eat(phil) == -1
			|| putforks(phil) == -1
			|| event(phil, "is sleeping", phil->params->sleep_time) == -1
			|| event(phil, "is thinking", 0) == -1)
			break ;
	}
	putforks(phil);
	return (0);
}
