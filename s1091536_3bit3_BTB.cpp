#include<iostream>
#include<string>
#include<queue>
#include<map>
#include<sstream>

using namespace std;

string IN; // each input inst
int Rs1 = 0, Rs2 = 0, Rd = 0, Im = 0;
string Rd_Lable;
int entry_num = 4, entry = 0, inst_num = 0;
int  PC = 0, next_PC = 0;
//queue<bool>history; // 3 bits history
int history = 0;
map<string, int>Lables; // Lable=>address
struct BTB
{
	int his = 0;
	string BC[8] = { "SN","SN", "SN", "SN", "SN", "SN", "SN", "SN" };
	int misprediction = 0;
	bool flag = false;
};
vector<BTB>BTBs; // depend on numbers of entrys
struct Inst
{
	int address = 0;
	string type;
	int rs1 = 0, rs2 = 0, rd = 0, im = 0;
	string rd_Lable;
};
vector<Inst>Insts; // all insts

int REG[32] = {};

void fetch(string);     // storing insts
char predict();         // predict T or N
char outcome();         // run inst & show T or N
void print();           // print BTBs
void update(char,char); // update BTB


void fetch(string IN)
{
	stringstream ss(IN);
	string s;
	Inst temp_inst;
	temp_inst.address = inst_num;
	ss >> s;
	//cout << s;
	if (s[0] != 'a' && s[0] != 'b')
	{
		s.erase(s.end() - 1);
		Lables[s] = inst_num;
		inst_num--;
		//ss >> s;
	}
	//else if (s == "End")
	//{
	//	Lables[s] = inst_num;
	//	return;
	//}
	else
	{
		temp_inst.type = s;
		char C;
		int N = 0;
		if (s == "addi") //addi r2 ,r2 ,-1
		{
			ss >> C >> N;
			//cout << N << ' ';
			temp_inst.rd = N;
			//==================
			ss >> C >> C >> N;
			//cout << N << ' ';
			temp_inst.rs1 = N;
			//==================
			ss >> C >> N;
			//cout << N << '\n';
			temp_inst.im = N;
		}
		else // s == "beq"  //beq r1 ,r2 ,End
		{
			ss >> C >> N;
			//cout << N << ' ';
			temp_inst.rs1 = N;
			//==================
			ss >> C >> C >> N;
			//cout << N << ' ';
			temp_inst.rs2 = N;
			//==================
			ss >> s;
			//cout << s << '\n';
			s.erase(s.begin());
			temp_inst.rd_Lable = s;
		}
		Insts.push_back(temp_inst);
	}
}

char P, O; // predict & outcome
char predict()
{
	BTBs[entry].flag = true;
	int H = BTBs[entry].his;
	if (BTBs[entry].BC[H] == "SN" || BTBs[entry].BC[H] == "WN")
		return 'N';
	else return 'T';
}

char outcome()
{
	
	if (Insts[PC].type == "addi")
	{
		REG[Insts[PC].rd] = REG[Insts[PC].rs1] + Insts[PC].im;
		next_PC = PC + 1;
		return 'N';
	}
	else // inst == "beq"
	{
		if (REG[Insts[PC].rs1] == REG[Insts[PC].rs2])
			next_PC = Lables[Insts[PC].rd_Lable];
		else next_PC = PC + 1;
	}
	if (PC + 1 != next_PC){
		return 'T';
	}	
	else {
		return 'N';
	}
}

void Bit2(int N)
{
	if (N >= 4)
	{
		cout << '1';
		N -= 4;
	}
	else cout << '0';
	if (N >= 2)
	{
		cout << '1';
		N -= 2;
	}
	else cout << '0';
	cout << N;
}

void print()
{
	Inst Temp = Insts[PC];
	cout << "entry: " << entry << "      ";
	if (Temp.type == "addi")
	{
		cout << "addi " << "R" << Temp.rd << " ,R" << Temp.rs1 << " ," << Temp.im << '\n';
	}
	else
	{
		cout << "beq " << "R" << Temp.rs1 << " ,R" << Temp.rs2 << " ," << Temp.rd_Lable << '\n';
	}
	cout << "prediction: " << P << '\n';
	cout << "outcome   : " << O << '\n';

	if (P == O)
		cout << "Prediction success\n";
	else
		cout << "Prediction fail\n";

	cout << "misprediction: " << BTBs[entry].misprediction << '\n';

	for (int i = 0; i < entry_num; i++)
	{
		cout <<i<< ". ( ";
		Bit2(BTBs[i].his);
		for (int j = 0; j < 8; j++)
			cout << ", " << BTBs[i].BC[j];
		cout << " )";
		if (!BTBs[i].flag)
			cout << " Entry has never been used.";
		cout<<'\n';
	}
	cout << "=========================" << '\n';
}

void update(char P, char O)
{
	//history.push(O);
	//history.pop();
	history = BTBs[entry].his;
	string TN = BTBs[entry].BC[history];
	if (TN == "SN")
		O == 'T' ? TN = "WN" : TN = TN;
	else if (TN == "WN")
		O == 'T' ? TN = "WT" : TN = "SN";
	else if (TN == "WT")
		O == 'T' ? TN = "ST" : TN = "WN";
	else if (TN == "ST")
		O == 'T' ? TN = TN : TN = "WT";
	BTBs[entry].BC[history] = TN;
	// finish update BC
	history = (history % 4) * 2;  // update 3-bit
	if (O == 'T') history++;      // update 3-bit
	BTBs[entry].his = history;    // update 3-bit
}

int main()
{
	cout << "Enter entry number:(must be 2,4,8,16,....)\n";
	cin >> entry_num;
	cin.ignore();
	cout << "Enter assembly code:(finish by EOF)\n";
	inst_num = 0;
	while (getline(cin, IN)) {
		fetch(IN);
		inst_num++;
	}
	cout << "Insts num: " << inst_num << '\n'<<
		"========= Start =========" << '\n';
	// Run
	for (int i = 0; i < entry_num; i++)
	{
		BTB temp;
		BTBs.push_back(temp);
	}
	//for (int i = 0; i < entry_num; i++) // for 3-bits his. == 000
	//	history.push(0);
	while (PC < inst_num)
	{
		P = predict();
		O = outcome();
		if (P != O) BTBs[entry].misprediction++;
		print();
		update(P, O);
		PC = next_PC;
		entry = PC % entry_num;
	}
	cout << "Finish!\n";
	return 1;
}

/* test
addi r1 ,r0 ,0
addi r2 ,r1 ,10
Loop:
beq r1 ,r2 ,End
addi r2 ,r2 ,-1
beq r0 ,r0 ,Loop
End:
*/
