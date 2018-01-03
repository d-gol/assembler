#include "WriterOfEverything.h"

std::string WriterOfEverything::tableOfSymbols = "#Table of Symbols""\n";
std::string WriterOfEverything::relText = "\n""#.relText""\n";
std::string WriterOfEverything::relData = "\n\n""#.relData""\n";
std::string WriterOfEverything::text = "\n\n""#.text""\n";
std::string WriterOfEverything::data = "\n\n""#.data""\n";
std::string WriterOfEverything::all = "";
std::string WriterOfEverything::error = "";
int WriterOfEverything::counterText = 9;
int WriterOfEverything::counterData = 9;

bool WriterOfEverything::errorCode = false;
int WriterOfEverything::errorLine = 0;

WriterOfEverything::WriterOfEverything(){
	
}


WriterOfEverything::~WriterOfEverything(){
	clear();
}

void WriterOfEverything::clear(){
	text.clear();
	data.clear();
	relText.clear();
	relData.clear();
	tableOfSymbols.clear();
	all.clear();
}