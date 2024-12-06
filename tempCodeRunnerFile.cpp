ofstream outFile(fileName);
    // if (!outFile.is_open()) {
    //     cerr << "Error creating the instruction file." << endl;
    //     return 1;
    // }

    // // Write sample instructions to the file
    // outFile << "LOOP\nADD r1, r2, r3\n";
    // outFile << "LW r4, 8(r1)\n";
    // outFile << "SW r5, 16(r2)\n";
    // outFile << "BEQ r1, r2, 12\n";
    // outFile << "JAL r0, LOOP\n";
    // outFile << "ADDI r6, r1, 42\n";
    // outFile << "NAND r7, r3, r4\n";
    // outFile << "MUL r8, r5, r6\n";
    // outFile.close();