#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "TableOfSymbols.h"

#define MAX_INSTR 36 //broj instrukcija

using namespace std;

class Instruction{
private:
	static const string instructions[MAX_INSTR];	// imena instrukcija
	
	string name; //naziv instrukcije
	string op1; //prvi operand
	string op2; //drugi operand
	long imm; //neposredna vrednost

	//stringovi koji se upisuju u text sekciju, prevodi instrukcija
	std::string res11; 
	std::string res22;
	std::string res33;
	std::string res44;

	//pomocni stringovi, sluze za ldc
	std::string res11p;
	std::string res22p;
	std::string res33p;
	std::string res44p;

	bool doubleInstruction; //fleg koji nam sluzi oko upisa, da znamo da li se instrukcija prevodi kao dve instrukcije, za ldc

	bool readOperands(int a, string line);
	void performLdcLh(int tip, string s); //sredi ldch i ldcl
public:
	Instruction();
	~Instruction();

	static int findInstruction(string s); //naci da li je zadata instrukcija u nizu instrukcija, vrati indeks, ako nije - vrati (-1)
	bool readInstruction(string s); // rasparcaj intrukciju, znaci procitaj naziv, odredi tip, procitaj operande, formiraj hex vrednosti za upis
	string writeInstruction(); //upisi u fajl te hex vrednosti koje si dobio
	bool isDoubleInstruction(){ return doubleInstruction;  }
};