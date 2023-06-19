using namespace std;
#include <iostream>
#include <omp.h>
#include <fstream>

#define N 5

int main() {
    int i, j;
    char* buffer;
    bool flag_lettura; // true quando non ho niente da leggere
    bool flag_fine; // la metto true quando un thread stampa zero
    fstream stream;

    stream.open("input.txt", ios::in);

    buffer = new char[N];
    stream >> buffer[0];
    i = 0; // i sta sull'ultimo che ho scritto (non posso usarlo)
    j = 0; //j sta sul prossimo che devo leggere
    flag_lettura = false; // ho un valore da leggere
    flag_fine = false;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            char c;
            do
            {
                int j_;
                bool flag_;
                #pragma omp flush(i)
                #pragma omp atomic read
                    j_ = j;
                #pragma omp flush(flag_lettura)
                #pragma omp atomic read
                    flag_ = flag_lettura;
                // verifico di avere spazio
                if((j_ <= i && (i-j_+1) < N) || (i < j_ && (j_-i-1) > 0) || flag_)
                {
                    stream >> c;
                    i++;
                    if(i == N)
                        i = 0;
                    buffer[i] = c;
                    #pragma omp flush(flag_lettura)
                    #pragma omp atomic write
                        flag_lettura = false;
                }
            }while(c != '0');
        }

        #pragma omp section
        {
            cout << "CIAO SONO IL THREAD " << omp_get_thread_num() << endl;
            bool flag_fine_;
            do
            {
                // verifico di avere qualcosa da stampare
                int i_;
                int j_;
                bool flag_lettura_;
                #pragma omp flush(i)
                #pragma omp atomic read
                    i_ = i;
                #pragma omp flush(j)
                #pragma omp atomic read
                    j_ = j;
                #pragma omp flush(flag_lettura)
                #pragma omp atomic read
                    flag_lettura_ = flag_lettura;
                if((j_ <= i_ || (i_ < j_ && j_ < N)) && flag_lettura_ == false)
                {
                    // se j == i stampo ma poi metto la flag_lettura a false
                    if(i_ == j_)
                    {
                        #pragma omp flush(flag_lettura)
                        #pragma atomic write
                            flag_lettura = true;
                    }
                    cout << "Il thread " << omp_get_thread_num() << " ha letto " << buffer[j] << endl;
                    #pragma omp flush(j)
                    #pragma omp atomic write
                            j = j + 1;
                    if(j_ + 1 == N)
                    {
                        #pragma omp flush(j)
                        #pragma omp atomic write
                            j = 0;
                    }
                    if(buffer[j_] == '0')
                    {
                        #pragma omp flush(flag_fine)
                        #pragma atomic write
                            flag_fine = true;
                    }
                    #pragma omp flush(flag_fine)
                    #pragma atomic read
                        flag_fine_ = flag_fine;
                }
            } while(! flag_fine_);
        }
#pragma omp section
        {
            cout << "CIAO SONO IL THREAD " << omp_get_thread_num() << endl;
            bool flag_fine_;
            do
            {
                // verifico di avere qualcosa da stampare
                int i_;
                int j_;
                bool flag_lettura_;
#pragma omp flush(i)
#pragma omp atomic read
                i_ = i;
#pragma omp flush(j)
#pragma omp atomic read
                j_ = j;
#pragma omp flush(flag_lettura)
#pragma omp atomic read
                flag_lettura_ = flag_lettura;
                if((j_ <= i_ || (i_ < j_ && j_ < N)) && flag_lettura_ == false)
                {
                    // se j == i stampo ma poi metto la flag_lettura a false
                    if(i_ == j_)
                    {
#pragma omp flush(flag_lettura)
#pragma atomic write
                        flag_lettura = true;
                    }
                    cout << "Il thread " << omp_get_thread_num() << " ha letto " << buffer[j] << endl;
#pragma omp flush(j)
#pragma omp atomic write
                    j = j + 1;
                    if(j_ + 1 == N)
                    {
#pragma omp flush(j)
#pragma omp atomic write
                        j = 0;
                    }
                    if(buffer[j_] == '0')
                    {
#pragma omp flush(flag_fine)
#pragma atomic write
                        flag_fine = true;
                    }
#pragma omp flush(flag_fine)
#pragma atomic read
                    flag_fine_ = flag_fine;
                }
            } while(! flag_fine_);
        }
#pragma omp section
        {
            cout << "CIAO SONO IL THREAD " << omp_get_thread_num() << endl;
            bool flag_fine_;
            do
            {
                // verifico di avere qualcosa da stampare
                int i_;
                int j_;
                bool flag_lettura_;
#pragma omp flush(i)
#pragma omp atomic read
                i_ = i;
#pragma omp flush(j)
#pragma omp atomic read
                j_ = j;
#pragma omp flush(flag_lettura)
#pragma omp atomic read
                flag_lettura_ = flag_lettura;
                if((j_ <= i_ || (i_ < j_ && j_ < N)) && flag_lettura_ == false)
                {
                    // se j == i stampo ma poi metto la flag_lettura a false
                    if(i_ == j_)
                    {
#pragma omp flush(flag_lettura)
#pragma atomic write
                        flag_lettura = true;
                    }
                    cout << "Il thread " << omp_get_thread_num() << " ha letto " << buffer[j] << endl;
#pragma omp flush(j)
#pragma omp atomic write
                    j = j + 1;
                    if(j_ + 1 == N)
                    {
#pragma omp flush(j)
#pragma omp atomic write
                        j = 0;
                    }
                    if(buffer[j_] == '0')
                    {
#pragma omp flush(flag_fine)
#pragma atomic write
                        flag_fine = true;
                    }
#pragma omp flush(flag_fine)
#pragma atomic read
                    flag_fine_ = flag_fine;
                }
            } while(! flag_fine_);
        }
#pragma omp section
        {
            cout << "CIAO SONO IL THREAD " << omp_get_thread_num() << endl;
            bool flag_fine_;
            do
            {
                // verifico di avere qualcosa da stampare
                int i_;
                int j_;
                bool flag_lettura_;
#pragma omp flush(i)
#pragma omp atomic read
                i_ = i;
#pragma omp flush(j)
#pragma omp atomic read
                j_ = j;
#pragma omp flush(flag_lettura)
#pragma omp atomic read
                flag_lettura_ = flag_lettura;
                if((j_ <= i_ || (i_ < j_ && j_ < N)) && flag_lettura_ == false)
                {
                    // se j == i stampo ma poi metto la flag_lettura a false
                    if(i_ == j_)
                    {
#pragma omp flush(flag_lettura)
#pragma atomic write
                        flag_lettura = true;
                    }
                    cout << "Il thread " << omp_get_thread_num() << " ha letto " << buffer[j] << endl;
#pragma omp flush(j)
#pragma omp atomic write
                    j = j + 1;
                    if(j_ + 1 == N)
                    {
#pragma omp flush(j)
#pragma omp atomic write
                        j = 0;
                    }
                    if(buffer[j_] == '0')
                    {
#pragma omp flush(flag_fine)
#pragma atomic write
                        flag_fine = true;
                    }
#pragma omp flush(flag_fine)
#pragma atomic read
                    flag_fine_ = flag_fine;
                }
            } while(! flag_fine_);
        }
    }

    return 0;
}
