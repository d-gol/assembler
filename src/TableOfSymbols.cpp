#include "TableOfSymbols.h"
#include "WriterOfEverything.h"

List<Symbol>* TableOfSymbols::listOfSymbols = new List<Symbol>();
string TableOfSymbols::currentSection = "";

TableOfSymbols::TableOfSymbols(){

}

TableOfSymbols::~TableOfSymbols(){
	delete listOfSymbols;
}

void TableOfSymbols::addSymbol(Symbol symb){
	listOfSymbols->put(symb);
}

void TableOfSymbols::addSymbolAfter(Symbol symb){
	int sect = 0;
	if (!symb.getName().compare("UND")){
		symb.section = 0;
		listOfSymbols->putInFront(symb);
		updateIdNumbers();
		updateSectionNumbers();
		return;
	}
	sect = symb.getSection() - 1;
	listOfSymbols->insertAfter(symb, sect);
	updateIdNumbers();
}

void TableOfSymbols::updateSymbol(Symbol symb){
	listOfSymbols->update(symb);
}

void TableOfSymbols::updateIdNumbers(){
	Symbol::resetLastId();
	listOfSymbols->onBegin();
	
	while (true){
		listOfSymbols->getCurrent().setId(Symbol::getLastId());
		
		if (listOfSymbols->hasNext())
			listOfSymbols->onNext();
		else
			break;

		Symbol::incLastId();
	}
}

void TableOfSymbols::updateSectionNumbers(){
	listOfSymbols->onBegin();
	if (listOfSymbols->hasNext())
		listOfSymbols->onNext(); //da predjemo und
	else
		return;

	while (true){
		int i = listOfSymbols->getCurrent().getSection();
		listOfSymbols->getCurrent().setSection(i+1);

		if (!listOfSymbols->getCurrent().flink->isEmpty()){

			while (true){
				listOfSymbols->getCurrent().flink->getCurrent().setSection(listOfSymbols->getCurrent().flink->getCurrent().getSection()+1);

				if (listOfSymbols->getCurrent().flink->hasNext())
					listOfSymbols->getCurrent().flink->onNext();
				else
					break;
			}

		}

		if (listOfSymbols->hasNext())
			listOfSymbols->onNext();
		else
			break;
	}
}

bool TableOfSymbols::findSymbol(Symbol s){
	return listOfSymbols->find(s);
}

Symbol TableOfSymbols::getSymbol(string s){
	return listOfSymbols->get(Symbol(s));
}

Symbol& TableOfSymbols::getSymbolByReference(string s){
	return listOfSymbols->getByRef(Symbol(s));
}

void TableOfSymbols::writeTable(){
	listOfSymbols->onBegin();
	while (true){
		Symbol s = listOfSymbols->getCurrent();
		WriterOfEverything::appendTableOfSymbols(s.getName() + "\t\t" + std::to_string(s.getSection()) + "\t\t" + std::to_string(s.getValue()) + "\t\t" + s.getScope() + "\t\t" + std::to_string(s.getId()) + "\n");
		if (listOfSymbols->hasNext())
			listOfSymbols->onNext();
		else
			break;
	}
}

void TableOfSymbols::updateIdNumbersFrom(int start){
	listOfSymbols->onNumber(start);

	while (true){
		int pom = listOfSymbols->getCurrent().getId() + 1;
		listOfSymbols->getCurrent().setId(pom);

		if (listOfSymbols->hasNext())
			listOfSymbols->onNext();
		else
			break;
	}
}

Symbol TableOfSymbols::findByNumber(int b){
	return listOfSymbols->findByNumber(b);
}

