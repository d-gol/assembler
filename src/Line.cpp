#include "Line.h"

int Line::LC = 0;

bool Line::isEnded = false;

void Line::performLine(string s){
	if (s == "")
		return;
	if (!isEnded){

		line.assign(s);
		istringstream iss(line);

		iss >> firstWord;

		if (checkText(firstWord))
			performText();
		else if (checkData(firstWord))
			performData();
		else if (checkBss(firstWord))
			performBss();
		else if (!firstWord.compare(".char"))
			performChar();
		else if (!firstWord.compare(".align"))
			performAlign();
		else if (!firstWord.compare(".skip"))
			performSkip();
		else if (!firstWord.compare(".global"))
			performGlobal();
		else if (!firstWord.compare(".extern"))
			performExtern();
		else if (!firstWord.compare(".long"))
			performLong();
		else if (!firstWord.compare(".word"))
			performWord();
		else if (!firstWord.compare(".end"))
			performEnd();
		else if (firstWord.at(firstWord.length() - 1) == ':')
			performLabel();
		else
			performInstruction();
	}
}

void Line::performEnd(){
	isEnded = true;
}

void Line::performInstruction(){
	Instruction *instr = new Instruction();
	if (!instr->readInstruction(line)){
		WriterOfEverything::setError("Nelegalan kod operacije. Procitana vrednost iz ulaznog fajla nije ni labela, ni direktiva, ni instrukcija, ni sekcija.");
		return;
	}
	LC += 4;
	instr->writeInstruction();

	WriterOfEverything::incCounterText(9);
}

void Line::performAlign(){
	istringstream iss(line);
	iss >> firstWord;

	string op;
	iss >> op;

	int oper = stoi(op);

	if (LC % oper != 0){
		LC += (oper - (LC % 4));
	}
}

void Line::performExtern(){
	int stringIter = 0, currentSymbol = 0, start = 0;
	string line2 = line.substr(8, line.length());
	int value = 0;
	value = Line::LC;

	while (stringIter < line2.length()){
		if (line2.at(stringIter) == ',' || stringIter == line2.length() - 1){
			string symbolName = line2.substr(start, stringIter - start+1);

			if (TableOfSymbols::findSymbol(Symbol(symbolName))){ //ako je u tabeli simbola
				Symbol s = TableOfSymbols::getSymbol(symbolName);

				if (s.isDefined()){ //ako je definisan
					s.setScope('g');
					TableOfSymbols::updateSymbol(s);
				}
				else{ //ako nije definisan
					if (!TableOfSymbols::findSymbol(Symbol("UND"))){//ako ne postoji und sekcija
						TableOfSymbols::addSymbolAfter(Symbol("UND", 0, true));
					}
					else{ //ako postoji

					}
					s.setSection(0);
					s.setScope('g');
					TableOfSymbols::updateSymbol(s);
				}
			}
			else{ // ako nije u tabeli simbola
				if (!TableOfSymbols::findSymbol(Symbol("UND"))){ //ako ne postoji und sekcija
					TableOfSymbols::addSymbolAfter(Symbol("UND", 0, true));
				}
				else{ //ako postoji

				}
				Symbol newSymbol = Symbol(symbolName, 0, 0, 'g'); // dodajemo ga u tabelo simbola vrednost je uvek nula za globalne simbole!
				TableOfSymbols::addSymbol(newSymbol);
			}


			currentSymbol = 0;
			start = stringIter + 1;
		}
		else{
			currentSymbol++;
		}
		stringIter++;
	}
}

void Line::performGlobal(){
	int stringIter = 0, currentSymbol = 0, start = 0;
	string line2 = line.substr(8, line.length());
	int value = 0;
	value = Line::LC;

	while (stringIter < line2.length()){
		if (line2.at(stringIter) == ',' || stringIter==line2.length()-1){
			string symbolName = line2.substr(start, stringIter-start+1);

			if (TableOfSymbols::findSymbol(Symbol(symbolName))){ //ako je u tabeli simbola
				Symbol s = TableOfSymbols::getSymbol(symbolName);

				if (s.isDefined()){ //ako je definisan
					s.setScope('g');
					TableOfSymbols::updateSymbol(s);
				}
				else{ //ako nije definisan
					if (!TableOfSymbols::findSymbol(Symbol("UND"))){//ako ne postoji und sekcija
						TableOfSymbols::addSymbolAfter(Symbol("UND", 0, true));
					}
					else{ //ako postoji
						
					}
					s.setSection(0);
					s.setScope('g');
					TableOfSymbols::updateSymbol(s);
				}
			}
			else{ // ako nije u tabeli simbola
				if (!TableOfSymbols::findSymbol(Symbol("UND"))){ //ako ne postoji und sekcija
					TableOfSymbols::addSymbolAfter(Symbol("UND", 0, true));
				}
				else{ //ako postoji

				}
				Symbol newSymbol = Symbol(symbolName, 0, 0, 'g'); // dodajemo ga u tabelo simbola vrednost je uvek nula za globalne simbole!
				TableOfSymbols::addSymbol(newSymbol);
			}
			

			currentSymbol = 0;
			start = stringIter+1;
		}
		else{
			currentSymbol++;
		}
		stringIter++;
	}
}

