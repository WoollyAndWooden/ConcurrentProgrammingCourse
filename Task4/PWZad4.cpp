#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

using namespace std;

struct User{
    int Id;
    string surname;
};

string filename = "/tmp/myfifo";

int execClient(string filepath, string id);
int execServer();

int main(int argc, char **argv)
{
    bool server = false;
    bool client = false;
    string filepath;
    string id;
    for(int i=1; i<argc; i++)
    {
        if(strcmp(argv[i], "-c") == 0)
            client = true;
        else if(strcmp(argv[i], "-s") == 0)
            server = true;
        else if(strcmp(argv[i], "-f") == 0)
            filepath = argv[++i];
        else if(strcmp(argv[i], "-i") == 0)
            id = argv[++i];
    }
    if(server == client)
    {
        cout << "Either both server and client arguments were passed, or None was" << endl;
        return 1;
    }
    if(server)
        return execServer();
    if(client)
        return execClient(filepath, id);
    return 0;
}
int execServer()
{
    User Users[5] = {
        {1, "Kowalski"}, 
        {2, "Niedzielski"},
        {3, "a"},
        {4, "b"},
        {5, "c"}
    };

    
    int fifo = mkfifo(filename.c_str(), 0666);
    if(!fifo)
    {
        cout << "Error with server Fifo pipe creation" << endl;
        return 2;
    }

    int serverFifo = open(filename.c_str(), O_RDONLY | O_NONBLOCK);

    while(true)
    {        
        cout << "Starting sleeping..." << endl;

        sleep(15);
        fd_set readfd;
        FD_ZERO(&readfd);
        FD_SET(serverFifo, &readfd);

        int retval = select(serverFifo + 1, &readfd, NULL, NULL, NULL);
        if(retval == -1)
        {
            perror("select");
            close(serverFifo);
            exit(EXIT_FAILURE);
        }
        else if(retval > 0 && FD_ISSET(serverFifo, &readfd))
        {
            cout << "Reading data from server pipe..." << endl;
            char readFromFifo[30];
            char foundSurname[20] = "Not found";
            read(serverFifo, readFromFifo, 30);
            
            if(readFromFifo[0] != '\0')
            {
                cout << "Read " << readFromFifo << endl;
                int id;
                char filepath[20];
                sscanf(readFromFifo, "%d %20[^\n]", &id, filepath);

                int fifoClient = mkfifo(filepath, 0666);
                if(!fifoClient)
                {
                    cout << "Error with client Fifo pipe creation" << endl;
                    return 2;
                }
                for(int i = 0; i < 10; i++)
                {
                    if(Users[i].Id == id)
                    {
                        
                        strcpy(foundSurname, Users[i].surname.c_str());
                        cout << "Found username with this id " << foundSurname << endl;
                        break;
                    }
                }
                int clientFifo = open(filepath, O_WRONLY);
        
                write(clientFifo, foundSurname, 20);
                cout << "Writting to " << filepath << " " << foundSurname << endl;
                
                close(clientFifo);
            }
        }
        
    }
    close(serverFifo);
    return 0;
}

int execClient(string filepath, string id)
{
    mkfifo(filename.c_str(), 0666);
    int serverFifo = open(filename.c_str(), O_WRONLY);
    string text = id + " " + filepath;
    cout << "Writing " << text << endl;
    write(serverFifo, text.c_str(), 30);
    close(serverFifo);
    sleep(2);
    mkfifo(filepath.c_str(), 0666);
    int clientFifo = open(filepath.c_str(), O_RDONLY);
    char result[20];
    read(clientFifo, result, 20);
    cout << result << endl;
    
    close(clientFifo);
    return 0;
}