#include "helper2.h"
#include <netinet/ip.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: ./client <port> “<command_name> <arg1> <arg2> ... <argN>”" << endl;
        return -1;
    }

    if (!isValidPortNumber(argv[1]))
    {
        cout << "ERROR: Invalid port number" << endl;
        return -1;
    }

    //set add struct
    struct sockaddr_in st;
    st.sin_family = AF_INET;
    st.sin_addr.s_addr = INADDR_ANY;
    st.sin_port = stoi(argv[1]);

    //set up socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == -1)
    {
        cout << "ERROR: Failed to create an client socket" << endl;
        return -1;
    }

    //connect to server
    if (connect(clientSocket, (const struct sockaddr *)&st, sizeof(st)) == -1)
    {
        cout << "ERROR: Failed to connect" << endl;
        return -1;
    }

    //keep asking user for input
    char input[MAX_MESSAGE];
    string command = argv[2];
    // string to char array
    strcpy(input, command.c_str());
    // print command
    cout << "[C]: " << command << endl;

    //send message to server
    if (send(clientSocket, &input, sizeof(input), 0) < (int)sizeof(input))
    {
        cout << "ERROR: sending data" << endl;
    }

    // get feedback
    char feedback[MAX_MESSAGE];

    if (recv(clientSocket, &feedback, sizeof(char) * MAX_MESSAGE, 0) == -1)
    {
        cout << "ERROR: receiving data" << endl;
    }
    else
    {
        cout << feedback << endl;
    }

    return 0;
}
