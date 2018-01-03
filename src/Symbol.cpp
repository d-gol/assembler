#include "Symbol.h"

int Symbol::lastId = 0;
int Symbol::lastSection = 0;

Symbol::Symbol(){
	name = "";
	value = 0;
	flink = new List<FlinkEntry>();
	section = 0;
	scopelId = 'l';
	id = lastId++;
	defined = false;
}

Symbol::Symbol(string n){ //ovo neka ostane bez id, sluzi za pretragu ;)
	name.assign(n);
	value = 0;
	flink = new List<FlinkEntry>();
	defined = false;
	scopelId = 'l';
}

Symbol::Symbol(string n, int v){
	name.assign(n);
	value = v;
	flink = new List<FlinkEntry>();
	id = section = lastSection++;
	defined = false;
	scopelId = 'l';
}

Symbol::Symbol(string n, int v, bool def){ //za sekcije
	name.assign(n);
	value = v;
	flink = new List<FlinkEntry>();
	id = section = lastSection++;
	defined = def;
	scopelId = 'l';
}

Symbol::Symbol(string n, int val, int sec){
	name.assign(n);
	value = val;
	section = sec - 1;
	if (section < 0)
		section = 0;
	flink = new List<FlinkEntry>();
	defined = false;
	id = lastId++;
	scopelId = 'l';
}

Symbol::Symbol(string n, int val, int sec, bool def){
	name.assign(n);
	value = val;
	section = sec - 1;
	if (section < 0)
		section = 0;
	flink = new List<FlinkEntry>();
	defined = def;
	id = lastId++;
	scopelId = 'l';
}

Symbol::Symbol(string n, int val, int sec, char scope){
	name.assign(n);
	value = val;
	section = sec - 1;
	if (section < 0)
		section = 0;
	flink = new List<FlinkEntry>();
	defined = false;
	id = lastId++;
	scopelId = scope;
}

Symbol::Symbol(string n, int val, int sec, char scope, bool def){
	name.assign(n);
	value = val;
	section = sec - 1;
	if (section < 0)
		section = 0;
	flink = new List<FlinkEntry>();
	defined = def;
	id = lastId++;
	scopelId = scope;
}

Symbol::~Symbol(){
	//delete flink;
}

void Symbol::setName(string n){
	name = "";
	name.assign(n);
}