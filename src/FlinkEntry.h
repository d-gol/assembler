#pragma once
#include <string>
class FlinkEntry
{
private:
	int counterText; //gde treba promeniti u tekst sekciji
	int relType; //tip relokacije
	std::string relString; //instrukcija ili direktiva koja je zahtevala relokaciju
	int section; //sekcija u kojoj je koriscen nedefinisan simbol
public:
	FlinkEntry();
	FlinkEntry(int ct, int rt,std::string rs, int sect){
		counterText = ct;
		relType = rt;
		relString.assign(rs);
		int sec = 0;
		if (sect > 0){
			sec = sect - 1;
		}
		section = sec;
	}
	~FlinkEntry();

	FlinkEntry& operator=(FlinkEntry& fe){	//ne valja, ne znam za sad ni da li treba, treba i flink da se iskopira ako treba
		if (this != &fe){
			counterText = fe.counterText;
			relType = fe.relType;
			relString.clear();
			relString.assign(fe.relString);
			section = fe.section;
		}
		return *this;
	}

	int getSection(){
		return section;
	}

	void setSection(int i){
		section = i;
	}

	int getRelType(){
		return relType;
	}

	int getCounterText(){
		return counterText;
	}

	std::string getRelString(){
		return relString;
	}

	friend class Symbol;
};
