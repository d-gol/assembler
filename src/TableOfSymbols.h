#pragma once
#include "Symbol.h"
#include "List.h"

class TableOfSymbols{
private:
	static List<Symbol> *listOfSymbols; //lista svih simbola

	static void updateIdNumbersFrom(int start);
	static void updateIdNumbers();
	static void updateSectionNumbers();
	static string currentSection; //koja je trenutna sekcija
public:
	TableOfSymbols();
	~TableOfSymbols();

	static void addSymbol(Symbol s); //dodavanje na kraj. Staticka jer tabeli simbola pristupamo iz drugih klasa, ne samo iz main!
	static void addSymbolAfter(Symbol s);
	static bool findSymbol(Symbol s);
	static Symbol getSymbol(string s);
	static Symbol& getSymbolByReference(string s);
	static Symbol findByNumber(int b);
	static void writeTable(); 
	static void backpatch();
	static void updateSymbol(Symbol s);
	static void setCurrentSection(string s){
		currentSection.assign(s);
	}
	static string getCurrentSection(){
		return currentSection;
	}
};