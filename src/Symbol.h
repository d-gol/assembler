#pragma once
#include <string>
#include "List.h"
#include "functions.h"
#include "FlinkEntry.h"
using namespace std;

class Symbol{
private:
	static int lastId;
	static int lastSection;
	string name;
	int value;
	int section; //redni broj sekcije u kojoj je dati simbol
	int id; //redni broj simbola u tabeli simbola
	char scopelId; //globalni ili lokalni

	bool defined;
	List<FlinkEntry> *flink; //lista obracanja unapred, zapravo lista integera jer je tako lista implementirana
	friend class TableOfSymbols;
public:
	Symbol();
	Symbol(string n);
	Symbol(string n, int val);
	Symbol(string n, int val, int sec);
	Symbol(string n, int val, int sec, bool def);
	Symbol(string n, int val, int sec, char scope);
	Symbol(string n, int val, int sec, char scope, bool def);
	Symbol(string n, int val, bool def);
	
	~Symbol();

	void setId(int i){
		id = i;
	}

	void setValue(int v){
		value = v;
	}

	void addFlink(FlinkEntry fe){
		flink->putInFront(fe);
	}

	bool isDefined(){
		return defined;
	}

	void define(){
		defined = true;
	}

	void undefine(){
		defined = false;
	}

	static int getLastSection(){
		return lastSection;
	}

	static int incLastSection(){
		lastSection++;
	}

	void setName(string s);

	string getName() const{
		return name;
	}

	int getValue() const{
		return value;
	}

	int getSection(){
		return section;
	}

	void setSection(int s){
		section = s;
	}

	static void resetLastId(){
		Symbol::lastId = 0;
	}

	static void incLastId(){
		Symbol::lastId++;
	}

	static int getLastId(){
		return Symbol::lastId;
	}

	int getId(){
		return id;
	}

	char getScope(){
		return scopelId;
	}

	void setScope(char c){
		scopelId = c;
	}

	friend bool operator==(Symbol s1, Symbol s2){	//kada se trazi jednakost dva simbola, gledaju se samo njihova imena
		return (!s1.getName().compare(s2.getName()));
	}

	Symbol& operator=(const Symbol& s){	//ne valja, ne znam za sad ni da li treba, treba i flink da se iskopira ako treba
		if (this != &s){
			name.clear();
			name.assign(s.getName());
			value = s.getValue();
			defined = s.defined;
			section = s.section;
			scopelId = s.scopelId;
			s.flink->onBegin();
			flink = new List<FlinkEntry>;
			if (!s.flink->isEmpty()){
				
				while (true){
					flink->put(s.flink->getCurrent());
					if (!s.flink->hasNext())
						break;
					else
						s.flink->onNext();
				}
				s.flink->onBegin();
			}
		}
		return *this;
	}

};
