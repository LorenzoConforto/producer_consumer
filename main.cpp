using namespace std;
#include <iostream>
#include <omp.h>
#include <fstream>

#define N 5

int main() {
    int i = 0, j = 0;
    char* buffer;
    bool flag; // true quando non ho niente da leggere
    bool check = true; // quando è false uno dei consumer ha trovato '0' quindi mi fermo
    fstream stream;

    stream.open("input.txt", ios::in);

    buffer = new char[N];
    stream >> buffer[0];

    #pragma omp parallel
    {
        #pragma omp single
        {
            char c;
            do
            {
                // verifico di avere spazio
                if((j <= i && (i-j+1) < N) || (i < j && (j-i-1) > 0))
                {
                    stream >> c;
                    i++;
                    if(i == N)
                        i = 0;
                    buffer[i] = c;
                    #pragma omp task firstprivate(j)
                    {
                        char temp;
                        #pragma omp flush(buffer)
                        #pragma omp atomic read
                        temp = buffer[j];
                        cout << "Il thread " << omp_get_thread_num() << " ha letto " << temp << endl;
                    }
                    j++;
                    if(j == N)
                        j = 0;

                }
            }while(c != '0');
        }
    }

    return 0;
}
