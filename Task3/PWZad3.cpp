#include<iostream>
#include<fstream>
#include <regex>
#include <unistd.h>
#include <sys/wait.h>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    if(!(argc == 5 || argc == 6))
    {
        cout << "Incorrect number of arguments" << endl;
        return -1;
    }
    bool child = false;
    string filename, searchString;
    for(int i=1; i < argc; i++)
    {
        if(strcmp(argv[i], "-p") == 0)
        {
            filename = argv[++i];
        }
        else if(strcmp(argv[i], "-s") == 0)
        {
            searchString = argv[++i];
        }
        else if(strcmp(argv[i], "child") == 0)
            child = true;
        else
        {
            cout << "Invalid argument" << endl;
            return -2;
        }
    }
    ifstream file(filename);
    int count = 0;
    if(file.is_open())
    {
        string line;
        vector<pid_t> childPids;
        regex inputPattern(R"(\\input\{(.+?)\})");
        //cout << inputPattern << endl;
        cout << "Searching for " << searchString << " in " << filename << endl;
        while(getline(file, line))
        {
            smatch match;
            if(regex_search(line, match, inputPattern))
            {
                cout << "Opening new file " << match[1] << endl;
                string newFilename = match[1];

                pid_t pid = fork();
                if(pid == -1)
                {
                    cout << "Failed to fork" << endl;
                    return -3;
                }
                else if(pid == 0)
                {
                    string params = "-p " + newFilename + " -s " + searchString;
                    execl("./Zad3", argv[0], "-p", newFilename.c_str(), "-s", searchString.c_str(), "child", nullptr);

                    cout << "Failed to execute child" << endl;
                    return -4;
                }
                else
                    childPids.push_back(pid);
            }
                
            else
            {
                size_t pos = line.find(searchString);
                while(pos != string::npos)
                {
                    count++;
                    pos = line.find(searchString, pos+1);
                }
            }
        }

        cout << "Found " << count << " occurences in " << filename << endl;

        for(pid_t pid : childPids)
        {
            int status;
            waitpid(pid, &status, 0);

            if(WIFEXITED(status))
            {
                int exitCode = WEXITSTATUS(status);
                if(exitCode < 0)
                    cout << "Error: " << status << endl;
                else
                    count+=WEXITSTATUS(status);
            }
            else
                cerr << "Child process terminated abnormally" << endl;
        }
    }
    else
        cout << "Could not open " << filename << endl;

    if(child)
        return count;

    cout << searchString << " occured in files exactly " << count << " times" << endl;

    return 0;
}