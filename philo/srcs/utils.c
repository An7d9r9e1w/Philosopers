#include <phil.h>

int	matoi(const char *str)
{
	int	n;

	n = 0;
	while (*str >= '0' && *str <= '9')
		n = n * 10 + *str++ - '0';
	if (*str && (*str < '0' || *str > '9'))
		return (-1);
	return (n);
}

int	msleep(struct timeval *start, int msec)
{
	struct timeval	cur;

	msec *= 1000;
	while (1)
	{
		if (usleep(50) == -1 || gettimeofday(&cur, 0) == -1)
			return (-1);
		if ((cur.tv_sec - start->tv_sec) * 1000000
			- start->tv_usec + cur.tv_usec > msec)
			break ;
	}
	return (0);
}

unsigned long	gettimediff(struct timeval *time1, struct timeval *time2)
{
	return ((unsigned int)((time1->tv_sec - time2->tv_sec) * 1000
		+ (time1->tv_usec - time2->tv_usec) * 0.001));
}

int	itoa(char *buf, unsigned int n)
{
	unsigned int	tmp;
	int				len;

	tmp = n / 10;
	len = 1;
	while (tmp)
	{
		tmp /= 10;
		len++;
	}
	buf += len - 1;
	*buf = '0';
	while (n)
	{
		*buf-- = n % 10 + '0';
		n /= 10;
	}
	return (len);
}

int	mprint(int msec, int id, const char *msg, pthread_mutex_t *mutex)
{
	char	buf[256];
	char	*p;
	int		stat_loc;

	p = buf;
	p += itoa(p, msec);
	*p++ = ' ';
	p += itoa(p, id);
	*p++ = ' ';
	while (*msg)
		*p++ = *msg++;
	*p++ = '\n';
	if (pthread_mutex_lock(mutex))
		return (-1);
	stat_loc = write(1, buf, p - buf);
	if (pthread_mutex_unlock(mutex))
		return (-1);
	return (stat_loc);
}
