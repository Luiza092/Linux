#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <algorithm>
#include <cctype>

using namespace std;

vector<string> split(const string &line) {
	vector<string> parts;
	string word;

	for(char c : line) {
		if(c == ' ') {
			if(!word.empty()) {
				parts.push_back(word);
				word.clear();
			}
		} else {
			word += c;
		}
	}

	if(!word.empty()) parts.push_back(word);

	return parts;
}

int runComm(vector<string> args, bool silent = false,
	 string redirFile = " ", bool append = false) {
	if(args.empty()) return 0;

	pid_t pid = fork();
	if(pid == 0) { //child
//silent-> PID.Log
		if(silent) {
			string file = to_string(getpid()) + ".log";
			int fd = open(file.c_str(),O_WRONLY | O_CREAT |
				 O_TRUNC, 0644);
				dup2(fd, 1);
				dup2(fd, 2);
				close(fd);
			}
// > or >>
		if(!redirFile.empty()) {
			int flags = O_WRONLY | O_CREAT |
				(append ? O_APPEND : O_TRUNC);
			int fd = open(redirFile.c_str(), flags, 0644);
			dup2(fd, 1);
			close(fd);
		}

		vector<char*> argv;
		for(auto &a : args) argv.push_back((char*)a.c_str());
		argv.push_back(nullptr);

		execvp(argv[0], argv.data());
		perror("execvp failed");
		exit(1);
	}else { //parent
		int status;
		waitpid(pid, &status, 0);
		return WEXITSTATUS(status);
	}
}

bool containsSubstr(const string &str, const string &substr, size_t &pos) {
	size_t n = str.size();
	size_t m = substr.size();

	if(m == 0 || n < m) return false;

	for(size_t i = 0; i <= n - m; ++ i) {
		bool match = true;
		for(size_t j = 0 ; j < m; ++j) {
			if(str[i + j] != substr[j]) {
				match = false;
				break;
			}
		}
		if(match) {
			pos = i;
			return true;
		}
	}
	return false;
}

int main(){
	string line;
	while(true) {
		cout << "shell ";
		if(!getline(cin, line)) break;
		if(line.empty()) continue;
		if(line == "exit") break; 

		size_t pos;

		if(containsSubstr(line, "&&", pos)) {
			string a = line.substr(0, pos);
			string b = line.substr(pos + 2);
			if(runComm(split(a)) != 0) runComm(split(b));
			continue;
		}

		if(containsSubstr(line, "||", pos)) {
			string a = line.substr(0, pos);
                        string b = line.substr(pos + 2);
                        if(runComm(split(a)) != 0) runComm(split(b));
                        continue;
		}

		auto start = line.begin();
		bool found = false;
		for(auto it = line.begin(); it != line.end(); ++it) {
			if(*it == ';') {
				found = true;
				break;
			}
		}

		if(found) {
			while(start != line.end()) {
				auto end = find(start, line.end(), ';');
				string cmd(start, end);

				cmd.erase(cmd.begin(), find_if(cmd.begin(),
				cmd.end(), [](char c){return !isspace(c); }));
				cmd.erase(find_if(cmd.rbegin(), cmd.rend(),
					[](char c){ 
					return !isspace(c); }).base(),
				 	cmd.end());

				if(!cmd.empty()) runComm(split(cmd));
				if(end == line.end()) break;
				start = end + 1;
			}
			continue;
		}

		bool silent = false;
		if(line.size() >= 6 && line.substr(0, 6) == "silent") {
			line = line.substr(6);
			silent = true;
		}

		string file = "";
		bool append = false;
		if(containsSubstr(line, ">>", pos)) {
			file = line.substr(pos + 2);
			line = line.substr(0, pos);
			append = true;
		}else if(containsSubstr(line, ">", pos)) {
			file = line.substr(pos + 1);
			line = line.substr(0, pos);
		}

		while(!file.empty() && file.front() == ' ') file.erase(
			file.begin());
		runComm(split(line), silent, file, append);
	}

	return 0;
}


