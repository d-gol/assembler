#pragma once
#include "Instruction.h"
#include "WriterOfEverything.h"
#include "functions.h"

class Line{
private:
	std::string line; //to je linija koja je preneta iz fajla
	static int LC; //Location Counter unutar trenutne sekcije
//	static int section; //redni broj trenutne SEKCIJE. Znaci, ne redni broj iz tabele simbola, nego npr imamo text1,2,3 i jos neke simbole, section je 3. Za sada ne treba, ostaje za svaki slucaj
	std::string firstWord;
	static bool isEnded; //kada naidjemo na direktivu .end setujemo ovaj fleg i ostale linije se nece obradjivati 

	void performText();
	void performData();
	void performBss();
	void performChar();
	void performAlign();
	void performSkip();
	void performGlobal();
	void performExtern();
	void performInstruction();
	void performLabel();
	void performLong();
	void performEnd();
	void performWord();
public:
	Line(){
		line = "";
		LC = 0;
		firstWord = "";
	}
	Line(std::string s){
		line.assign(s);
		LC = 0;
		firstWord = "";
	}
	~Line(){

	}

	static int getLC(){
		return LC;
	}

	void performLine(std::string s);
};

