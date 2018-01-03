#pragma once
#include <string>

inline bool checkText(std::string s){
	int endIndex = 5;
	int startIndex = 0;

	std::string text = s.substr(startIndex, endIndex);

	if (text.compare(".text")) //ako nije tekst
		return false;

	if (text.length() == 5)
		return true;

	endIndex = s.length() - 1;
	startIndex = 6;

	std::string number = s.substr(startIndex, endIndex);
	int index = stoi(number); //redni broj npr text1, text333 itd

	return true;
}

inline bool checkData(std::string s){
	int endIndex = 5;
	int startIndex = 0;

	std::string text = s.substr(startIndex, endIndex);

	if (text.compare(".data")) //ako nije tekst
		return false;

	if (text.length() == 5)
		return true;

	endIndex = s.length() - 1;
	startIndex = 6;

	std::string number = s.substr(startIndex, endIndex);
	int index = stoi(number); //redni broj npr data1, data333 itd

	return true;
}

inline bool checkBss(std::string s){
	int endIndex = 4;
	int startIndex = 0;

	std::string text = s.substr(startIndex, endIndex);

	if (text.compare(".bss")) //ako nije tekst
		return false;

	if (text.length() == 4)
		return true;

	endIndex = s.length() - 1;
	startIndex = 5;

	std::string number = s.substr(startIndex, endIndex);
	int index = stoi(number); //redni broj npr data1, data333 itd

	return true;
}

#pragma warning( disable : 4018 ) //za proveru if (s.at(i)>=10)
inline bool isNumber(std::string s){
	int i = 0;
	while (i < s.length()){
		if ((int)s.at(i) >= 'a'){
			return false;
		}
		i++;
	}
	return true;
}

inline std::string fillString(std::string s){ // funkcija koja pomaze oko ispisa u heksadecimalnom obliku, npr ako se dobije 3h ona to pretvara u 03h 
	std::string ret = "00";
	if (s.length() == 2){
		ret.assign(s);
	}
	else{
		ret.at(1) = s.at(0);
		ret.at(0) = '0';
	}
	return ret;
}

inline std::string fillStringLong(std::string s){
	std::string ret = "00000000";
	if (s.length() == 8){
		ret.assign(s);
	}
	else{
		int startIndex = 8 - s.length();
		int endIndex = 8;
		ret.replace(startIndex, endIndex, s);
	}
	return ret;
}

inline std::string fillStringWord(std::string s){
	std::string ret = "0000";
	if (s.length() == 4){
		ret.assign(s);
	}
	else{
		int startIndex = 4 - s.length();
		int endIndex = 4;
		ret.replace(startIndex, endIndex, s);
	}
	return ret;
}

inline int countNonSpace(std::string s,int poc, int duz){
	int br = 0;
	for (int i = poc; i < poc + duz; i++){
		if (s.at(i)!=' ')
			br++;
	}
	return br;
}