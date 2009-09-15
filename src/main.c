#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <sys/user.h>

#include "itrace.h"

pid_t pid;

void peek_memory(uint8_t* buf, size_t cnt, off_t addr) {
    uint8_t *p;
    int i, j;
    union {
            long val;
            uint8_t chars[sizeof(long)];
    } data;

    p = buf;

    j = cnt / sizeof(long);
    for(i = 0; i < j; i++, p += sizeof(long)) {
        data.val = ptrace(PTRACE_PEEKDATA, pid, addr + i * sizeof(long), NULL);
        memcpy(p, data.chars, sizeof(long));
    }

    j = cnt % sizeof(long);
    if(j != 0) {
        data.val = ptrace(PTRACE_PEEKDATA, pid, addr + i * sizeof(long), NULL);
        memcpy(p, data.chars, j);
    }    
}



int main(int argc, char *argv[])
{
        int status;
	struct user_regs_struct regs;
	itrace_handle *trace;

	trace = itrace_init();
	itrace_set_peek_mem_func(trace, peek_memory);

        switch (pid = fork()) {
	        case -1:
	                perror("fork");
	                break;

	        case 0:
	                ptrace(PTRACE_TRACEME, 0, 0, 0);
	                execl("/bin/ls", "ls", NULL);
	                break;

	        default:
	                wait(&status); 
	                while(SIGTRAP == WSTOPSIG(status)) {
				ptrace(PTRACE_GETREGS, pid, NULL, &regs);

				itrace_trace(trace, regs.eip);	

	                        if (ptrace(PTRACE_SINGLESTEP, pid, 0, 0) != 0) perror("ptrace");
	                        wait(&status);
	               }
        }
        return EXIT_SUCCESS;
}

