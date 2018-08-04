#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <fstream>

using namespace std;

#define time double(clock())/CLOCKS_PER_SEC

int main()
{
    {
        double a=time;
        ofstream fout;
        fout.open("bla1.txt", ios::out);
        for (int i=1; i<=100000; i++) fout<<"Hello World No. "<<i<<"\n";
        cout<<time-a<<endl;
    }
    {
        double a=time;
        FILE* fout;
        fout=fopen("bla2.txt", "w");
        for (int i=1; i<=100000; i++) fprintf(fout, "Hello World No. %i\n", i);
        cout<<time-a<<endl;
    }
    {
        double a = time;
        freopen("bla3.txt", "w", stdout);
        for (int i=1; i<=100000; i++) cout<<"Hello World No. "<<i<<"\n";
        fclose(stdout);
        freopen("/dev/tty", "a", stdout);
        cout << time-a << endl;
    }
    cout << "DONE?" << endl;
    return 0;
    /// system ("pause");
}
