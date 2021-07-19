#ifndef PHIL_H
# define PHIL_H

# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>

# define ENOARG "Incorrect number of arguments: must be 4 or 5"
# define EINVARG "Invalid arguments: there should be positive integers only"
# define EMALLOC "Cannot allocate memory"
# define EINITMUTEX "Cannot initialize mutex"
# define EDSTRMUTEX "Cannot destroy mutex"
# define ELMUTEX "Cannot lock mutex"
# define EULMUTEX "Cannot unlock mutex"
# define ETHREAD "Cannot create thread"
# define EJOIN "Cannot join thread"
# define EGETTIME "Cannot get time"
# define ESLEEP "Sleep error"
# define EWRITE "Cannot print standard output"

typedef struct s_params
{
	struct timeval	start;
	pthread_mutex_t	write;
	unsigned long	life_time;
	int				number_of_phils;
	int				eat_time;
	int				sleep_time;
	int				eat_limit;
	int				alive;
}	t_params;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				last_phil;
}	t_fork;

typedef struct s_phil
{
	t_fork			*forks[2];
	t_params		*params;
	struct timeval	last_meal;
	int				id;
	int				meals;
}	t_phil;

/*
 *	ERRORS
 */
int				error(const char *msg, int stat_loc);
int				errorphil(t_phil *phil, const char *msg, int stat_loc);
int				errorthreads(t_phil *phils, pthread_t *threads,
					const char *msg, int stat_loc);

/*
 *	UTILS
 */
unsigned long	gettimediff(struct timeval *time1, struct timeval *time2);
int				matoi(const char *str);
int				msleep(struct timeval *start, int msec);
int				itoa(char *buf, unsigned int n);
int				mprint(int msec, int id, const char *msg,
					pthread_mutex_t *mutex);

/*
 *	INITS
 */
void			freeforks(t_fork *forks, int len);
void			freephils(t_phil *phils);
int				initparams(t_params *params, int argc, char *argv[]);
int				initforks(t_fork **forks, int len);
int				initphils(t_phil **phils, t_params *params, t_fork *forks);

/*
 *	EVENTS
 */
int				event(t_phil *phil, char *msg, int msec);
int				takeforks(t_phil *phil);
int				putforks(t_phil *phil);
int				eat(t_phil *phil);
void			*startroutine(void *vphil);
#endif	/*PHIL_H*/
