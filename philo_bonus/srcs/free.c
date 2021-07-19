#include <phil.h>

void	error(const char *msg, int stat_loc)
{
	char	buf[256];
	char	*err;
	char	*p;

	err = "Error: ";
	p = buf;
	while (*err)
		*p++ = *err++;
	while (*msg)
		*p++ = *msg++;
	*p++ = '\n';
	write(2, buf, p - buf);
	exit(stat_loc);
}

void	freesems(t_params *params)
{
	if (params->forks != SEM_FAILED)
	{
		sem_close(params->forks);
		sem_unlink("/forks");
	}
	if (params->taking != SEM_FAILED)
	{
		sem_close(params->taking);
		sem_unlink("/taking");
	}
	if (params->writing != SEM_FAILED)
	{
		sem_close(params->writing);
		sem_unlink("/writing");
	}
}

void	freephils(t_phil *phils)
{
	freesems(phils->params);
	free(phils);
}

void	killphils(t_phil *phils)
{
	int	i;

	i = -1;
	while (++i < phils->params->number_of_phils)
		if (phils[i].pid != -1)
			kill(phils[i].pid, SIGTERM);
	freephils(phils);
}