void Line::performLabel(){
	istringstream iss(line);
	iss >> firstWord;

	std::string symbolName;
	symbolName = line.substr(0, firstWord.length() - 1);

	string secondWord;
	iss >> secondWord;
	int value = 0;
	value = Line::LC;

	if (secondWord == ""){
		
		
	}
	else{
		std::size_t pos = line.find(secondWord);      // position of "live" in str
		std::string newLine = line.substr(pos);
		line.assign(newLine);
		istringstream iss5(newLine);
		iss5 >> firstWord;

		if (checkText(firstWord))
			performText();
		else if (checkData(firstWord))
			performData();
		else if (checkBss(firstWord))
			performBss();
		else if (!firstWord.compare(".char"))
			performChar();
		else if (!firstWord.compare(".align"))
			performAlign();
		else if (!firstWord.compare(".skip"))
			performSkip();
		else if (!firstWord.compare(".global"))
			performGlobal();
		else if (!firstWord.compare(".extern"))
			performExtern();
		else if (!firstWord.compare(".long"))
			performLong();
		else if (!firstWord.compare(".end"))
			performEnd();
		else if (firstWord.at(firstWord.length() - 1) == ':')
			WriterOfEverything::setError("Dve labele u jednoj liniji.");
		else
			performInstruction();
	}

	if (TableOfSymbols::findSymbol(Symbol(symbolName))){
		Symbol s = TableOfSymbols::getSymbol(symbolName);

		if (s.isDefined()){
			WriterOfEverything::setError("Dupla definicija simbola: " + symbolName);
			return;
		}
		else{
			s.define();
			s.setValue(value);
			s.setSection(Symbol::getLastSection()-1);
			TableOfSymbols::updateSymbol(s);
		}
	}
	else{
		TableOfSymbols::addSymbol(Symbol(symbolName, value, Symbol::getLastSection(), true));
	}

}

void Line::performChar(){
	istringstream iss(line);
	iss >> firstWord;

	string op;
	iss >> op;

	if (isNumber(op)){

		long broj = stol(op);

		std::stringstream ss;
		ss << std::hex << broj;
		std::string res1(ss.str());

		res1 = fillString(res1);

		if (checkData(TableOfSymbols::getCurrentSection())){
			WriterOfEverything::appendData(res1 + " ");
			WriterOfEverything::incCounterData(3);
		}
		if (checkText(TableOfSymbols::getCurrentSection())){
			WriterOfEverything::appendText(res1 + " ");
			WriterOfEverything::incCounterText(3);
		}
	}
	else{
		
	}
}

