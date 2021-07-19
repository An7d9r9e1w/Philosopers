#include <phil.h>
#include <stdio.h>

static void	foreveralone(int life_time)
{
	printf("0 1 has taken fork\n");
	if (usleep(life_time * 1000) == -1)
		exit(1);
	printf("%d 1 died\n", life_time);
	exit(0);
}

void	initsems(t_params *params)
{
	sem_unlink("/forks");
	sem_unlink("/taking");
	sem_unlink("/writing");
	params->forks = SEM_FAILED;
	params->taking = SEM_FAILED;
	params->writing = SEM_FAILED;
	params->forks = sem_open("/forks", O_CREAT, 0644, params->number_of_phils);
	params->taking = sem_open("/taking", O_CREAT, 0644, 1);
	params->writing = sem_open("/writing", O_CREAT, 0644, 1);
	if (params->forks == SEM_FAILED
		|| params->taking == SEM_FAILED
		|| params->writing == SEM_FAILED)
	{
		freesems(params);
		error(EOPENSEM, 1);
	}
}

void	initparams(t_params *params, int argc, char *argv[])
{
	if (argc < 4 || argc > 5)
		error(ENOARG, -1);
	params->number_of_phils = matoi(argv[0]);
	params->life_time = matoi(argv[1]);
	params->eat_time = matoi(argv[2]);
	params->sleep_time = matoi(argv[3]);
	if (params->number_of_phils < 1 || params->life_time < 1
		|| params->eat_time < 1 || params->sleep_time < 1)
		error(EINVARG, 1);
	if (argc == 5)
	{
		params->eat_limit = matoi(argv[4]);
		if (params->eat_limit < 0)
			error(EINVARG, 1);
	}
	else
		params->eat_limit = -1;
	if (params->number_of_phils == 1)
		foreveralone(params->life_time);
	initsems(params);
}

void	initphils(t_phil **phils, t_params *params)
{
	t_phil	*phil;
	int		i;

	*phils = malloc(sizeof(t_phil) * params->number_of_phils);
	if (!*phils)
	{
		freesems(params);
		error(EMALLOC, 1);
	}
	i = -1;
	phil = *phils;
	while (++i < params->number_of_phils)
	{
		phil->params = params;
		phil->id = i + 1;
		phil->pid = -1;
		phil->meals = 0;
		phil++;
	}
}
