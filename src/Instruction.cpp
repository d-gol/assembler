#include "Instruction.h"
#include "WriterOfEverything.h"
#include "functions.h"
#include "Line.h"

const string Instruction::instructions[] = {
	"add", "sub", "mul", "div", "cmp", "and", "or", "not",
	"test", "ldr", "str", "je", "jne", "jge", "jg", "jle",
	"jl", "jp", "jn", "jo", "jno", "call", "ret/iret/jmp", "push",
	"pop", "mov", "in", "out", "shr", "shl", "int", "ldch/l",
	"ldc"
};

Instruction::Instruction(){
	op1 = "";
	op2 = "";
	imm = 0;
	name = "";
	res11 = "";
	res22 = "";
	res33 = "";
	res44 = "";
	res11p = "";
	res22p = "";
	res33p = "";
	res44p = "";
	doubleInstruction = false;
}

int Instruction::findInstruction(string s){
	int ret = -1;
	string helper;

	helper.assign(s);

	if (!s.compare("jmp") || !s.compare("ret") || !s.compare("iret"))
		helper.assign("ret/iret/jmp");
	if (!s.compare("ldch") || !s.compare("ldcl"))
		helper.assign("ldch/l");

	for (int i = 0; i < MAX_INSTR; i++){
		if (!instructions[i].compare(helper)){
			ret = i;
			break;
		}
	}

	return ret;
}

bool Instruction::readInstruction(string s){
	istringstream iss(s);
	string word;

	iss >> word;
	name.assign(word);
	int br = findInstruction(word);
	if (br != -1){
		if (!readOperands(br, s))
			return false;
	}
	else
		return false; // privremeno ovako, smisli sta ces za greske! ustvari ok je ovo :)

	return true;
}

