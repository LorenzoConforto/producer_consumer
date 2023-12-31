using namespace std;
#include <iostream>
#include <omp.h>
#include <string.h>
#include <fstream>
#define N 10 // buffer size
#define I 3 // input streams number
#define O 5 // output streams number

char* buffer;

void streams_init(string*& input_streams, string*& output_streams);

void print()
{
    for(int i = 0 ; i < N ; i++)
        cout << buffer[i];
    cout << endl;
}

void init(){
    for(int i = 0 ; i< N ; i++)
        buffer[i] = '$';
}

int main()
{
    buffer = new char[N];
    init();
    int w = 0; // la prima posizione in cui scrivere è 0
    int r = 0; // la prima posizione in cui dovrò leggere è 0
    bool read_flag = false; // al momento su 0 non c'è niente da leggere
    bool zero_wrote_flag = false; // la metto true quanto un consumer stampa 0 e quindi devono fermarsi anche gli altri
    string* input_streams; // array degli stream di lettura
    string* output_streams; // array degli stream di scrittura
    streams_init(input_streams, output_streams);



    #pragma omp parallel sections
    {
        #pragma omp section // producer 0
        {
            int w_; // valore locale di w
            int r_; // valore locale di r
            bool read_flag_; // valore locale di read_flag
            char c; // carattere temporaneo di lettura
            fstream s;
            s.open(input_streams[0], ios::in);
            if(s.fail())
                cout << "PROBLAMA\n";
            do
            {
                // come prima cosa leggo il valore attuale di w e lo aggiorno per il prossimo thread
                #pragma omp critical
                {
                    //cout << w << endl;
                    w_ = w;
                    w++;
                    if(w_ + 1 == N)
                        w = 0;
                }
                // adesso mi servono anche l'attuale valore di r e di read_flag
                #pragma omp flush(r)
                #pragma atomic read
                    r_ = r;
                #pragma omp flush(read_flag)
                #pragma atomic read
                    read_flag_ = read_flag;
                // verifico di avere spazio per scrivere
                if((r_ <= w_ && (w_-r_+1) < N) || (w_ < r_ && (r_-w_-1) > 0) || (! read_flag_))
                {
                    //cout << "producer   "; print();
                    s >> c;
                    // non serve fare il flush di buffer perchè se posso scriverci vuol dire che è già stato letto e mi sono
                    // già prenotato w quindi nessun altro vuole scriverci
                    if(c != EOF)
                        buffer[w_] = c;
                    // se la flag di lettura era falsa la metto vera perchè adesso c'è qualcosa da leggere
                    if(! read_flag_)
                    {
                        #pragma omp flush(read_flag)
                        #pragma omp atomic write
                            read_flag = true;
                    }
                }
            } while(c != '0' && c != EOF);
            s.close();
        }

        #pragma omp section // consumer 0
        {
            bool zero_wrote_flag_; // valore locale di zero_wrote_flag
            int r_; // valore locale di r
            int w_; // valore locale di w
            char c; // carattere temporaneo
            bool read_flag_; // valore locale di read_flag
            fstream s;
            s.open(output_streams[0], ios::out);
            do
            {
                //come prima mi prendo il valore di r e lo aggiorno per il prossimo thread
                #pragma omp critical
                {
                    r_ = r;
                    r++;
                    if(r_ + 1 == N)
                        r = 0;
                }
                // adesso mi servono i valori di w e read_flag
                #pragma omp flush(w)
                #pragma omp atomic read
                    w_ = w;
                #pragma omp flush(read_flag)
                #pragma omp atomic read
                    read_flag_ = read_flag;
                // verifico se ho qualcosa da leggere
                if((r_ <= w_ || (w_ < r_ && r_ < N)) && read_flag_)
                {
                    /*cout << r_  << "  -  ";
                    print();*/
                    c = buffer[r_];
                    cout << c << endl;
                    if(c != '0')
                    {
                        //cout << c;
                        s << c;
                        //cout << c;
                        #pragma omp flush(zero_wrote_flag)
                        #pragma atomic read
                            zero_wrote_flag_ = zero_wrote_flag;
                    }
                    else
                    {
                        #pragma omp flush(zero_wrote_flag)
                        #pragma atomic write
                            zero_wrote_flag = true;
                        zero_wrote_flag_ = true;
                    }
                    if(r_ == w_)
                    {
                        #pragma omp flush(read_flag)
                        #pragma omp atomic write
                            read_flag = false;
                    }
                }
            }while(! zero_wrote_flag_);
            s.close();
        }
    }


    return 0;
}

void streams_init(string*& input_streams, string*& output_streams)
{
    input_streams = new string[I];
    input_streams[0] = "input_0.txt";
    input_streams[1] = "input_1.txt";
    input_streams[2] = "input_2.txt";

    output_streams = new string[O];
    output_streams[0] = "output_0.txt";
    output_streams[1] = "output_1.txt";
    output_streams[2] = "output_2.txt";
    output_streams[3] = "output_3.txt";
    output_streams[4] = "output_4.txt";
}
