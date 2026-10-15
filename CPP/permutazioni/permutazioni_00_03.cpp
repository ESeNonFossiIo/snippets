#include <iostream>
#include <cstring>
#include <fstream>

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
    int trovati =0;
    std::string s;
    std::fstream f("permutazioni_trovate.txt", std::ios::out);
    if(!f) {
        std::cout << "Errore nella creazione del file permutazioni_trovate.txt!";
        return -1;
    }
    f.close();
    
    system("clear");
    std::cout << std::endl;
    int i = 0,
        j = 0,
        k = 0;
    float percentuale = 0;
    int percentuale_int = 0;
    
    
    std::string word;
    std::cout << "\n\t Di quale parola vuoi calcolare le permutazioni? ";
	std::cin >> word;
    
    int lenWord = word.length();
    //std::cout << lenWord;
    char* permutazioni = new char[lenWord];
    int procedi = 0;
    
    // inizializza le permutazioni
    for(j=0; j<lenWord; j++)
        permutazioni[j] = word[j];
    
    std::cout << "\n\t Permutazione di " << word << " valutata: " << permutazioni << "\n" ;
    // calcola le permutazioni
    for(i = 1; i < fattoriale(lenWord); i++)
    {
        for(j=2; j<=lenWord; j++)
        {
            procedi = calcolaProcedi(fattoriale(lenWord), j, i);
            for(k=0; k<procedi; k++){ scambiaChar(permutazioni[j-1-k],permutazioni[j-1-k-1]); }
        }        
                std::fstream t("permutazioni_totali.txt", std::ios::app);
                t << i << ". " << permutazioni << "\n";
                t.close();
        
        std::ifstream h("dizionario.txt");
            while(h.good()) {
            getline(h, s); //preleva una riga dal file
            if(s==permutazioni){
                trovati ++;
                std::fstream f("permutazioni_trovate.txt", std::ios::app);
                f << trovati << ". " <<permutazioni;
                f << "\n";
                f.close();
            }
            //std::cout << s << std::endl ;
        }
        h.close();
        
        system("clear");  
        
        std::cout << "\n\t Permutazione di " << word << " valutata: " << permutazioni << "\n" ;
        
        for(j=0; j<lenWord; j++)
        {
            permutazioni[j] = word[j];
        }               
    
        percentuale = ((((i+1)*100)/fattoriale(lenWord)));
        percentuale_int = 2*(int)(percentuale/10);
        std::cout << "\n\n";

        std::cout << "\tProgresso: \t0% ";
        for(j=0;j <= percentuale_int ; j++)
           std::cout << "#"; 
        for(j= percentuale_int+1; j<= 20; j++)
           std::cout << " ";   
        std::cout << " 100% -> ";
        std::cout << percentuale  << "% \n\n"; 
        
        std::cout << "\n\n \tPermutazioni di " << word << " trovate -> \n";
        std::ifstream g("permutazioni_trovate.txt");

        while(g.good()) {
            getline(g, s); //preleva una riga dal file
            std::cout << "\t\t\t\t" <<s<<std::endl;
        }
        g.close();
    }
    std::cout << std::endl;
    delete[] permutazioni;
    return 0;
}