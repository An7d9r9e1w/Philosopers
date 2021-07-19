#include <phil.h>

void	waitphils(t_phil *phils)
{
	int	stat_loc;
	int	i;

	i = -1;
	stat_loc = 0;
	while (++i < phils->params->number_of_phils && !stat_loc)
	{
		if (waitpid(-1, &stat_loc, 0) == -1)
		{
			killphils(phils);
			error(EWAIT, 1);
		}
	}
	if (stat_loc)
		killphils(phils);
	else
		freephils(phils);
}

void	initprocesses(t_phil *phils, t_params *params)
{
	int	i;

	i = -1;
	if (gettimeofday(&params->start, 0) == -1)
	{
		freephils(phils);
		return (error(EGETTIME, 1));
	}
	while (++i < params->number_of_phils)
	{
		phils[i].last_meal.tv_sec = params->start.tv_sec;
		phils[i].last_meal.tv_usec = params->start.tv_usec;
		phils[i].pid = fork();
		if (phils[i].pid == -1)
		{
			killphils(phils);
			error(EFORK, 1);
		}
		if (!phils[i].pid)
			startroutine(phils + i);
	}
	waitphils(phils);
}

int	main(int argc, char *argv[])
{
	t_params	params;
	t_phil		*phils;

	initparams(&params, argc - 1, argv + 1);
	initphils(&phils, &params);
	initprocesses(phils, &params);
	return (0);
}
