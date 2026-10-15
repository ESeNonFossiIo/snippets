#include <iostream>

void scambiaChar(char &a, char &b){
    char c;
    c = a;
    a = b;
    b = c;
}

int fattoriale(int num){
    int f = 1;
    for(int i=2;i<=num;i++)
        f *= i;
    return f;
}

int calcolaProcedi(int totale, int passo, int i){  
    return passo - (((int)i/(totale/fattoriale(passo)))%passo) - 1;
}
    
int main(){
    system("clear");
    std::cout << std::endl;
    int i = 0,
        j = 0,
        k = 0;
    
    
    char word[] = "silvia";
    int lenWord = sizeof(word)/sizeof(word[0]) - 1;
    char permutazioni[fattoriale(lenWord)][lenWord];
    int procedi = 0;
    
    // inizializza le permutazioni
    for(i = 0; i < fattoriale(lenWord); i++ )
    {
        for(j=0;j<lenWord;j++)
        {
            permutazioni[i][j] = word[j];
        }
    }
    
    // calcola le permutazioni
    for(j=2; j<=lenWord; j++)
    {
        for(i = 0; i < fattoriale(lenWord); i++)
        {
            procedi = calcolaProcedi(fattoriale(lenWord), j, i);
            for(k=0; k<procedi; k++){ scambiaChar(permutazioni[i][j-1-k],permutazioni[i][j-1-k-1]); }
        }
    }
    
    // stampa le permutazioni
    for(i = 0; i < fattoriale(lenWord); i++ )
    {
        // std::cout << i+1 << " - " << calcolaProcedi(fattoriale(lenWord), 3, i) << " - ";     
        for(j=0;j<lenWord;j++)
        {
            std::cout << permutazioni[i][j];
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    return 0;
}