void TableOfSymbols::backpatch(){
	listOfSymbols->onBegin();
	if (listOfSymbols != 0){
		while (true){
			Symbol s = listOfSymbols->getCurrentByValue();
			if (!s.isDefined() && s.getScope()!='g'){
				WriterOfEverything::setError("Nedefinisan sibmol: " + s.getName());
				return;
			}

			//idemo kroz svaki FlinkEntry svakog definisanog ili globalnog simbola, koji ima flink
			if (!s.flink->isEmpty()){
				s.flink->onBegin();

				while (true){

					if (s.getSection() != s.flink->getCurrent().getSection() || s.flink->getCurrent().getRelType() == 3 || s.flink->getCurrent().getRelType() == 4 || s.flink->getCurrent().getRelType() == 5 || s.flink->getCurrent().getRelType() == 6){
						//upisemo zapis o relokaciji, ako ima potrebe za njim
						FlinkEntry f = s.flink->getCurrent();
						int sec = f.getSection();
						Symbol pom = TableOfSymbols::findByNumber(sec);

						int refSymbSec = 0;

						if (s.getScope() == 'g'){
							refSymbSec = s.getId();
						}
						else{
							refSymbSec = s.getSection();
						}

						if (checkData(pom.getName())){
							WriterOfEverything::appendRelData(to_string(s.flink->getCurrent().getCounterText()) + "\t\t" + to_string(s.flink->getCurrent().getRelType()) + "\t\t" + to_string(refSymbSec) + "\n");
						}
						if (checkText(pom.getName())){
							WriterOfEverything::appendRelText(to_string(s.flink->getCurrent().getCounterText()) + "\t\t" + to_string(s.flink->getCurrent().getRelType()) + "\t\t" + to_string(refSymbSec) + "\n");
						}
					}
					
					//popravljamo tekst i data sekcije u zavisnosti od tipa relokacije


					//jmp
					if (s.flink->getCurrent().getRelType() == 1 || s.flink->getCurrent().getRelType()==6){
						//pocetak
						std::string l;
						FlinkEntry fe = s.flink->getCurrent();
						l.assign(fe.getRelString());

						istringstream iss(l);
						string instrName, op1, op2, toW = "";

						iss >> instrName;

						iss >> op1;
						iss >> op2;
						int byte1, byte2, byte3, byte4, imm, type;
						imm = -4;

						if (s.getScope() == 'l'){
							imm = s.getValue() - 4;
						}

						if (s.flink->getCurrent().getRelType() == 6){
							if (s.isDefined())
								imm = s.getValue();
							else
								imm = 0;
						}

						if (op1.at(0) == '[')
							type = 3;
						else
							type = 2;

						byte1 = 22 * 4 + type;
						std::stringstream ss;
						ss << std::hex << byte1; // int decimal_value
						std::string res1(ss.str());
						string res11 = fillString(res1);

						byte2 = imm >> 16;
						if (byte2<0)
							byte2 = 256 + byte2;
						std::stringstream ss2;
						ss2 << std::hex << byte2;
						std::string res2(ss2.str());
						string res22 = fillString(res2);

						byte3 = (imm >> 8) & 255;
						std::stringstream ss3;
						ss3 << std::hex << byte3;
						std::string res3(ss3.str());
						string res33 = fillString(res3);

						byte4 = (imm & 255);
						std::stringstream ss4;
						ss4 << std::hex << byte4;
						std::string res4(ss4.str());
						string res44 = fillString(res4);

						toW.append(res11).append(res22).append(res33).append(res44);

						WriterOfEverything::replaceText(fe.getCounterText(), 8, toW); //jeste getCntText ali to je data kod longa

						//kraj za jmp
					}

					//long
					if (s.flink->getCurrent().getRelType() == 3){
						FlinkEntry fe = s.flink->getCurrentByValue();
						string toW = "";

						int val = s.getValue();
						std::stringstream ss;
						ss << std::hex << val; // int decimal_value
						std::string res1(ss.str());
						string res11 = fillStringLong(res1);
						toW.append(res11);

						int sec = fe.getSection();
						Symbol pom = TableOfSymbols::findByNumber(sec);
						
						if (s.getScope() == 'g'){
							toW.clear();
							toW.assign("00000000");
						}

						if (checkData(pom.getName())){
							WriterOfEverything::replaceData(fe.getCounterText(), 8, toW); //jeste getCntText ali to je data kod longa
						}
						else if (checkText(pom.getName())){
							WriterOfEverything::replaceText(fe.getCounterText(), 8, toW); //jeste getCntText ali to je data kod longa
						}

					}

					//je jne..
					if (s.flink->getCurrent().getRelType() == 2){
						std::string l;
						FlinkEntry fe = s.flink->getCurrent();
						l.assign(fe.getRelString());

						istringstream iss(l);
						std::string word, op, imm;
						iss >> word;
						iss >> op;
						iss >> imm;
						int br = Instruction::findInstruction(word);
						int byte1, pom1, byte2, byte3, byte4, pom2;
						std::string toW = "";

						if (br != -1){
							pom1 = stoi(op);
							pom2 = s.getValue() - 4;

							if (s.getScope() == 'g')
								pom2 = -4;

							byte1 = br * 4 + (pom1 >> 3);
							std::stringstream ss;
							ss << std::hex << byte1; // int decimal_value
							std::string res1(ss.str());
							std::string res11 = fillString(res1);

							byte2 = (pom2 >> 16) + ((pom1 & 7) << 5);
							std::stringstream ss2;
							ss2 << std::hex << byte2;
							std::string res2(ss2.str());
							std::string res22 = fillString(res2);

							byte3 = (pom2 >> 8) & 255;
							std::stringstream ss3;
							ss3 << std::hex << byte3;
							std::string res3(ss3.str());
							std::string res33 = fillString(res3);

							byte4 = (pom2 & 255);
							std::stringstream ss4;
							ss4 << std::hex << byte4;
							std::string res4(ss4.str());
							std::string res44 = fillString(res4);

							toW.append(res11).append(res22).append(res33).append(res44);

							WriterOfEverything::replaceText(fe.getCounterText(), 8, toW);
						}
					}
					//ldch, ldcl
					else if (s.flink->getCurrent().getRelType() == 4 || s.flink->getCurrent().getRelType() == 5){
						std::string l;
						FlinkEntry fe = s.flink->getCurrent();
						l.assign(fe.getRelString());

						istringstream iss(l);
						std::string word, op1, op2, imm, toW = "";
						iss >> word;
						iss >> op1;
						iss >> op2;
						int br = Instruction::findInstruction(word);
						int byte1 = 0, pom1 = 0, byte2 = 0, pom2 = 0;

						pom1 = stoi(op1);
						int hl = 0;

						if (!word.compare("ldch")){
							hl = 1;
						}

						if (br != -1){
							pom2 = s.getValue();

							if (s.getScope() == 'g')
								pom2 = 0;

							byte2 = ((pom1 & 3) << 6) + (hl << 5) + ((pom2 >> 11) & 31);
							std::stringstream ss2;
							ss2 << std::hex << byte2;
							std::string res2(ss2.str());
							string res22 = fillString(res2);

							int byte3 = (pom2 >> 3) & 255;
							std::stringstream ss3;
							ss3 << std::hex << byte3;
							std::string res3(ss3.str());
							string res33 = fillString(res3);

							int byte4 = (pom2 & 7) << 5;
							std::stringstream ss4;
							ss4 << std::hex << byte4;
							std::string res4(ss4.str());
							string res44 = fillString(res4);
							toW.append(res22).append(res33).append(res44);

							WriterOfEverything::replaceText(fe.getCounterText() + 2, 6, toW);
						}
					}


					if (s.flink->hasNext())
						s.flink->onNext();
					else
						break;
				}
			}

			if (listOfSymbols->hasNext())
				listOfSymbols->onNext();
			else
				break;
		}
	}
}
