#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct	deneme
{
};

void	*static_function(void *a)
{
	static int	i = 0;

	i++;
	printf("%sThred arttırdı %d\n", (char *)a, i);
}

int	main(void)
{
	pthread_t *thread;
	pthread_mutex_t mutex;
	int i = 0;
	char *ahmet = "ahmet";

	thread = malloc(100 * sizeof(pthread_t));
	pthread_mutex_init(&mutex, NULL);
	while (i < 1000)
	{
		pthread_mutex_lock(&mutex);
		pthread_create((&(thread[i++])), NULL, &static_function, ahmet);
		pthread_mutex_unlock(&mutex);
	}
	i = 0;
	printf("Main \n");
}