bool Instruction::readOperands(int tip, string line){
	int byte1;
	int byte2;
	int byte3;
	int byte4;
	int type; // za ret iret jmp

	istringstream iss2(line);
	string word;
	iss2 >> word; //predjemo naziv instrukcije

	switch (tip){
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:  case 25: case 26: case 27: case 28: case 29:{ //add sub mul div cmp ldr str and or not test mov in out shl shr

				iss2 >> op1;
				iss2 >> op2;
				if (tip != 5 && tip != 6 && tip != 7 && tip != 8 && tip != 25 && tip != 26 && tip != 27)
					iss2 >> imm;
				else
					imm = 0;

				int pom1 = stoi(op1);
				int pom2 = stoi(op2);

				byte1 = tip * 4 + (pom1 >> 2);
				std::stringstream ss;
				ss << std::hex << byte1; // int decimal_value
				std::string res1(ss.str());
				res11 = fillString(res1);

				byte2 = (imm >> 16) + (pom2 << 2) + ((pom1 & 3) << 6);
				std::stringstream ss2;
				ss2 << std::hex << byte2;
				std::string res2(ss2.str());
				res22 = fillString(res2);

				byte3 = (imm >> 8) & 255;
				std::stringstream ss3;
				ss3 << std::hex << byte3;
				std::string res3(ss3.str());
				res33 = fillString(res3);

				byte4 = (imm & 255);
				if (tip == 28 || tip == 29){
					byte4 = 0;
				}
				std::stringstream ss4;
				ss4 << std::hex << byte4;
				std::string res4(ss4.str());
				res44 = fillString(res4);

				return true;
				break;
	}
	case 30:{ //int
				iss2 >> op1;
				int pom1 = stoi(op1);

				byte1 = tip * 4 + (pom1 >> 2);
				std::stringstream ss;
				ss << std::hex << byte1; // int decimal_value
				std::string res1(ss.str());
				res11 = fillString(res1);

				byte2 = (pom1 & 3) << 6;
				std::stringstream ss2;
				ss2 << std::hex << byte2;
				std::string res2(ss2.str());
				res22 = fillString(res2);

				res33.assign("00");
				res44.assign("00");
				
				return true;
				break;
	}
	case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: case 20: case 23: case 24:{ //instrukcija skoka i push i pop
				 iss2 >> op1;
				 iss2 >> op2;
				 int pom1 = stoi(op1);
				 Symbol s;

				 if (!isNumber(op2)){
					 //sredi za labelu
					 if (TableOfSymbols::findSymbol(Symbol(op2))){
						 s = TableOfSymbols::getSymbol(op2);

						 if (s.isDefined()){
							 int sCheck = 0;
							 if (Symbol::getLastSection() > 0){
								 sCheck = Symbol::getLastSection() - 1;
							 }
							 if (s.getSection() == sCheck){

							 }
							 else{
								 FlinkEntry fe = FlinkEntry(WriterOfEverything::getCounterText(), 2, line, Symbol::getLastSection());
								 s.addFlink(fe);
								 TableOfSymbols::updateSymbol(s);
							 }
						 }
						 else{
							 FlinkEntry fe = FlinkEntry(WriterOfEverything::getCounterText(), 2, line, Symbol::getLastSection());
							 s.addFlink(fe);
							 TableOfSymbols::updateSymbol(s);
						 }
					 }
					 else{
						 s = Symbol(op2, 0, Symbol::getLastSection());//value =0
						 FlinkEntry fe = FlinkEntry(WriterOfEverything::getCounterText(), 2, line, Symbol::getLastSection());
						 s.addFlink(fe);
						 TableOfSymbols::addSymbol(s);
					 }
					 imm = s.getValue()-4;
				 }
				 else{
					 imm = stoi(op2);
				 }

				 if (tip == 23 || tip == 24)
					 imm = 0;

				 if (pom1 == 16)
					 pom1 = 1;
				 else if (pom1 == 17)
					 pom1 = 2;

				 byte1 = tip * 4 + (pom1 >> 3);
				 std::stringstream ss;
				 ss << std::hex << byte1; // int decimal_value
				 std::string res1(ss.str());
				 res11 = fillString(res1);

				 int tmp1 = imm >> 16, tmp2 = pom1 & 7;

				 byte2 = (imm >> 16) + ((pom1 & 7) << 5);
				 std::stringstream ss2;
				 ss2 << std::hex << byte2;
				 std::string res2(ss2.str());
				 res22 = fillString(res2);

				 byte3 = (imm >> 8) & 255;
				 std::stringstream ss3;
				 ss3 << std::hex << byte3;
				 std::string res3(ss3.str());
				 res33 = fillString(res3);

				 byte4 = (imm & 255);
				 std::stringstream ss4;
				 ss4 << std::hex << byte4;
				 std::string res4(ss4.str());
				 res44 = fillString(res4);

				 return true;
				 break;
	}
	case 21:{
				//push lc
				imm = 0;
				int pom1 = Line::getLC();
				//upis

				byte1 = 23 * 4 + (pom1 >> 3);
				std::stringstream ss;
				ss << std::hex << byte1; // int decimal_value
				std::string res1(ss.str());
				res11 = fillString(res1);

				int tmp1 = imm >> 16, tmp2 = pom1 & 7;

				byte2 = (imm >> 16) + ((pom1 & 7) << 5);
				std::stringstream ss2;
				ss2 << std::hex << byte2;
				std::string res2(ss2.str());
				res22 = fillString(res2);

				res33 = "00";
				res44 = "00";


			 return true;
			 break;
	}
	case 22:{ //ret iret jmp
				iss2 >> op1;
				string symbolName = "";
				Symbol s;
				int tor = 1; //tip relokacije

				//ret
				if (!name.compare("ret")){
					type = 0;
					//pop pc
					int pom1 = 1;
					imm = 0;

					//upis

					byte1 = 24 * 4 + (pom1 >> 3);
					std::stringstream ss;
					ss << std::hex << byte1; // int decimal_value
					std::string res1(ss.str());
					res11 = fillString(res1);

					int tmp1 = imm >> 16, tmp2 = pom1 & 7;

					byte2 = (imm >> 16) + ((pom1 & 7) << 5);
					std::stringstream ss2;
					ss2 << std::hex << byte2;
					std::string res2(ss2.str());
					res22 = fillString(res2);

					res33 = "00";
					res44 = "00";

					//kraj upisa

				}
				//iret
				else if (!name.compare("iret")){
					type = 1;
					//pop psw
					imm = 0;
					int pom1 = stoi(op1);
					pom1 = 2;

					//upis

					byte1 = 24 * 4 + (pom1 >> 3);
					std::stringstream ss;
					ss << std::hex << byte1; // int decimal_value
					std::string res1(ss.str());
					res11 = fillString(res1);

					int tmp1 = imm >> 16, tmp2 = pom1 & 7;

					byte2 = (imm >> 16) + ((pom1 & 7) << 5);
					std::stringstream ss2;
					ss2 << std::hex << byte2;
					std::string res2(ss2.str());
					res22 = fillString(res2);

					res33 = "00";
					res44 = "00";

					//kraj upisa

					//pop pc
					pom1 = 1;

					//upis

					byte1 = 24 * 4 + (pom1 >> 3);
					std::stringstream ss33;
					ss33 << std::hex << byte1; // int decimal_value
					std::string res133(ss33.str());
					res11p = fillString(res133);

					byte2 = (imm >> 16) + ((pom1 & 7) << 5);
					std::stringstream ss233;
					ss233 << std::hex << byte2;
					std::string res233(ss233.str());
					res22p = fillString(res233);

					res33p = "00";
					res44p = "00";

					doubleInstruction = true;
					//kraj upisa
				}
				//jmp
				else if (!name.compare("jmp")){
					string sub = "";
					sub.assign(op1);
					if (op1.at(0) == '['){
						type = 3;
						if (op1.at(op1.length() - 1) != ']'){
							WriterOfEverything::setError("Greska u adresiranju kod jmp instrukcije.");
							return false;
						}
						sub.clear();
						sub.assign(op1.substr(1, op1.length() - 2));
						if (!isNumber(sub)){
							symbolName.assign(op1.substr(1, op1.length() - 2));
							tor = 6;
						}
						else
							imm = stoi(sub);
					}
					else{
						type = 2;
						if (!isNumber(op1)){
							symbolName.assign(op1.substr(1, op1.length() - 1));
						}
						else
							imm = stoi(sub);
					}

					//skok i ostalo
					tor = 1;
					if (!isNumber(sub)){
						//sredi za labelu
						if (TableOfSymbols::findSymbol(Symbol(sub))){
							s = TableOfSymbols::getSymbol(sub);

							if (s.isDefined()){
								int sCheck = 0;
								if (Symbol::getLastSection() > 0){
									sCheck = Symbol::getLastSection() - 1;
								}
								if (s.getSection() == sCheck){

								}
								else{
									FlinkEntry fe = FlinkEntry(WriterOfEverything::getCounterText(), tor, line, Symbol::getLastSection());
									s.addFlink(fe);
									TableOfSymbols::updateSymbol(s);
								}
							}
							else{
								FlinkEntry fe = FlinkEntry(WriterOfEverything::getCounterText(), tor, line, Symbol::getLastSection());
								s.addFlink(fe);
								TableOfSymbols::updateSymbol(s);
							}
						}
						else{
							s = Symbol(sub, 0, Symbol::getLastSection());//value =0
							FlinkEntry fe = FlinkEntry(WriterOfEverything::getCounterText(), tor, line, Symbol::getLastSection());
							s.addFlink(fe);
							TableOfSymbols::addSymbol(s);
						}
						imm = s.getValue();
					}
					else{
						
					}

//					imm = Symbol.getValue();
					//kraj skoka

					byte1 = tip * 4 + type;
					std::stringstream ss;
					ss << std::hex << byte1; // int decimal_value
					std::string res1(ss.str());
					res11 = fillString(res1);

					byte2 = imm >> 16;
					std::stringstream ss2;
					ss2 << std::hex << byte2;
					std::string res2(ss2.str());
					res22 = fillString(res2);

					byte3 = (imm >> 8) & 255;
					std::stringstream ss3;
					ss3 << std::hex << byte3;
					std::string res3(ss3.str());
					res33 = fillString(res3);

					byte4 = (imm & 255);
					std::stringstream ss4;
					ss4 << std::hex << byte4;
					std::string res4(ss4.str());
					res44 = fillString(res4);

				}
		
				return true;
				break;
	}

	case 31:{
				performLdcLh(tip, line);
				return true;
				break;
	}
	case 32:{
				string toPerform="";
				doubleInstruction = true;
				toPerform.append(line);
				toPerform.insert(3, "h");
				performLdcLh(tip, toPerform);
				WriterOfEverything::incCounterText(9);
				toPerform.clear();
				toPerform.assign(line);
				toPerform.insert(3, "l");
				performLdcLh(tip, toPerform);
				return true;
				break;
	}
	default:{
				return false;
				break;
	}
	}
}

