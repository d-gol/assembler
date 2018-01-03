#include "Instruction.h"
#include "TableOfSymbols.h"
#include "WriterOfEverything.h"
#include "Line.h"
#include <fstream>

int main(int argc, char* argv[]){
	TableOfSymbols *tos = new TableOfSymbols();
	WriterOfEverything *woe = new WriterOfEverything();
	int lineNumber = 1; //linija u ulaznom fajlu, sluzi nam da je inkrementiramo i da na osnovu nje znamo gde je doslo do greske, ako dodje do greske

	ifstream ulazniFajl;
	ofstream izlazniFajl;

	std::string strUlaz = "", strIzlaz = "";

	strUlaz.assign(argv[1]);
	strIzlaz.assign(argv[1]);
	strUlaz.append(".txt");
	strIzlaz.append("_o");
	strIzlaz.append(".txt");

	ulazniFajl.open(strUlaz);
	izlazniFajl.open(strIzlaz);
	string ulaz;

	Line *line = new Line();
	if (ulazniFajl.is_open()){ //ako smo uspesno otvorili
		while (!ulazniFajl.eof()){	//idemo kroz ulazni fajl, citamo liniju po liniju i upisujemo u izlazni
			getline(ulazniFajl, ulaz);
			line->performLine(ulaz);
			if (WriterOfEverything::hasError()){ //ako se nadje greska, ne cita se dalje ulazni fajl
				WriterOfEverything::setErrorLine(lineNumber);
				break;
			}
			lineNumber++;
		}
		tos->backpatch();
		if (!WriterOfEverything::hasError())
			tos->writeTable(); //na kraju upisemo tabelu simbola, ako nema greske
	
		izlazniFajl << WriterOfEverything::writeEverything() << endl;
	}

	ulazniFajl.close();
	izlazniFajl.close();

	return 0;
}
