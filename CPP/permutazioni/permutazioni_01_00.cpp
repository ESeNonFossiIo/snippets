#include <iostream>
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
    int i = 0,
        j = 0,
        h = 0,
        k = 0;
    int num_lettera = 0;
    std::string word = "badsddsc"; // parola di cui si vogliono ottenere le permutazioni
    std::string w = word; // vettore di appoggio per word
    std::string p = word; // vettore della permutazione
    std::string voceDiz = ""; // variabile in cui immagazzinare la vode del dizionarionario con cui si sta lavorando
    int numLettere = word.length();
    long long int f = fattoriale(numLettere);

    word = ordinaStringa(word);
    for(i=0;i<f;i++){
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
        std::cout << p << std::endl;
        while(p<=voceDiz){
            if(p==voceDiz){ /*salvo p tra le permutazioni*/}
            /* passo alla seconda voce*/
        }
    }

    return 0;
}
