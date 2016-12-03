#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

#include <sys/prctl.h>

extern int code ( void );

void handle_signal ( int sig ) 
{
	printf( "SIGCHLD is caught: %d\n", getpid() );
	while ( waitpid((pid_t)(-1), 0, WNOHANG) > 0 ) 
	{}

	// Process shutdown operations

	exit( 0 );
}

void handle_parent_died ( int sig )
{
	printf( "SIGHUP is caught: %d\n", getpid() );

	// No way to perform shutdown operations (it`s child)

	exit( 0 );
}

// #define TEST_FORK

int main ( void )
{

	pid_t	fork_pid = fork();
	if ( fork_pid < 0 )
	{
		printf("Failed to fork\n");
		return ECHILD;
	}

	if ( fork_pid == 0 )	// Child
	{
		// Functions to complete child if parent died
		signal( SIGHUP, handle_parent_died );
		prctl( PR_SET_PDEATHSIG, SIGHUP );

		// Wait for backward button is pressed
#ifdef TEST_FORK
		printf( "Child PID=%d\n", getpid() );
		for ( int i = 3; i > 0; i-- )
		{
			printf( "Child counts: %d\n", i );
			sleep( 1 );
		}
#endif
		return 0;
	}
	else					// Parent
	{
		signal( SIGCHLD, handle_signal );		// Called if child completed - button pressed
		signal( SIGSEGV, handle_signal );		// Called if program failed - various memory access errors (ex. out of index)
#ifdef TEST_FORK
		printf( "Parent PID=%d\n", getpid() );
		sleep(10);
#endif
		return code();
	}
}
