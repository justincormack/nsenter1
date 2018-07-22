#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

extern char **environ;

// Reassociate with the most important namespaces of pid 1

int main(int argc, char **argv) {
	char *shell = "/bin/sh";
	char *def[] = {shell, NULL};
	char *cmd = shell;
	char **args = def;
	int fdm = open("/proc/1/ns/mnt", O_RDONLY);
	int fdu = open("/proc/1/ns/uts", O_RDONLY);
	int fdn = open("/proc/1/ns/net", O_RDONLY);
	int fdi = open("/proc/1/ns/ipc", O_RDONLY);
	int froot = open("/proc/1/root", O_RDONLY);

	if (fdm == -1 || fdu == -1 || fdn == -1 || fdi == -1 || froot == -1) {
		fprintf(stderr, "Failed to open /proc/1 files, are you root?\n");
		exit(1);
	}

	if (setns(fdm, 0) == -1) {
		perror("setns:mnt");
		exit(1);
	}
	if (setns(fdu, 0) == -1) {
		perror("setns:uts");
		exit(1);
	}
	if (setns(fdn, 0) == -1) {
		perror("setns:net");
		exit(1);
	}
	if (setns(fdi, 0) == -1) {
		perror("setns:ipc");
		exit(1);
	}
	if (fchdir(froot) == -1) {
		perror("fchdir");
		exit(1);
	}
	if (chroot(".") == -1) {
		perror("chroot");
		exit(1);
	}
	if (argc > 1) {
		cmd = argv[1];
		args = argv + 1;
	}
	if (execve(cmd, args, environ) == -1) {
		perror("execve");
		exit(1);
	}
	exit(0);	
}
