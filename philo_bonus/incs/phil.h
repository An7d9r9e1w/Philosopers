#ifndef PHIL_H
# define PHIL_H

# include <semaphore.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <stdlib.h>
# include <signal.h>

# define ENOARG "Incorrect number of arguments: must be 4 or 5"
# define EINVARG "Invalid arguments: there should be positive integers only"
# define EMALLOC "Cannot allocate memory"
# define EOPENSEM "Cannot open semaphore"
# define ELSEM "Cannot lock semaphore"
# define EULSEM "Cannot unlock semaphore"
# define EFORK "Cannot create process"
# define EWAIT "Cannot wait for child process"
# define EGETTIME "Cannot get time"
# define ESLEEP "Sleep error"
# define EWRITE "Cannot print standard output"

typedef struct s_params
{
	sem_t			*forks;
	sem_t			*taking;
	sem_t			*writing;
	struct timeval	start;
	unsigned long	life_time;
	int				number_of_phils;
	int				eat_time;
	int				sleep_time;
	int				eat_limit;
}	t_params;

typedef struct s_phil
{
	t_params		*params;
	struct timeval	last_meal;
	int				pid;
	int				id;
	int				meals;
}	t_phil;

/*
 *	FREE
 */
void			error(const char *msg, int stat_loc);
void			freesems(t_params *params);
void			freephils(t_phil *phils);
void			killphils(t_phil *phils);

/*
 *	UTILITIES
 */
int				matoi(const char *str);
int				msleep(struct timeval *start, int msec);
unsigned long	gettimediff(struct timeval *time1, struct timeval *time2);
int				itoa(char *buf, unsigned int n);
int				mprint(int msec, int id, const char *msg, sem_t *sem);

/*
 *	INITIALIZATIONS
 */
void			initsems(t_params *params);
void			initparams(t_params *params, int argc, char *argv[]);
void			initphils(t_phil **phils, t_params *params);

/*
 *	EVENTS
 */
void			event(t_phil *phil, char *msg, int msec);
void			takeforks(t_phil *phil);
void			eat(t_phil *phil);
void			startroutine(t_phil *phil);

void			waitphils(t_phil *phils);
void			initprocesses(t_phil *phils, t_params *params);
#endif	/*PHIL_H*/
