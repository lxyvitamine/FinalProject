#include "helper2.h"
#include <netinet/ip.h>
#include <signal.h>
#include <sys/select.h>
using namespace std;

struct {
	int fd;
	bool wantRead;
	bool wantSend;
	char buffer[1000];
	int bytesInBuffer;
	int bytesWritten;
} connections[100];

int numC = 0;

void* parseUserinput(void *input);

// Extra Credit 5: signal handler for SIGINT (Ctrl+C)
// void signalHandler(int signum) {
//     pthread_mutex_lock(&isShutdownLock);
//     isShutdown = true;
//     pthread_mutex_unlock(&isShutdownLock);
//     // call shutdown
//     pthread_mutex_lock(&parseUserinputLock);
//     shutdown(password);
//     pthread_mutex_unlock(&parseUserinputLock);
//     exit(signum);
// }

// accept and change data in struct
void handleIncomingConnection() {
	int acceptSd = accept(sd, NULL, NULL);
    
	if (acceptSd == -1) {
		return;
	}
    
	connections[numC].fd = acceptSd;
	connections[numC].wantRead = true;
	connections[numC].wantSend = false;
	bzero(connections[numC].buffer, 1000);
	connections[numC].bytesInBuffer = 0;
	connections[numC].bytesWritten = 0;
	numC++; // increment number of connections
}

void handleRead(int i) {
	int r = read(connections[i].fd, &connections[i].buffer[connections[i].bytesInBuffer], 
                 1000 - connections[i].bytesInBuffer);
	// cout << connections[i].buffer << endl;
	if (r == 0) {
		close(connections[i].fd);
		connections[i] = connections[--numC];
	} else {
		connections[i].bytesInBuffer += r;
		if (strstr(connections[i].buffer, "\n") != nullptr) { // until client press enter
			connections[i].wantRead = false;
			connections[i].wantSend = true;
		}
	}
}

void handleSend(int i) {
    string input(connections[i].buffer);
    
	input = input.substr(0, input.length() - 1); // delete "\n"
	// cout << input << endl;
    
    // if command is shutdown
	if (input.compare("shutdown " + password) == 0) { // equal
		pthread_mutex_lock(&isShutdownLock);
		isShutdown = true;
		pthread_mutex_unlock(&isShutdownLock);

		pthread_mutex_lock(&parseUserinputLock);
		shutdown(password);
		pthread_mutex_unlock(&parseUserinputLock);

		char shutdownOK[MAX_MESSAGE] = "[R]: OK";

		if ((int) send(connections[i].fd, &shutdownOK, sizeof(char) * MAX_MESSAGE, 0)
				< (int) (sizeof(char) * MAX_MESSAGE)) {
			cout << "ERROR: sending data" << endl;
		}

		shutdown(sd, SHUT_RDWR);
		close (connections[i].fd);
        // delete inputptr;
		// exit(0); 
	} else { // command not shutdown
		// call parseuserinput
		parseUserinput(&input);

		pthread_mutex_lock(&sendToClientLock);
		if ((int) send(connections[i].fd, &sendToClient, sizeof(char) * MAX_MESSAGE, 0)
				< (int) (sizeof(char) * MAX_MESSAGE)) {
			cout << "ERROR: sending data" << endl;
		}
		memset(sendToClient, 0, MAX_MESSAGE);
		pthread_mutex_unlock(&sendToClientLock);

		close (connections[i].fd);
	}
	connections[i] = connections[--numC];
}

void* parseUserinput(void *input) {
	pthread_mutex_lock(&parseUserinputLock);

	string userinput = *(static_cast<string*>(input));
	vector<string> inputs = parseCmd(userinput, " ");

// mutex
// no argument
	if (inputs.size() == 1) {
		if (inputs[0] == "list_candidates") {
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, list_candidtates().c_str());
			pthread_mutex_unlock(&sendToClientLock);
		} else if (inputs[0] == "view_result") {
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, view_result().c_str());
			pthread_mutex_unlock(&sendToClientLock);
		} else {
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, errorMsg.c_str());
			pthread_mutex_unlock(&sendToClientLock);
			//pthread_exit(NULL);
		}
	}
// 1 argument
	else if (inputs.size() == 2) {
		string cmd = inputs[0];
		string arg = inputs[1];

		if (cmd == "start_election") {
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, start_election(arg).c_str());
			pthread_mutex_unlock(&sendToClientLock);
		} else if (cmd == "end_election") {
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, end_election(arg).c_str());
			pthread_mutex_unlock(&sendToClientLock);
		}
// else if (cmd == "shutdown")
// {
//     shutdown(arg);
// }
		else if (cmd == "add_voter") {
			if (!isNumber(arg)) {
				//end thread
				//pthread_exit(NULL);
				pthread_mutex_lock(&sendToClientLock);
				strcpy(sendToClient, errorMsg.c_str());
				pthread_mutex_unlock(&sendToClientLock);

				pthread_mutex_unlock(&parseUserinputLock);
				return NULL;
			}
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, add_voter(stoi(arg)).c_str());
			pthread_mutex_unlock(&sendToClientLock);
		} else if (cmd == "check_registration_status") {
			if (!isNumber(arg)) {
				//end thread
				//pthread_exit(NULL);
				pthread_mutex_lock(&sendToClientLock);
				strcpy(sendToClient, errorMsg.c_str());
				pthread_mutex_unlock(&sendToClientLock);

				pthread_mutex_unlock(&parseUserinputLock);
				return NULL;
			}
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, check_registration_status(stoi(arg)).c_str());
			pthread_mutex_unlock(&sendToClientLock);
		} else if (cmd == "vote_count") {
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, vote_count(arg).c_str());
			pthread_mutex_unlock(&sendToClientLock);
		} else {
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, errorMsg.c_str());
			pthread_mutex_unlock(&sendToClientLock);
			//pthread_exit(NULL);
		}
	}
