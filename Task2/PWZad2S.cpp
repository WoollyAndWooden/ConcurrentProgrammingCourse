#include <fstream>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    while(true)
    {
        ifstream file("buffer.txt");
        cout << "Opened buffer.txt" << endl;
        string line;
        getline(file, line);
        if(!line.empty())
        {
            ofstream client(line, ios::out);
            cout << "Writting to " << line << endl;
            while(true)
            {
                getline(file, line);
                cout << line << endl;
                if(line!="STOP" && !line.empty())
                    client << line;
                if(line=="STOP")
                    break;
                
            }
        }
        file.close();
        cout << "Closed buffer" << endl;
        ofstream clear("buffer.txt");
        cout << "Cleared buffer" << endl;
        clear.close();
        sleep(1);

    }
    return 0;
}