#include <phil.h>

void	event(t_phil *phil, char *msg, int msec)
{
	struct timeval	cur;

	if (phil->meals != phil->params->eat_limit)
	{
		if (gettimeofday(&cur, 0) == -1)
			error(EGETTIME, 1);
		if (gettimediff(&cur, &phil->last_meal) > phil->params->life_time)
		{
			mprint(gettimediff(&cur, &phil->params->start),
				phil->id, "died", phil->params->writing);
			exit(1);
		}
		if (mprint(gettimediff(&cur, &phil->params->start),
				phil->id, msg, phil->params->writing) == -1)
			error(EWRITE, 1);
		if (msec && msleep(&cur, msec) == -1)
			error(ESLEEP, 1);
	}
}

void	takeforks(t_phil *phil)
{
	if (sem_wait(phil->params->taking) == -1)
		error(EWAIT, 1);
	if (sem_wait(phil->params->forks) == -1)
		error(EWAIT, 1);
	event(phil, "has taken fork", 0);
	if (sem_wait(phil->params->forks) == -1)
		error(EWAIT, 1);
	event(phil, "has taken fork", 0);
	sem_post(phil->params->taking);
}

void	eat(t_phil *phil)
{
	if (gettimeofday(&phil->last_meal, 0) == -1)
		error(EGETTIME, 1);
	event(phil, "is eating", phil->params->eat_time);
	if (phil->meals != phil->params->eat_limit)
		phil->meals++;
}

void	startroutine(t_phil *phil)
{
	while (phil->meals != phil->params->eat_limit)
	{
		takeforks(phil);
		eat(phil);
		sem_post(phil->params->forks);
		sem_post(phil->params->forks);
		event(phil, "is sleeping", phil->params->sleep_time);
		event(phil, "is thinking", 0);
	}
	exit(0);
}
