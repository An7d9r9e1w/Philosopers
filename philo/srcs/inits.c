#include <phil.h>

void	freeforks(t_fork *forks, int len)
{
	while (len--)
	{
		pthread_mutex_unlock(&forks[len].mutex);
		if (pthread_mutex_destroy(&forks[len].mutex))
		{
			error(EDSTRMUTEX, -1);
			break ;
		}
	}
	free(forks);
}

void	freephils(t_phil *phils)
{
	freeforks(*phils->forks, phils->params->number_of_phils);
	pthread_mutex_unlock(&phils->params->write);
	pthread_mutex_destroy(&phils->params->write);
	free(phils);
}

int	initparams(t_params *params, int argc, char *argv[])
{
	if (pthread_mutex_init(&params->write, 0))
		return (error(EINITMUTEX, -1));
	if (argc < 4 || argc > 5)
		return (error(ENOARG, -1));
	params->number_of_phils = matoi(argv[0]);
	params->life_time = matoi(argv[1]);
	params->eat_time = matoi(argv[2]);
	params->sleep_time = matoi(argv[3]);
	if (params->number_of_phils < 1 || params->life_time < 1
		|| params->eat_time < 1 || params->sleep_time < 1)
		return (error(EINVARG, -1));
	if (argc == 5)
	{
		params->eat_limit = matoi(argv[4]);
		if (params->eat_limit < 0)
			return (error(EINVARG, -1));
	}
	else
		params->eat_limit = -1;
	params->alive = 1;
	return (0);
}

int	initforks(t_fork **forks, int len)
{
	int	i;

	*forks = malloc(sizeof(t_fork) * len);
	if (!*fork)
		return (error(EMALLOC, -1));
	i = -1;
	while (++i < len)
	{
		if (pthread_mutex_init(&(*forks)[i].mutex, 0))
		{
			freeforks(*forks, i + 1);
			return (error(EINITMUTEX, -1));
		}
	}
	return (0);
}

int	initphils(t_phil **phils, t_params *params, t_fork *forks)
{
	t_phil	*phil;
	int		i;

	*phils = malloc(sizeof(t_phil) * params->number_of_phils);
	if (!*phils)
	{
		freeforks(forks, params->number_of_phils);
		return (error(EMALLOC, -1));
	}
	i = -1;
	phil = *phils;
	while (++i < params->number_of_phils)
	{
		phil->forks[!(i & 1)] = forks + (i + 1) % params->number_of_phils;
		phil->forks[i & 1] = forks + i;
		phil->params = params;
		phil->id = i + 1;
		phil->meals = 0;
		phil++;
	}
	return (0);
}
