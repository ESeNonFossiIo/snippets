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
        k = 0,
        h = 0;
    float percentuale = 0;
    int percentuale_int = 0;
    
    char word[] = "mattia";
    int lenWord = sizeof(word)/sizeof(word[0]) - 1;
    char permutazioni[lenWord];
    int procedi = 0;
    
    // inizializza le permutazioni
    for(j=0; j<lenWord; j++)
        permutazioni[j] = word[j];
    
    // calcola le permutazioni
    for(i = 0; i < fattoriale(lenWord); i++)
    {
        for(j=2; j<=lenWord; j++)
        {
            procedi = calcolaProcedi(fattoriale(lenWord), j, i);
            for(k=0; k<procedi; k++){ scambiaChar(permutazioni[j-1-k],permutazioni[j-1-k-1]); }
        }        
        for(j=0; j<lenWord; j++)
        {
            std::cout << permutazioni[j];
            permutazioni[j] = word[j];
        }   
        system("clear");   
        
        percentuale = ((((i+1)*100)/fattoriale(lenWord)));
        percentuale_int = 2*(int)(percentuale/10);
        std::cout << "\n\n";

        std::cout << "0% ";
        for(h=0;h <= percentuale_int ; h++)
           std::cout << "#"; 
        for(h= percentuale_int+1; h<= 20; h++)
           std::cout << " ";   
        std::cout << " 100% -> ";
        std::cout << percentuale  << "% \n\n";
    }
    
    std::cout << std::endl;
    return 0;
}