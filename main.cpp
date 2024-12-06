#include<iostream>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include<exception>
#include"RF.h"
using namespace std;

int LOAD_UNITS, LOAD_UNITS_CYCLES; 
int STORE_UNITS, STORE_UNITS_CYCLES;
int BEQ_UNITS, BEQ_UNITS_CYCLES;
int CALL_UNITS, CALL_UNITS_CYCLES;
int ADD_UNITS, ADD_UNITS_CYCLES; 
int NAND_UNITS, NAND_UNITS_CYCLES;
int MUL_UNITS, MUL_UNITS_CYCLES;

int DM[128];
int Clock;
int PC;
vector<reservationStation> loadStation;
vector<reservationStation> storeStation;
vector<reservationStation> beqStation;
vector<reservationStation> callStation;
vector<reservationStation> addStation;
vector<reservationStation> nandStation;
vector<reservationStation> mulStation;

struct Instruction
{
    string Op;
    int dest;
    int src1;
    int src2;
};
vector<Instruction> Instructions;

void Parser(string& instructionsFile)
{
    unordered_map<string,int> Labels;
    /*
    LW rd, imm(rs1)
    SW rs2, imm(rs1)
    BEQ rs1, rs2, imm
    JAL rs1, Label/imm
    ADD rd, rs1, rs2
    ADDI rs, rs1, imm
    NAND rd, rs1, rs2
    MUL rd, rs1, rs2
    */
    ifstream instStream(instructionsFile);
    if (!instStream.is_open())
    {
        cerr << "Error opening the instructions file " << instructionsFile << endl;
        return;
    }

    string line;
    int position = 0;
    //Parse for labels
    while(getline(instStream, line))
    {
        string temp;
        istringstream iss(line);
        if (!(iss >> temp))
            continue;

        if(temp != "LW" && temp != "SW" && temp != "BEQ" && temp != "JAL"
            && temp != "ADD" && temp != "ADDI" && temp != "NAND" && temp != "MUL")
        {
            Labels[temp] = position;
        }
        else
            ++position;
    }

    instStream.clear();
    instStream.seekg(0, ios::beg);  
    while (getline(instStream, line))
    {
        istringstream iss(line);
        Instruction inst;
        if (!(iss >> inst.Op))
            continue;

        if (inst.Op == "LW")
        {
            // LW rd, imm(rs1)
            string rd, address;
            if (iss >> rd >> address) // Parse rd and imm(rs1)
            {
                size_t leftParen = address.find('(');
                size_t rightParen = address.find(')');

                if (leftParen != string::npos && rightParen != string::npos && rightParen > leftParen)
                {
                    int imm = stoi(address.substr(0, leftParen)); // Extract immediate
                    string rs1 = address.substr(leftParen + 1, rightParen - leftParen - 1); // Extract rs1

                    inst.dest = stoi(rd.substr(1));   // Extract register number from "rX"
                    inst.src1 = stoi(rs1.substr(1));  // Extract register number from "rX"
                    inst.src2 = imm;                  // Store immediate in src2
                }
                else
                {
                    cerr << "Error parsing LW instruction: " << line << endl;
                    continue;
                }
            }
            else
            {
                cerr << "Error parsing LW instruction: " << line << endl;
                continue;
            }
        }
        else if (inst.Op == "SW")
        {
            // SW rs2, imm(rs1)
            string rs2, address;
            if (iss >> rs2 >> address) // Parse rs2 and imm(rs1)
            {
                size_t leftParen = address.find('(');
                size_t rightParen = address.find(')');

                if (leftParen != string::npos && rightParen != string::npos && rightParen > leftParen)
                {
                    int imm = stoi(address.substr(0, leftParen)); // Extract immediate
                    string rs1 = address.substr(leftParen + 1, rightParen - leftParen - 1); // Extract rs1

                    inst.src2 = stoi(rs2.substr(1));  // Extract register number from "rX"
                    inst.src1 = stoi(rs1.substr(1));  // Extract register number from "rX"
                    inst.dest = imm;                  // Use dest to store the immediate value
                }
                else
                {
                    cerr << "Error parsing SW instruction: " << line << endl;
                    continue;
                }
            }
            else
            {
                cerr << "Error parsing SW instruction: " << line << endl;
                continue;
            }
        }

        else if (inst.Op == "BEQ")
        {
            // BEQ rs1, rs2, imm
            int imm;
            string rs1, rs2;

            if (iss >> rs1 >> rs2 >> imm)
            {
                inst.src1 = stoi(rs1.substr(1)); 
                inst.src2 = stoi(rs2.substr(1)); 
                inst.dest = imm; // Use dest to store the branch target
            }
            else
            {
                cerr << "Error parsing BEQ instruction: " << line << endl;
                continue;
            }
        }
        else if (inst.Op == "JAL")
        {
            // JAL rs1, Label/imm
            string immOrLabel;
            string rs1;

            if (iss >> rs1 >> immOrLabel)
            {
                inst.src1 = stoi(rs1.substr(1));

                // Check if the target is a label or immediate value
                if (Labels.find(immOrLabel) != Labels.end())
                {
                    inst.dest = Labels[immOrLabel];
                }
                else
                {
                    inst.dest = stoi(immOrLabel);
                }
                inst.src2 = -1;
            }
            else
            {
                cerr << "Error parsing JAL instruction: " << line << endl;
                continue;
            }
        }
        else if (inst.Op == "ADD" || inst.Op == "NAND" || inst.Op == "MUL")
        {
            // ADD/NAND/MUL rd, rs1, rs2
            string rd, rs1, rs2;

            if (iss >> rd >> rs1 >> rs2)
            {
                inst.dest = stoi(rd.substr(1)); // Extract register number from "rX"
                inst.src1 = stoi(rs1.substr(1)); // Extract register number from "rX"
                inst.src2 = stoi(rs2.substr(1)); // Extract register number from "rX"
            }
            else
            {
                cerr << "Error parsing " << inst.Op << " instruction: " << line << endl;
                continue;
            }
        }
        else if (inst.Op == "ADDI")
        {
            // ADDI rs, rs1, imm
            int imm;
            string rd, rs1;

            if (iss >> rd >> rs1 >> imm)
            {
                inst.dest = stoi(rd.substr(1));  // Extract register number from "rX"
                inst.src1 = stoi(rs1.substr(1)); // Extract register number from "rX"
                inst.src2 = imm;                 // Use src2 to store the immediate value
            }
            else
            {
                cerr << "Error parsing ADDI instruction: " << line << endl;
                continue;
            }
        }
        else
            continue;

        // Add the successfully parsed instruction to the list
        Instructions.push_back(inst);
    }

    instStream.close();
}

int main() {
    // Create a sample instruction file
    string fileName = "instructions.txt";

    // Parse the file
    Parser(fileName);

    // Print the parsed instructions
    cout << "Parsed Instructions:\n";
    for (size_t i = 0; i < Instructions.size(); i++) {
        const Instruction& inst = Instructions[i];
        cout << i << ": " << inst.Op << " "
             << "dest=" << inst.dest << ", "
             << "src1=" << inst.src1 << ", "
             << "src2=" << inst.src2 << endl;
    }

    return 0;
}
  