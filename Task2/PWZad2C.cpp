#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[])
{
    string filename;
    string buffer = "buffer.txt";
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-f") == 0 && i+1 < argc) filename = argv[++i];
        else if(strcmp(argv[i], "-b") == 0 && i+1 < argc) buffer = argv[++i];
    }
    if (filename == ""){
        cout << "No filename specified" << endl;
        return 1;
    }
        

    int lf;
    while ((lf = open("lockfile", O_WRONLY|O_CREAT|O_EXCL, 0600))<0) {
        if(errno == EACCES) cout << " EACCESS\n";
        if(errno == EEXIST) cout << " EEXIST\n";
        sleep(1);
    }
    cout << "Lockfile created successfully\n";
    ofstream file(buffer, ios::out);

    file << filename << endl;

    string line = "";
    while(line!="STOP")
    {
        getline(cin, line);
        file << line;
        file << endl;
    }

    file.close();
    
    close(lf);
    sleep(2);
    if(unlink("lockfile") == -1) cout << "Error unlinking lockfile " << strerror(errno) << endl;
    else cout << "Lockfile unlinked\n";
    return 0;
}