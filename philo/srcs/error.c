#include <phil.h>

int	error(const char *msg, int stat_loc)
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
	return (stat_loc);
}

int	errorphil(t_phil *phil, const char *msg, int stat_loc)
{
	phil->params->alive = 0;
	return (error(msg, stat_loc));
}

int	errorthreads(t_phil *phils, pthread_t *threads,
	const char *msg, int stat_loc)
{
	if (phils)
	{
		phils->params->alive = 0;
		freephils(phils);
	}
	if (threads)
		free(threads);
	return (error(msg, stat_loc));
}
