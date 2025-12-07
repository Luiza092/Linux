#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

bool is_prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; i++)
        if (x % i == 0) return false;
    return true;
}

int nth_prime(int n) {
    int count = 0, x = 2;
    while (true) {
        if (is_prime(x)) {
            count++;
            if (count == n) return x;
        }
        x++;
    }
}

ssize_t safe_write(int fd, const void* buf, size_t count) {
    ssize_t n = write(fd, buf, count);
    if (n == -1) {
     std::cerr << "Write error!" << std::endl;
        exit(1);
    }
    return n;
}

ssize_t safe_read(int fd, void* buf, size_t count) {
    ssize_t n = read(fd, buf, count);
    if (n == -1) {
     std::cerr << "Read error!" << std::endl;
        exit(1);
    }
    return n;
}

int main() {
    int p2c[2], c2p[2];

    if (pipe(p2c) == -1 || pipe(c2p) == -1) {
     std::cerr << "Pipe creation error!" << std::endl;
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
     std::cerr << "Fork error!" << std::endl;
        return 1;
    }

    if (pid == 0) {
        close(p2c[1]);
        close(c2p[0]);

        while (true) {
            int m;
            if (safe_read(p2c[0], &m, sizeof(m)) <= 0) break;
            if (m == -1) break;

     std::cout << "[Child] Calculating " << m << "-th prime number..." << std::endl;
            int result = nth_prime(m);
     std::cout << "[Child] Sending calculation result of prime(" << m << ")..." << std::endl;

            safe_write(c2p[1], &result, sizeof(result));
        }

        close(p2c[0]);
        close(c2p[1]);
        exit(0);
    }

    close(p2c[0]);
    close(c2p[1]);

    while (true) {
     std::string s;
 std::cout << "[Parent] Please enter the number: ";
 std::cin >> s;

        if (s == "exit") {
            int code = -1;
            safe_write(p2c[1], &code, sizeof(code));
            break;
        }

        int m = stoi(s);

 std::cout << "[Parent] Sending " << m << " to the child process..." << std::endl;
        safe_write(p2c[1], &m, sizeof(m));

 std::cout << "[Parent] Waiting for the response from the child process..." << std::endl;
        int result;
        safe_read(c2p[0], &result, sizeof(result));

 std::cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "..." << std::endl;
    }

    close(p2c[1]);
    close(c2p[0]);
    wait(nullptr);

    return 0;
}
