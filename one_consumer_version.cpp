using namespace std;
#include <iostream>
#include <omp.h>
#include <fstream>

#define N 5

int main() {
    int i, j;
    char* buffer;
    bool flag; // true quando non ho niente da leggere
    fstream stream;

    stream.open("input.txt", ios::in);

    buffer = new char[N];
    stream >> buffer[0];
    i = 0; // i sta sull'ultimo che ho scritto (non posso usarlo)
    j = 0; //j sta sul prossimo che devo leggere
    flag = false; // ho un valore da leggere

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            char c;
            do
            {
                int j_;
                bool flag_;
                #pragma omp flush
                #pragma omp atomic read
                    j_ = j;
                #pragma omp flush(flag)
                #pragma omp atomic read
                    flag_ = flag;
                // verifico di avere spazio
                if((j_ <= i && (i-j_+1) < N) || (i < j_ && (j_-i-1) > 0) || flag_)
                {
                    stream >> c;
                    i++;
                    if(i == N)
                        i = 0;
                    buffer[i] = c;
                    #pragma omp flush(flag)
                    #pragma omp atomic write
                        flag = false;
                }
            }while(c != '0');
        }

        #pragma omp section
        {
            while(buffer[j] != '0')
            {
                // verifico di avere qualcosa da stampare
                int i_;
                bool flag_;
                #pragma omp flush(i)
                #pragma omp atomic read
                    i_ = i;
                #pragma omp flush(flag)
                #pragma omp atomic read
                    flag_ = flag;
                if((j <= i_ || (i_ < j && j < N)) && flag_ == false)
                {
                    // se j == i stampo ma poi metto la flag a false
                    if(i_ == j)
                    {
                        #pragma omp flush(flag)
                        #pragma atomic write
                            flag = true;
                    }
                    cout << "Il thread " << omp_get_thread_num() << " ha letto " << buffer[j] << endl;
                    j++;
                    if(j == N)
                        j = 0;
                }
            }
        }
    }

    return 0;
}