void Instruction::performLdcLh(int tip, string line){
	//ovo radi za neposrednu vrednost, sredi za labelu
	int byte1;
	int byte2;
	int byte3;
	int byte4;
	int tor = 5; //type of relocation

	istringstream iss2(line);
	string word;
	iss2 >> word; //predjemo naziv instrukcije
	Symbol s;
	iss2 >> op1;
	int pom1;
	pom1 = stoi(op1);

	iss2 >> op2;
	int pom2=0; // pom2 je c

	if (!word.compare("ldch")){
		tor = 4;
	}

	if (!isNumber(op2)){
		//sredi za labelu
		if (TableOfSymbols::findSymbol(Symbol(op2))){
			s = TableOfSymbols::getSymbol(op2);

			if (s.isDefined()){
				int sCheck = 0;
				if (Symbol::getLastSection() > 0){
					sCheck = Symbol::getLastSection() - 1;
				}
				//nema provere da li je u istoj sekciji, zato sto je apsolutni skok!
				FlinkEntry fe = FlinkEntry(WriterOfEverything::getCounterText(), tor, line, Symbol::getLastSection());
				s.addFlink(fe);
				TableOfSymbols::updateSymbol(s);
				
			}
			else{
				FlinkEntry fe = FlinkEntry(WriterOfEverything::getCounterText(), tor, line, Symbol::getLastSection());
				s.addFlink(fe);
				TableOfSymbols::updateSymbol(s);
			}
		}
		else{
			s = Symbol(op2, 0, Symbol::getLastSection());//value =0
			FlinkEntry fe = FlinkEntry(WriterOfEverything::getCounterText(), tor, line, Symbol::getLastSection());
			s.addFlink(fe);
			TableOfSymbols::addSymbol(s);
		}
		pom2 = s.getValue();
	}
	else{
		pom2 = stoi(op2);
	}

	int tip2 = 31;
	int hl = 0; //high - 1 low -0

	if (!word.compare("ldch")){
		hl = 1;
	}

	byte1 = tip2 * 4 + (pom1 >> 2);
	std::stringstream ss;
	ss << std::hex << byte1; // int decimal_value
	std::string res1(ss.str());
	res11 = fillString(res1);

	byte2 = ((pom1 & 3) << 6) + (hl << 5) + ((pom2 >> 11) & 31);
	std::stringstream ss2;
	ss2 << std::hex << byte2;
	std::string res2(ss2.str());
	res22 = fillString(res2);

	byte3 = (pom2 >> 3) & 255;
	std::stringstream ss3;
	ss3 << std::hex << byte3;
	std::string res3(ss3.str());
	res33 = fillString(res3);

	byte4 = (pom2 & 7) << 5;
	std::stringstream ss4;
	ss4 << std::hex << byte4;
	std::string res4(ss4.str());
	res44 = fillString(res4);

	//ako je ldcl upisemo i u pomocne promenljive
	if (!word.compare("ldch")){
		res11p = fillString(res1);
		res22p = fillString(res2);
		res33p = fillString(res3);
		res44p = fillString(res4);
	}

}

string Instruction::writeInstruction(){
	string s;
	s.append(res11);
	s.append(res22);
	s.append(res33);
	s.append(res44);
	s.append(" ");

	if (doubleInstruction){
		s.append(res11p);
		s.append(res22p);
		s.append(res33p);
		s.append(res44p);
		s.append(" ");
	}

	WriterOfEverything::appendText(s);
	return s;
}
