#include <iostream>
#include <cstring>

using namespace std;

char* formatHex(char* hex) {
    char* formatted = new char[8];
    int m = 0;
    for (int i = 0; i < 8; i++) {
        if (hex[i + m + 2] == '_') {
            m++;
            i--;
            continue;
        }
        formatted[i] = hex[i + m + 2];
    }
    return formatted;
}

char* hexToBin(char* hex) {
    char *binary = new char[32];

    for (int i = 0; i < 8; i++) {
        const char *bin;
        switch (hex[i]) {
            case '0':
                bin = "0000";
                break;
            case '1':
                bin = "0001";
                break;
            case '2':
                bin = "0010";
                break;
            case '3':
                bin = "0011";
                break;
            case '4':
                bin = "0100";
                break;
            case '5':
                bin = "0101";
                break;
            case '6':
                bin = "0110";
                break;
            case '7':
                bin = "0111";
                break;
            case '8':
                bin = "1000";
                break;
            case '9':
                bin = "1001";
                break;
            case 'A':
                bin = "1010";
                break;
            case 'B':
                bin = "1011";
                break;
            case 'C':
                bin = "1100";
                break;
            case 'D':
                bin = "1101";
                break;
            case 'E':
                bin = "1110";
                break;
            case 'F':
                bin = "1111";
                break;
            default:
                std::cout << "Invalid argument 2" << std::endl;
                return nullptr;
        }
        for (int j = 0; j < 4; j++)
        {
            binary[i * 4 + j] = bin[j];
        }
    }
    return binary;
}

int binToDec(char *bin) {
    int dec = 0;
    int size = std::strlen(bin);
    for (int i = 0; i < size; i++) {
        dec += (bin[i] - '0') << (size - i - 1);
    }
    return dec;
}

void printInitialInfo(char* hex, char* binary, char* opcode, int opcodeInt) {
    std::cout << "Hex    : " << hex << std::endl;
    std::cout << "Bin    : " << binary << std::endl;
    std::cout << "Opcode : " << opcode << " (" << opcodeInt << ")" << std::endl;
}

void getImm(int ref, string bin, char* imm) {
    if (ref == 0 || ref == 1 || ref == 7) {
        std::strcpy(imm, bin.substr(0, 12).c_str());
    }   else if (ref == 3) {
        std::string immStr = bin.substr(0, 7) + bin.substr(20, 5);
        std::strcpy(imm, immStr.c_str());
    }   else if (ref == 6) {
        std::string immStr = bin.substr(0,1) + bin.substr(24,1) + bin.substr(1, 6) + bin.substr(20, 4) + '0';
        std::strcpy(imm, immStr.c_str());
    }   else if (ref == 2 || ref == 5) {
        std::string immStr = bin.substr(0, 20) + "000000000000";
        std::strcpy(imm, immStr.c_str());
    }   else if (ref == 8) {
        std::string immStr = bin.substr(0,1) + bin.substr(12,8) + bin.substr(11,1) + bin.substr(1,10) + "0";
        std::strcpy(imm, immStr.c_str());
    }
}

int getRefFromOp(int opcode) {
    switch (opcode)  {
        case 3: // 0000011 - I-type load
            return 0;
        case 19: // 0010011 - I-type arithmetic/logical
            return 1;
        case 23: // 0110011 - U-type auipc
            return 2;
        case 35: // 0100011 - S-type store
            return 3;
        case 51: // 1100111 - R-type
            return 4;
        case 55: // 1101111 - U-type lui
            return 5;
        case 99: // 1100011 - B-type branch
            return 6;
        case 103: // 1100111 - I-type jalr
            return 7;
        case 111: // 1101111 - J-type jal
            return 8;
        default:
            std::cout << "Invalid opcode" << std::endl;
            return -1;
    }
}

