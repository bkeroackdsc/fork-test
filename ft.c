/* Test forking speed
   Benjamen Keroack, Dollar Shave Club
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>


int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

int main() {

	pid_t c;
	struct timeval tv1;
	struct timeval tv2;
	struct timeval result;

	/* Allocate 2GB, zeroed to make sure the OS actually allocates it */
	printf("Allocating memory...\n");
	char *m = calloc(2147483648, 1);
	if (m == NULL) {
		printf("Could not allocate memory!\n");
		return 1;
	}

	printf("Forking...\n");
	gettimeofday(&tv1, NULL);
	c = fork();
	gettimeofday(&tv2, NULL);
	if (c == 0) {
		timeval_subtract(&result, &tv2, &tv1) ;
		printf("Fork took: %ld s %i us\n", result.tv_sec, result.tv_usec);
	}
	if (c == -1) {
		printf("Error forking\n");
	}

	free(m);
	return 0;
}