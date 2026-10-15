#include <iostream>
#include <fstream>
//#include <string>

long long int fattoriale(int num){
    long long int f = 1;
    int i = 2;
    for( i=2; i<=num ; i++)
        f *= i;
    if(num==1){f=1;}
    return f;
}
long long int exp(int num, int esp){
    long long int f = 1;
    int i = 2;
    for(i=1; i<=esp ; i++)
        f *= num;
    return f;
}
std::string ordinaStringa(std::string stringa){
    int i,j;
    int n = stringa.length();
    char temp;

    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            if(stringa[i]<stringa[j]){
                temp=stringa[j];
                stringa[j]=stringa[i];
                stringa[i]=temp;
            }
        }
    }
    return stringa;
}

int main(){
    system("clear");
    int i = 0,
        j = 0,
        h = 0,
        k = 0,
        trovati = 0,
        percentuale_int = 0;
    float percentuale = 0;
    int num_lettera = 0;
    std::string original_word;
    std::cout << "\n\t Di quale parola vuoi calcolare le permutazioni? ";
	std::cin >> original_word;
    std::string word = original_word; // parola di cui si vogliono ottenere le permutazioni
    word = ordinaStringa(word);
    std::string w = word; // vettore di appoggio per word
    std::string p = word; // vettore della permutazione
    std::string voceDiz = ""; // variabile in cui immagazzinare la vode del dizionarionario con cui si sta lavorando
    int numLettere = word.length();
    long long int f = fattoriale(numLettere);

    //std::fstream permu_tot("permutazioni_totali.txt", std::ios::out);
    std::ifstream diz("dizionario.txt"); //apro il dizionario

    //permu_tot << "0" << ".\t " << word <<  "\n";
    getline(diz, voceDiz);
                    if(p==voceDiz){ /*salvo p tra le permutazioni*/
                    trovati ++;
                    std::fstream permutazioni("permutazioni_trovate.txt", std::ios::app);
                    permutazioni << trovati << "." << p;
                    permutazioni << "\n";
                    permutazioni.close();
                }
    if(diz.good()){
            while(p>=voceDiz){
                if(!diz.good()){break;}
                if(p==voceDiz){ /*salvo p tra le permutazioni*/
                    trovati ++;
                    std::fstream permutazioni("permutazioni_trovate.txt", std::ios::app);
                    permutazioni << trovati << "." << p;
                    permutazioni << "\n";
                    permutazioni.close();
                    break;
                }
                getline(diz, voceDiz);
            }
                /* passo alla seconda voce*/
        }

    std::fstream permutazioni("permutazioni_trovate.txt", std::ios::out);
    if(!permutazioni) {
        std::cout << "Errore nella creazione del file permutazioni_trovate.txt!";
        return -1;
    }
    permutazioni.close();


    for(i=1;i<f;i++){
        for(j=1;j<numLettere;j++){
            num_lettera = (i/(f/(f/fattoriale(numLettere - j))))%(numLettere-j+1);
            //std::cout << num_lettera;
            h = 0;
            k = -1;
            while(h <= num_lettera){
                if(w[k]!='*'){h++;}
                k++;
            }
            while(w[k]=='*'){
                k++;
            }
            p[j-1] = w[k];
            w[k] = '*';
            // bisogna stampare la k esima lettera
        }
        k = 0;
        while(w[k]=='*'){k++;}
        p[numLettere-1] = w[k];
        w = word;
        system("clear");

        //permu_tot << i << ".\t " << p << "\n";

        std::cout << "\n\t Le permutazioni di " << original_word << " considerata: " << p << std::endl;

        percentuale = ((((i+1)*100)/f));
        percentuale_int = 2*(int)(percentuale/10);
        std::cout << "\n\n";

        std::cout << "\tProgresso: \t0% ";
        for(j=0;j <= percentuale_int ; j++)
           std::cout << "#";
        for(j= percentuale_int+1; j<= 20; j++)
           std::cout << " ";
        std::cout << " 100% -> ";
        std::cout << percentuale  << "% \n\n";

        if(diz.good()){
            while(p>=voceDiz){
                if(!diz.good()){break;}
                getline(diz, voceDiz);
                if(p==voceDiz){ /*salvo p tra le permutazioni*/
                    trovati ++;
                    std::fstream permutazioni("permutazioni_trovate.txt", std::ios::app);
                    permutazioni << trovati << "." << p;
                    permutazioni << "\n";
                    permutazioni.close();
                    break;
                }
            }
        }
    }

    std::cout << "\n\n \tPermutazioni di " << word << " trovate sono " << trovati << " ->\t";
    std::ifstream g("permutazioni_trovate.txt");
    if(getline(g, p)) //preleva una riga dal file
        std::cout << p <<std::endl;
    while(g.good()){
        getline(g, p); //preleva una riga dal file
        std::cout << "\t\t\t\t\t\t" <<p<<std::endl;
    }
    g.close();

    diz.close();
    //permu_tot.close();
    return 0;
}
