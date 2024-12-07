#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <exception>
#include "ReservationStation.h"
#include <vector>
#include <algorithm>

using namespace std;

#define REGISTER_NUM 16

// Assuming ROBEntry and reservationStation are declared elsewhere in the project
// Example:
// struct ROBEntry {...};
// struct reservationStation {...};

struct Reg {
    ROBEntry* Qi = nullptr;
    int value = 0;
    bool busy = false;
};
vector<Reg> RF(REGISTER_NUM);

unordered_map<string, vector<reservationStation>*> Stations;
unordered_map<string, int> Stations_Time;

struct Instruction {
    string Op;
    int dest;
    int src1;
    int src2;
};
vector<Instruction> Instructions;

void Parser(string& instructionsFile) {
    unordered_map<string, int> Labels;
    ifstream instStream(instructionsFile);
    if (!instStream.is_open()) {
        cerr << "Error opening the instructions file " << instructionsFile << endl;
        return;
    }

    string line;
    int position = 0;
    
    // Parse for labels
    while (getline(instStream, line)) {
        string temp;
        istringstream iss(line);
        if (!(iss >> temp)) continue;

        if (temp != "LW" && temp != "SW" && temp != "BEQ" && temp != "JAL"
            && temp != "ADD" && temp != "ADDI" && temp != "NAND" && temp != "MUL") {
            Labels[temp] = position;
        } else {
            ++position;
        }
    }

    instStream.clear();
    instStream.seekg(0, ios::beg);
    
    // Parse instructions
    while (getline(instStream, line)) {
        istringstream iss(line);
        Instruction inst;
        if (!(iss >> inst.Op)) continue;

        if (inst.Op == "LW") {
            string rd, address;
            if (iss >> rd >> address) {
                size_t leftParen = address.find('(');
                size_t rightParen = address.find(')');
                if (leftParen != string::npos && rightParen != string::npos && rightParen > leftParen) {
                    int imm = stoi(address.substr(0, leftParen));  // Extract immediate
                    string rs1 = address.substr(leftParen + 1, rightParen - leftParen - 1); // Extract rs1
                    inst.dest = stoi(rd.substr(1));  // Register number
                    inst.src1 = stoi(rs1.substr(1));  // Register number
                    inst.src2 = imm;  // Immediate value
                } else {
                    cerr << "Error parsing LW instruction: " << line << endl;
                }
            }
        } else if (inst.Op == "SW") {
            string rs2, address;
            if (iss >> rs2 >> address) {
                size_t leftParen = address.find('(');
                size_t rightParen = address.find(')');
                if (leftParen != string::npos && rightParen != string::npos && rightParen > leftParen) {
                    int imm = stoi(address.substr(0, leftParen));  // Extract immediate
                    string rs1 = address.substr(leftParen + 1, rightParen - leftParen - 1); // Extract rs1
                    inst.src2 = stoi(rs2.substr(1));  // Register number
                    inst.src1 = stoi(rs1.substr(1));  // Register number
                    inst.dest = imm;  // Immediate value
                } else {
                    cerr << "Error parsing SW instruction: " << line << endl;
                }
            }
        } else if (inst.Op == "BEQ") {
            int imm;
            string rs1, rs2;
            if (iss >> rs1 >> rs2 >> imm) {
                inst.src1 = stoi(rs1.substr(1));  // Register number
                inst.src2 = stoi(rs2.substr(1));  // Register number
                inst.dest = imm;  // Branch target
            } else {
                cerr << "Error parsing BEQ instruction: " << line << endl;
            }
        } else if (inst.Op == "JAL") {
            string immOrLabel;
            string rs1;
            if (iss >> rs1 >> immOrLabel) {
                inst.src1 = stoi(rs1.substr(1));  // Register number
                if (Labels.find(immOrLabel) != Labels.end()) {
                    inst.dest = Labels[immOrLabel];  // Label
                } else {
                    inst.dest = stoi(immOrLabel);  // Immediate value
                }
                inst.src2 = -1;  // Not used
            } else {
                cerr << "Error parsing JAL instruction: " << line << endl;
            }
        } else if (inst.Op == "ADD" || inst.Op == "NAND" || inst.Op == "MUL") {
            string rd, rs1, rs2;
            if (iss >> rd >> rs1 >> rs2) {
                inst.dest = stoi(rd.substr(1));  // Register number
                inst.src1 = stoi(rs1.substr(1));  // Register number
                inst.src2 = stoi(rs2.substr(1));  // Register number
            } else {
                cerr << "Error parsing " << inst.Op << " instruction: " << line << endl;
            }
        } else if (inst.Op == "ADDI") {
            int imm;
            string rd, rs1;
            if (iss >> rd >> rs1 >> imm) {
                inst.dest = stoi(rd.substr(1));  // Register number
                inst.src1 = stoi(rs1.substr(1));  // Register number
                inst.src2 = imm;  // Immediate value
            } else {
                cerr << "Error parsing ADDI instruction: " << line << endl;
            }
        }
        Instructions.push_back(inst);
    }
}
