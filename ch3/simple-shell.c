/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 80		/* 80 chars per line, per command */

char *input = NULL ;	/* pointer to input line, kept for freeing mem*/

char *strim(char *s);
int get_input(char *args[]);

int main(void)
{
	static char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	
	int i, upper;
		
    while (1){   
        printf("osh>");
        fflush(stdout);
		
        int num = get_input(args);
        if(!num)
			continue;
			
        if(strcmp(args[0],"exit") == 0) {
			break;
		}
        
        pid_t pid = fork();
        
        if (pid < 0) { /* error occurred */
			fprintf(stderr, "Fork Failed\n");
			return 1;
		}
		else if (pid == 0) { /* child process */
			execvp(args[0],args);
		}
		else { /* parent process */
			if(*args[num-1] != '&')
				wait(NULL);
		}
    }
    
	free(input);
	return 0;
}

/**
 * Get input into the specified array, 
 * each element containing a word or token
 * 
 * @param char* args[] the array 
 * @return the number of tokens entered into the array
 **/
int get_input(char *args[]) {
	
	int n_spaces = 0;
	free(input);
	input = malloc(MAX_LINE * sizeof(char)) ;
	
	// get complete line
    if(fgets(input, MAX_LINE, stdin) == NULL) {
		fprintf(stderr, "Input failed\n");
		free(input);
		exit(-1);
	}
	
	size_t size = strlen(input);
	if(size == 0)
		return 0;
	
	// format the input string			
	input[size - 1] = '\0';				// use this since we know '\n' is included everytime
	input = strim(input);
        
    /* split string and append tokens to 'args' */
	char *p = strtok (input, " ");
	while (p) {
		args[n_spaces++] = p;
		p = strtok (NULL, " ");
	}
	args[n_spaces] = 0;  // append NULL at the end
	
	for(int i = 0;i < n_spaces;i++) {
		printf("%s ",args[i]);
	}

	return n_spaces;
}

char *strim(char *s)
{
	size_t size;
	char *end;

	size = strlen(s);
	if (!size)
		return s;

	end = s + size - 1;
	while (end >= s && isspace(*end))
		end--;
	*(end + 1) = '\0';

	return s;
}
