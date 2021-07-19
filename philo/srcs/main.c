#include <phil.h>

int	waitphils(pthread_t *threads, t_phil *phils)
{
	void	*result;
	int		i;

	i = -1;
	while (++i < phils->params->number_of_phils)
		if (pthread_join(threads[i], &result))
			return (errorthreads(phils, threads, EJOIN, -1));
	freephils(phils);
	free(threads);
	return (0);
}

int	initthreads(t_phil *phils, t_params *params)
{
	pthread_t	*threads;
	int			i;

	threads = malloc(sizeof(pthread_t) * params->number_of_phils);
	if (!threads)
		return (errorthreads(phils, 0, EMALLOC, -1));
	i = -1;
	if (gettimeofday(&params->start, 0) == -1)
		return (errorthreads(phils, threads, EGETTIME, -1));
	while (++i < params->number_of_phils)
	{
		phils[i].last_meal.tv_sec = params->start.tv_sec;
		phils[i].last_meal.tv_usec = params->start.tv_usec;
		if (pthread_create(threads + i, 0, startroutine, phils + i))
			return (errorthreads(phils, threads, ETHREAD, -1));
	}
	return (waitphils(threads, phils));
}

int	main(int argc, char *argv[])
{
	t_params	params;
	t_fork		*forks;
	t_phil		*phils;

	if (initparams(&params, argc - 1, argv + 1) == -1
		|| initforks(&forks, params.number_of_phils) == -1
		|| initphils(&phils, &params, forks) == -1
		|| initthreads(phils, &params) == -1)
		return (1);
	return (0);
}