void Line::performLong(){
	istringstream iss(line);
	iss >> firstWord;
	Symbol s;
	long broj = 0;

	string op;
	iss >> op;

	if (isNumber(op)){
		broj = stol(op);
		
		std::stringstream ss;
		ss << std::hex << broj;
		std::string res1(ss.str());

		res1 = fillStringLong(res1);

		Symbol pom = TableOfSymbols::findByNumber(s.getSection());

		if (checkData(TableOfSymbols::getCurrentSection())){
			WriterOfEverything::appendData(res1 + " ");
			WriterOfEverything::incCounterData(9);
		}
		if (checkText(TableOfSymbols::getCurrentSection())){
			WriterOfEverything::appendText(res1 + " ");
			WriterOfEverything::incCounterText(9);
		}
		return;
	}
	else{
		if (TableOfSymbols::findSymbol(Symbol(op))){
			s = TableOfSymbols::getSymbol(op);
			Symbol pom = TableOfSymbols::findByNumber(s.getSection());
			int cnt = 0;

			if (checkData(TableOfSymbols::getCurrentSection())){
				cnt = WriterOfEverything::getCounterData();
			}
			else if (checkText(TableOfSymbols::getCurrentSection())){
				cnt = WriterOfEverything::getCounterText();
			}

			if (s.isDefined()){
				int sCheck = 0;
				if (Symbol::getLastSection() > 0){
					sCheck = Symbol::getLastSection() - 1;
				}
				FlinkEntry fe = FlinkEntry(cnt, 3, line, Symbol::getLastSection());
				s.addFlink(fe);
				TableOfSymbols::updateSymbol(s);
			}
			else{
				FlinkEntry fe = FlinkEntry(cnt, 3, line, Symbol::getLastSection());
				s.addFlink(fe);
				TableOfSymbols::updateSymbol(s);
			}
		}
		else{
			s = Symbol(op, 0, Symbol::getLastSection());//value =0
			int cntAdd=0;
			if (checkData(TableOfSymbols::getCurrentSection())){
				cntAdd = WriterOfEverything::getCounterData();
			}
			else if (checkText(TableOfSymbols::getCurrentSection())){
				cntAdd = WriterOfEverything::getCounterText();
			}
			FlinkEntry fe = FlinkEntry(cntAdd, 3, line, Symbol::getLastSection());
			s.addFlink(fe);
			TableOfSymbols::addSymbol(s);
		}

		broj = s.getValue();
	}

	std::stringstream ss;
	ss << std::hex << broj;
	std::string res1(ss.str());

	res1 = fillStringLong(res1);

	Symbol pom = TableOfSymbols::findByNumber(s.getSection());

	if (checkData(TableOfSymbols::getCurrentSection())){
		WriterOfEverything::appendData(res1 + " ");
		WriterOfEverything::incCounterData(9);
	}
	if (checkText(TableOfSymbols::getCurrentSection())){
		WriterOfEverything::appendText(res1 + " ");
		WriterOfEverything::incCounterText(9);
	}
}

void Line::performWord(){
	istringstream iss(line);
	iss >> firstWord;

	string op;
	iss >> op;

	if (isNumber(op)){

		int broj = stoi(op);

		std::stringstream ss;
		ss << std::hex << broj;
		std::string res1(ss.str());

		res1 = fillStringWord(res1);

		if (checkData(TableOfSymbols::getCurrentSection())){
			WriterOfEverything::appendData(res1 + " ");
			WriterOfEverything::incCounterData(5);
		}
		if (checkText(TableOfSymbols::getCurrentSection())){
			WriterOfEverything::appendText(res1 + " ");
			WriterOfEverything::incCounterText(5);
		}
	}
	else{
		
	}
}

void Line::performSkip(){
	istringstream iss(line);
	iss >> firstWord;

	string op;
	iss >> op;

	int oper = stoi(op);

	LC += oper;

	//
	string zaUpis = std::string(oper, '0');
	zaUpis.append(" ");
	
	if (checkData(TableOfSymbols::getCurrentSection())){
		WriterOfEverything::appendData(zaUpis);
		WriterOfEverything::incCounterData(oper + 1);
	}
	else if(checkText(TableOfSymbols::getCurrentSection())){
		WriterOfEverything::appendText(zaUpis);
		WriterOfEverything::incCounterText(oper + 1);
	}
}

void Line::performData(){
	LC = 0;
	//dodaj u tabelu simbola!
	if (TableOfSymbols::findSymbol(Symbol(firstWord))){
		WriterOfEverything::setError("Definisane dve .data sekcije sa istim nazivom.");
		return;
	}

	istringstream iss(line);
	iss >> firstWord;
	string adr = "";
	iss >> adr;
	int address = 0;
	if (adr != "")
		address = stoi(adr);

	TableOfSymbols::addSymbolAfter(Symbol(firstWord, address, true)); //ovde pazi
	TableOfSymbols::setCurrentSection(firstWord);
}

void Line::performText(){
	LC = 0;
	//dodaj u tabelu simbola!
	if (TableOfSymbols::findSymbol(Symbol(firstWord))){
		WriterOfEverything::setError("Definisane dve .text sekcije sa istim nazivom.");
		return;
	}
	istringstream iss(line);
	iss >> firstWord;
	string adr = "";
	iss >> adr;
	int address = 0;
	if (adr != "")
		address = stoi(adr);

	TableOfSymbols::addSymbolAfter(Symbol(firstWord, address, true));
	TableOfSymbols::setCurrentSection(firstWord);
}

void Line::performBss(){
	LC = 0;
	//dodaj u tabelu simbola!
	if (TableOfSymbols::findSymbol(Symbol(firstWord))){
		WriterOfEverything::setError("Definisane dve .bss sekcije sa istim nazivom.");
		return;
	}
	istringstream iss(line);
	iss >> firstWord;
	string adr = "";
	iss >> adr;
	int address = 0;
	if (adr != "")
		address = stoi(adr);

	TableOfSymbols::addSymbolAfter(Symbol(firstWord, address, true));
	TableOfSymbols::setCurrentSection(firstWord);
}