// 2 argument
	else if (inputs.size() == 3) {
		string cmd = inputs[0];
		string arg1 = inputs[1];
		string arg2 = inputs[2];

		if (cmd == "add_candidate") {
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, add_candidate(arg1, arg2).c_str());
			pthread_mutex_unlock(&sendToClientLock);
		} else if (cmd == "vote_for") {
			if (!isNumber(arg2)) {
				//pthread_exit(NULL);
				pthread_mutex_lock(&sendToClientLock);
				strcpy(sendToClient, errorMsg.c_str());
				pthread_mutex_unlock(&sendToClientLock);

				pthread_mutex_unlock(&parseUserinputLock);
				return NULL;
			}
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, vote_for(arg1, stoi(arg2)).c_str());
			pthread_mutex_unlock(&sendToClientLock);
		} else if (cmd == "check_voter_status") {
			if (!isNumber(arg1) || !isNumber(arg2)) {
				//pthread_exit(NULL);
				pthread_mutex_lock(&sendToClientLock);
				strcpy(sendToClient, errorMsg.c_str());
				pthread_mutex_unlock(&sendToClientLock);

				pthread_mutex_unlock(&parseUserinputLock);
				return NULL;
			}
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient,
					check_voter_status(stoi(arg1), stoi(arg2)).c_str());
			pthread_mutex_unlock(&sendToClientLock);
		} else {
			pthread_mutex_lock(&sendToClientLock);
			strcpy(sendToClient, errorMsg.c_str());
			pthread_mutex_unlock(&sendToClientLock);
			//pthread_exit(NULL);
		}
	} else {
		pthread_mutex_lock(&sendToClientLock);
		strcpy(sendToClient, errorMsg.c_str());
		pthread_mutex_unlock(&sendToClientLock);
	}

	pthread_mutex_unlock(&parseUserinputLock);
	return NULL;
}

int main(int argc, char *argv[]) {
    // signal(SIGINT, signalHandler);

    //default to 10000
	unsigned short int port = 10000;

    // 1. parse command
    // -r -p -a
	int option;
	while ((option = getopt(argc, argv, "a:rp:")) != -1) {
		switch (option) {
		case 'a':
			password = optarg;
			changePassword = true;
			break;
		case 'r':
			// read file
			recover();
			break;
		case 'p':
			//connect to port
			if (!isValidPortNumber(optarg)) {
				cout << "ERROR: Invalid port number" << endl;
				exit(0);
				break;
			}
			port = stoi(optarg);
			break;
		case '?':
			exit(0);
			break;
		}
	}

    //address filling out the structure
	struct sockaddr_in st;
	st.sin_family = AF_INET;
	st.sin_addr.s_addr = INADDR_ANY; // bind to the current IP of this machine
	st.sin_port = port;

    //set-up socket
	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd == -1) {
		cout << "ERROR: create server socket" << endl;
		shutdown(sd, SHUT_RDWR);
		return -1;
	}

	int reuse_address = 1;

	if ((setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(int)))
			== -1) {
		cerr << "ERROR: set socket reuseaddress" << endl;
	}

    //bind
	if (bind(sd, (const struct sockaddr*) &st, sizeof(st)) == -1) {
		cout << "ERROR: bind" << endl;
		shutdown(sd, SHUT_RDWR);
		return -1;
	}

    //listen
	if (listen(sd, 20) == -1) {
		cout << "ERROR: listen" << endl;
		shutdown(sd, SHUT_RDWR);
		return -1;
	}

	if (0 != pthread_mutex_init(&sdLock, NULL)) {
		throw "Failed to initialize a mutex";
	}

	if (0 != pthread_mutex_init(&isShutdownLock, NULL)) {
		throw "Failed to initialize a mutex";
	}

    // if (0 != pthread_mutex_init(&runningThreadLock, NULL))
    // {
    //     throw "Failed to initialize a mutex";
    // }

	if (0 != pthread_mutex_init(&parseUserinputLock, NULL)) {
		throw "Failed to initialize a mutex";
	}

	if (0 != pthread_mutex_init(&sendToClientLock, NULL)) {
		throw "Failed to initialize a mutex";
	}

	if (0 != pthread_mutex_init(&candidatesLock, NULL)) {
		throw "Failed to initialize a mutex";
	}

	if (0 != pthread_mutex_init(&votersLock, NULL)) {
		throw "Failed to initialize a mutex";
	}

	while (1) {
        pthread_mutex_lock(&isShutdownLock);
        if (isShutdown)
        {
            pthread_mutex_unlock(&isShutdownLock);
            break;
        }
        pthread_mutex_unlock(&isShutdownLock);
        
		fd_set r, w;
		FD_ZERO(&r);
		FD_ZERO(&w);
		FD_SET(sd, &r);
		int maxFD = sd;
		for (int i = 0; i < numC; i++) {
			if (connections[i].wantRead) {
				FD_SET(connections[i].fd, &r);
			}
			if (connections[i].wantSend) {
				FD_SET(connections[i].fd, &w);
			}

			if (maxFD < connections[i].fd) {
				maxFD = connections[i].fd;
			}
		}

		select(maxFD + 1, &r, &w, NULL, NULL);

		if (FD_ISSET(sd, &r)) {
			handleIncomingConnection();
		}
		for (int i = 0; i < numC; i++) {
			if (connections[i].wantRead && FD_ISSET(connections[i].fd, &r))
				handleRead(i);
			if (connections[i].wantSend && FD_ISSET(connections[i].fd, &w))
				handleSend(i);
		}
	}
	return 0;
}
