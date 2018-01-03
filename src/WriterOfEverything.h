#pragma once
#include "Instruction.h"
/*Klasa u kojoj su svi stringovi za upis!
Dakle: text, data, relText, relData, tabela simbola.
U te stringove cemo upisivati iz Instruction.cpp i ostalog.
Ima i error code, oznacava da li je doslo do greske.
I error line, da vidimo u kojoj linije je doslo do greske
Trazi se PRVA greska, posle nje se prestaje sa citanjem fajla.
Linija te prve greske se upsiuje.
*/
class WriterOfEverything{
private:
	static std::string text;
	static std::string data;
	static std::string relText;
	static std::string relData;
	static std::string tableOfSymbols;
	static std::string error;
	static std::string all;
	
	static int counterText;
	static int counterData;

	static bool errorCode;
	static int errorLine; //linija u ulaznom fajlu u kojoj je doslo do greske
public:
	WriterOfEverything();
	~WriterOfEverything();

	static void setErrorLine(int l){
		errorLine = l;
		all.assign("GRESKA: Linija " + std::to_string(errorLine) + "\t" + error);
	}

	static int getCounterText(){
		return counterText;
	}

	static int getCounterData(){
		return counterData;
	}

	static void replaceText(int start, int end, string s){
		text.replace(start, end, s);
	}

	static void incCounterText(int n){
		counterText += n;
	}

	static void replaceData(int start, int end, string s){
		data.replace(start, end, s);
	}

	static void incCounterData(int n){
		counterData += n;
	}

	static bool hasError(){
		return errorCode;
	}

	void clear(); //obrisi sve iz svih stringova

	static std::string getText(){
		return text;
	}

	static std::string getData(){
		return data;
	}

	static std::string getRelText(){
		return relText;
	}

	static std::string getRelData(){
		return relData;
	}

	static std::string getTableOfSymbols(){
		return tableOfSymbols;
	}

//set
	static void setText(std::string s){
		text.assign(s);
	}

	static void setData(std::string s){
		data.assign(s);
	}

	static void setRelText(std::string s){
		relText.assign(s);
	}

	static void setRelData(std::string s){
		relData.assign(s);
	}

	static void setTableOfSymbols(std::string s){
		tableOfSymbols.assign(s);
	}
//append
	static void appendText(std::string s){
		text.append(s);
	}

	static void appendData(std::string s){
		data.append(s);
	}

	static void appendRelText(std::string s){
		relText.append(s);
	}

	static void appendRelData(std::string s){
		relData.append(s);
	}

	static void appendTableOfSymbols(std::string s){
		tableOfSymbols.append(s);
	}

	static std::string writeEverything(){
		all.append(tableOfSymbols);
		all.append(relText);
		all.append(relData);
		all.append(text);
		all.append(data);
		return all;
	}

	static void setError(string err){
		if (errorCode)
			return;	//ako je vec postavljena greska, ne postavljaj novu

		errorCode = true;
		tableOfSymbols.clear();
		text.clear();
		data.clear();
		relText.clear();
		relData.clear();
		error.assign(err);
		all.assign("GRESKA: Linija " + std::to_string(errorLine) + "\t" + err);
	}

};