int main(int argc, char **argv) {
    
    char* hex = formatHex(argv[1]);
    
    if (strlen(hex) != 8) {
        std::cout << "Invalid argument" << std::endl;
        return 1;
    }

    char* binary = hexToBin(hex);
    if (binary == nullptr) {
        std::cout << "Invalid argument" << std::endl;
        return 1;
    }
    
    string binaryStr(binary);
    char *opcode = new char[8];   
    strcpy(opcode, binaryStr.substr(25, 7).c_str());
    
    int opcodeInt = binToDec(opcode);

    int ref = getRefFromOp(opcodeInt);
    if (ref == -1) {
        return 1;
    }
    
    printInitialInfo(hex, binary, opcode, opcodeInt);

    int funct3Int;
    int funct7Int;
    int rs2Int;
    int rs1Int;
    int rdInt;
    int immInt;
    char* funct3 = new char[3];
    char* funct7 = new char[7];
    char* rs2 = new char[5];
    char* rs1 = new char[5];
    char* rd = new char[5];
    char* imm;

    string instruction = "";

    if (ref == 4) {                                 // R-Type
        strcpy(funct3, binaryStr.substr(17, 3).c_str());
        strcpy(funct7, binaryStr.substr(0, 7).c_str());
        strcpy(rs2, binaryStr.substr(7, 5).c_str());
        strcpy(rs1, binaryStr.substr(12, 5).c_str());
        strcpy(rd, binaryStr.substr(20, 5).c_str());

        funct3Int = binToDec(funct3);
        funct7Int = binToDec(funct7);
        rs2Int = binToDec(rs2);
        rs1Int = binToDec(rs1);
        rdInt = binToDec(rd);

        cout << "funct3 : " << funct3 << " (" << funct3Int << ")" << endl;
        cout << "funct7 : " << funct7 << " (" << funct7Int << ")" << endl;
        cout << "rs1    : " << rs1 << " (" << rs1Int << ")" << endl;
        cout << "rs2    : " << rs2 << " (" << rs2Int<< ")" << endl;
        cout << "rd     : " << rd << " (" << rdInt << ")" << endl;

        if (funct3Int == 0) {
            if (funct7Int == 0) {
                instruction += "add";
            }   else if (funct7Int == 32) {
                instruction += "sub";
            }
        }   else if (funct3Int == 1) {
            instruction += "sll";
        }   else if (funct3Int == 2) {
            instruction += "slt";
        }   else if (funct3Int == 3) {
            instruction += "sltu";
        }   else if (funct3Int == 4) {
            instruction += "xor";
        }   else if (funct3Int == 5) {
            if (funct7Int == 0) {
                instruction += "srl";
            }   else if (funct7Int == 32) {
                instruction += "sra";
            }
        }   else if (funct3Int == 6) {
            instruction += "or";
        }   else if (funct3Int == 7) {
            instruction += "and";
        }

        instruction += " x" + to_string(rdInt) + ", x" + to_string(rs1Int) + ", x" + to_string(rs2Int);
        
    }   else if (ref == 0 || ref == 1 || ref == 7) {// I-Type
        imm = new char[12];

        strcpy(funct3, binaryStr.substr(17, 3).c_str());
        strcpy(rs1, binaryStr.substr(12, 5).c_str());
        strcpy(rd, binaryStr.substr(20, 5).c_str());
        strcpy(imm, binaryStr.substr(0, 12).c_str());
        getImm(ref, binaryStr, imm);

        funct3Int = binToDec(funct3);
        rs1Int = binToDec(rs1);
        rdInt = binToDec(rd);
        immInt = binToDec(imm);

        cout << "funct3 : " << funct3 << " (" << funct3Int << ")" << endl;
        cout << "rs1    : " << rs1 << " (" << rs1Int << ")" << endl;
        cout << "rd     : " << rd << " (" << rdInt << ")" << endl;
        cout << "imm    : " << imm << " (" << immInt << ")" << endl;

        if (ref == 0) {
            if (funct3Int == 0) {
                instruction += "lb";
            }   else if (funct3Int == 1) {
                instruction += "lh";
            }   else if (funct3Int == 2) {
                instruction += "lw";
            }   else if (funct3Int == 4) {
                instruction += "lbu";
            }   else if (funct3Int == 5) {
                instruction += "lhu";
            }
        }   else if (ref == 1) {
            if (funct3Int == 0) {
                instruction += "addi";
            }   else if (funct3Int == 1) {
                instruction += "slli";
            }   else if (funct3Int == 2) {
                instruction += "slti";
            }   else if (funct3Int == 3) {
                instruction += "sltiu";
            }   else if (funct3Int == 4) {
                instruction += "xori";
            }   else if (funct3Int == 5) {
                if (true)                       // FIXME:
                {}
            }   else if (funct3Int == 6) {
                instruction += "ori";
            }   else if (funct3Int == 7) {
                instruction += "andi";
            }
        }   else if (ref == 7) {
            instruction += "jalr";
        }
        if (ref == 0) {
            instruction += " x" + to_string(rdInt) + ", " + to_string(immInt) + "(x" + to_string(rs1Int) + ")";
        }   else if (ref == 7 || ref == 1) {
            instruction += " x" + to_string(rdInt) + ", x" + to_string(rs1Int) + ", " + to_string(immInt);
        }
    }   else if (ref == 3) {                        //S-Type
        imm = new char[12];

        strcpy(funct3, binaryStr.substr(17, 3).c_str());
        strcpy(rs2, binaryStr.substr(7, 5).c_str());
        strcpy(rs1, binaryStr.substr(12, 5).c_str());
        getImm(ref, binaryStr, imm);

        funct3Int = binToDec(funct3);
        rs2Int = binToDec(rs2);
        rs1Int = binToDec(rs1);
        immInt = binToDec(imm);

        cout << "funct3 : " << funct3 << " (" << funct3Int << ")" << endl;
        cout << "rs1    : " << rs1 << " (" << rs1Int << ")" << endl;
        cout << "rs2    : " << rs2 << " (" << rs2Int << ")" << endl;
        cout << "imm    : " << imm << " (" << immInt << ")" << endl;

        if (funct3Int == 0) {
            instruction += "sb";
        }   else if (funct3Int == 1) {
            instruction += "sh";
        }   else if (funct3Int == 2) {
            instruction += "sw";
        }

        instruction += " x" + to_string(rs2Int) + ", " + to_string(immInt) + "(x" + to_string(rs1Int) + ")";
    }   else if (ref == 6) {                        //B-Type
        imm = new char[12];

        strcpy(funct3, binaryStr.substr(17, 3).c_str());
        strcpy(rs2, binaryStr.substr(7, 5).c_str());
        strcpy(rs1, binaryStr.substr(12, 5).c_str());
        getImm(ref, binaryStr, imm);

        funct3Int = binToDec(funct3);
        rs2Int = binToDec(rs2);
        rs1Int = binToDec(rs1);
        immInt = binToDec(imm);

        cout << "funct3   : " << funct3 << " (" << funct3Int << ")" << endl;
        cout << "rs1      : " << rs1 << " (" << rs1Int << ")" << endl;
        cout << "rs2      : " << rs2 << " (" << rs2Int << ")" << endl;
        cout << "imm      : " << imm << " (" << immInt << ")" << endl;

        if (funct3Int == 0) {
            instruction += "beq";
        }   else if (funct3Int == 1) {
            instruction += "bne";
        }   else if (funct3Int == 4) {
            instruction += "blt";
        }   else if (funct3Int == 5) {
            instruction += "bge";
        }   else if (funct3Int == 6) {
            instruction += "bltu";
        }   else if (funct3Int == 7) {
            instruction += "bgeu";
        }

        instruction += " x" + to_string(rs2Int) +  ", x" + to_string(rs1Int) + + ", " + to_string(immInt);
    }   else if (ref == 2 || ref == 5) {            //U-Type
        imm = new char[32];

        strcpy(rd, binaryStr.substr(20, 5).c_str());
        getImm(ref, binaryStr, imm);

        rdInt = binToDec(rd);
        immInt = binToDec(imm);
        
        if (ref == 2) {
            instruction += "auipc";
        }   else if (ref == 5) {
            instruction += "lui";
        }

        instruction += " x" + to_string(rdInt) + ", " + to_string(immInt);

    }   else if (ref == 8) {                        //J-Type
        imm = new char[21];

        strcpy(rd, binaryStr.substr(20, 5).c_str());
        getImm(ref, binaryStr, imm);

        rdInt = binToDec(rd);
        immInt = binToDec(imm);

        cout << "rd     : " << rd << " (" << rdInt << ")" << endl;
        cout << "imm    : " << imm << " (" << immInt << ")" << endl;

        instruction += "jal x" + to_string(rdInt) + ", " + to_string(immInt);
    }    
        
    cout << "Instruction : " << instruction << endl;
    return 0;
}