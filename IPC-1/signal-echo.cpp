#include <iostream>
#include <csignal>
#include <unistd.h>
#include <pwd.h>
#include <ucontext.h>

void sigusr1_handler(int sig, siginfo_t *info, void *context) {
	if (!info || !context) {
		std::cerr << "Didn't get siginfo or context\n";
		return;
	}

	pid_t sender_pid = info->si_pid;
	uid_t sender_uid = info->si_uid;

	passwd *pw = getpwuid(sender_uid);
	const char *username = pw ? pw->pw_name : "unknown";
	ucontext_t *uc = (ucontext_t*) context;

#if defined(__x86_64__)
	unsigned long eip = uc->uc_mcontext.gregs[REG_RIP];
	unsigned long eax = uc->uc_mcontext.gregs[REG_RAX];
	unsigned long ebx = uc->uc_mcontext.gregs[REG_RBX];
#elif defined(__i386__)
	unsigned long eip = uc->uc_mcontext.gregs[REG_EIP];
	unsigned long eax = uc->uc_mcontext.gregs[REG_EAX];
	unsigned long ebx = uc->uc_mcontext.gregs[REG_EBX];
#else
	unsigned long eip = 0, eax = 0, ebx = 0;
#endif

	std::cout << "Received a SIGUSR1 signal from process " << sender_pid
		<< " executed by " << sender_uid << " (" << username << ")." << std::endl;
	std::cout << "State of the context: EIP = " << eip
		<< ", EAX = " << eax << ", EBX = " << ebx << "." << std::endl;
}

int main() {
	std::cout << "PID: " << getpid() << std::endl;

	struct sigaction sa = {};

	sa.sa_sigaction = sigusr1_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);

	if (sigaction(SIGUSR1, &sa, nullptr) != 0) {
		perror("sigaction");
		return 1;
	}

	while (true) {
		sleep(10);
	}

	return 0;
}

