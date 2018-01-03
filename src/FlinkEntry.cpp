#include "FlinkEntry.h"


FlinkEntry::FlinkEntry(){
	counterText = 0;
	relType = -1;
	relString = "";
}

FlinkEntry::~FlinkEntry(){
	relString.clear();
}
