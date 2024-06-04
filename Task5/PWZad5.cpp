#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>

#define SERVER_KEY 12
#define CLIENT_KEY 22

using namespace std;

struct message{
    long type;
    char data[256];
};

int execClient(char word[256]);
int execServer();

int main(int argc, char **argv)
{
    bool server = false;
    bool client = false;
    char word[256];
    int receive;
    int send;
    for(int i=1; i<argc; i++)
    {
        if(strcmp(argv[i], "-c") == 0)
            client = true;
        else if(strcmp(argv[i], "-s") == 0)
            server = true;
        else if(strcmp(argv[i], "-w") == 0)
            strcpy(word, argv[++i]);
    }
    if(server == client)
    {
        cout << "Either both server and client arguments were passed, or None was" << endl;
        return 1;
    }
    if(server)
        return execServer();
    if(client)
        return execClient(word);
    return 0;
}

int execServer()
{
    struct word{
        char eng[256];
        char pol[256];
    };
    word dictionary[5]
    {
        {"food", "jedzenie"},
        {"mushroom", "grzyb"},
        {"phone", "telefon"},
        {"task", "zadanie"},
        {"language", "język"},
    };
    int receive;
    int send;
    if(send = msgget(SERVER_KEY, 0777 | IPC_CREAT) == -1)
    {
        perror("msgget send");
        exit(EXIT_FAILURE);
    }
    if(receive = msgget(CLIENT_KEY, 0777 | IPC_CREAT) == -1)
    {
        perror("msgget receive");
        exit(EXIT_FAILURE);
    }
    cout << send << " " << receive << endl;
    struct message msg;
    for(int i = 0; i < 5; i++)
    {
        cout << "Waiting for requests..." << endl;
        cout << i << endl;
        cout << "Reading requests..." << endl;
        if(msgrcv(receive, &msg, sizeof(msg.data), 0, 0) == -1)
        {
            perror("msgrcv receive");
            exit(EXIT_FAILURE);
        }
        cout << "Received " << msg.data << " from " << msg.type << endl;
        bool found = false;
        for(int i = 0; i < 5; i++)
        {
            if(strcmp(msg.data, dictionary[i].pol) == 0)
            {
                found = true;
                strcpy(msg.data, dictionary[i].eng);
                break;
            }
            else if(strcmp(msg.data, dictionary[i].eng) == 0)
            {
                found = true;
                strcpy(msg.data, dictionary[i].pol);
                break;
            }
        }
        if(!found)
            strcpy(msg.data, "Not found");
        cout << "Sending back " << msg.data << " to " << msg.type << endl;
        if(msgsnd(send, &msg, sizeof(msg.data), 0) == -1)
        {
            perror("msgsnd send");
            exit(EXIT_FAILURE);
        }
    }
    msgctl(send, IPC_RMID, 0);
    msgctl(receive, IPC_RMID, 0);
    return 0;
}

int execClient(char word[256])
{
    message msg;
    msg.type = getpid();
    strcpy(msg.data, word);

    int receive;
    int send;

    //reverse send and receive, since names are server oriented
    if(receive = msgget(SERVER_KEY, 0777 | IPC_CREAT) == -1)
    {
        perror("msgget receive");
        exit(EXIT_FAILURE);
    }
    if(send = msgget(CLIENT_KEY, 0777 | IPC_CREAT) == -1)
    {
        perror("msgget send");
        exit(EXIT_FAILURE);
    }

    if(msgsnd(send, &msg, sizeof(msg.data), 0) == -1)
    {
        perror("msgsend send");
        exit(EXIT_FAILURE);
    }

    cout << "Sent " << msg.data << endl;
    strcpy(msg.data, "Still waiting");

    if(msgrcv(receive, &msg, sizeof(msg.data), msg.type, 0) == -1)
    {
        perror("msgrcv receive");
        exit(EXIT_FAILURE);
    }
    cout << "Received " << msg.data << endl;

    return 0;
}