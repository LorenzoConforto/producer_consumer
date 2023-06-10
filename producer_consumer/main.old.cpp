using namespace std;
#include <iostream>

#define N 5

int main() {
    int i, j;
    char* buffer;
    char sel, c;
    bool flag = true; // true quando non ho niente da leggere

    buffer = new char[N];

    cout << "Aggiungi il primo elemento : ";
    cin >> c;
    buffer[0] = c;
    i = 0; // i sta sull'ultimo che ho scritto (non posso usarlo)
    j = 0; //j sta sul prossimo che devo leggere
    flag = false; // ho un valore da leggere

    while(true)
    {
        cout << "Vuoi aggiungere o stampare? ";
        cin >> sel;
        if(sel == 'a') // voglio aggiungere un elemento
        {
            // verifico di avere spazio
            if((j <= i && (i-j+1) < N) || (i < j && (j-i-1) > 0) || flag)
            {
                cout << "Inserire carattere: ";
                cin >> c;
                i++;
                if(i == N)
                    i = 0;
                buffer[i] = c;
                flag = false;
            } else cout << "Non ho spazio" << endl;
        }
        if(sel == 's') // voglio stampare un elemento
        {
            // verifico di avere qualcosa da stampare
            if((j <= i || (i < j && j < N)) && flag==false)
            {
                // se j == i stampo ma poi metto la flag a false
                if(i == j)
                    flag = true;
                cout << "Ho letto " << buffer[j] << endl;
                j++;
                if(j == N)
                    j = 0;
            } else cout << "Non ho niente da leggere" << endl;
        }
    }
    return 0;
}
