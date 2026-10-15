#include <string>
#include <vector>

class Splitter {
		std::vector<std::string> _tokens;
	public:
		typedef std::vector<std::string>::size_type size_type;
	public:
		Splitter ( const std::string& src, const std::string& delim )
		{
			reset ( src, delim );
		}
		std::string& operator[] ( size_type i )
		{
			return _tokens.at ( i );
		}
		size_type size() const
		{
			return _tokens.size();
		}
		void reset ( const std::string& src, const std::string& delim )
		{
			std::vector<std::string> tokens;
			std::string::size_type start = 0;
			std::string::size_type end;
			for ( ; ; ) {
				end = src.find ( delim, start );
				tokens.push_back ( src.substr ( start, end - start ) );
				if ( end == std::string::npos )
					break;
					start = end + delim.size();
				}
				_tokens.swap ( tokens );
		}
};


#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream> //header necessario per ofstream!

int main(int argc, char** argv)
{
	if ( argc < 2 ){
		std::cout << "\n Perfavore inserisci un indirizzo nell command line. (ex. "<<argv[0]<<" /dir )\n\n";
		exit (EXIT_FAILURE);
	}else{
		std::cout << "\n output.txt verra' generato per la directory "<<argv[1]<<"."<<std::endl;
	}

	std::ofstream f("output.txt"); //se il file non esiste lo crea, altrimenti lo sovrascrive!
	if(!f) {
		std::cout<<"Errore nella creazione del file!"<<std::endl;
		return -1;
	}else{
		std::cout<<" Il file output.txt e' stato inizializzato correttamente!"<<std::endl;
	}

	// std::cout<<argc<<std::endl;
	DIR *dir;
	struct dirent *entry;

	const std::string line ="start";
	Splitter split ( line, "," );

	std::cout<<" output.txt:\n\n"<<std::endl;

	if ((dir = opendir(argv[1])) == NULL)
		perror("opendir() error");
		else {
			// puts("contents of root:");
			while ((entry = readdir(dir)) != NULL){
				split.reset( entry->d_name, "." );
				if((split.size()>1)&&(split[0]!="")){
					f << "g09 <" <<split[0]<<"."<<split[1]<< " >" <<split[0]<<".out"<<std::endl;
					std::cout << "\tg09 <" <<split[0]<<"."<<split[1]<< " >" <<split[0]<<".out"<<std::endl;
				}

				}
				// std::cout << split.size() << std::endl;
				// std::cout << entry->d_name << std::endl;
				// for ( Splitter::size_type i = 0; i < split.size(); i++ )
				// 	std::cout<<'|'<< split[i] <<"|\n";
				// }
				closedir(dir);
			}

	f.close(); //chiudo il file
	std::cout<<"\n\n Scrittura avvenuta con successo!\n\n";
	return 0;
}

// using namespace std;
//
// int main() {
//
//
// 	f << "ciao"<<endl //si può inserire qualsiasi cosa nello stream, stringhe
// 	<< 1 << endl //numeri, con virgola o meno
// 	<< 1.3 << endl; //l'endl indica che ogni cosa viene scritta a capo, se fosse omesso scriverebbe tutto su una linea
//
//
//
// 	return 0;
// }
