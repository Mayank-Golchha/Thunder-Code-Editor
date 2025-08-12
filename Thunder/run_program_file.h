// TODO : include linkers from some file where a user can write the linkers dont pass as parameters

#ifndef RUN_PROGRAM_FILE_H
#define RUN_PROGRAM_FILE_H

#include <string>
#include <vector>
#include <filesystem>

#include "language_type.h"

class RunProgram{
    public:
        LANGUAGE_TYPE type;
        // std::string start_command = "start cmd.exe @cmd /k ";
        std::string start_command = "start cmd.exe /k \"";

        RunProgram(LANGUAGE_TYPE type_) : type(type_) {}

        std::string C_Program(std::string file_path){
            // start cmd.exe /k "cd /d \"D:\C Projects\" && gcc array.c -o array && array.exe"
            // g++ your_file.cpp -o output_name && ./output_name
            std::vector<std::string> linkers;
            std::filesystem::path file(file_path);
            std::string file_name = file.filename().string();
            // std::string command = "cd /d \"" + file.parent_path().string() + "\\\" && gcc " + file_name;
            std::string command = "cd /d \"" + file.parent_path().string() + "\\\" && g++ " + file_name;
            for (int i = 0;i<linkers.size();i++) command += " -"+ linkers[i];
            command += " -o " + file.stem().string() + " && .\\" + file.stem().string() + "\"";
            return command;
        }

        std::string Python_Program(std::string file_path){
            std::filesystem::path file(file_path);
            std::string file_name = file.filename().string();
            std::string command = "cd /d \"" + file.parent_path().string() + "\\\" && py " + file_name + "\"";
            return command;        
        } 

        void run(std::string file_path){
            switch (type){
                case LANGUAGE_TYPE::C:
                    start_command += C_Program(file_path);
                    break;
                case LANGUAGE_TYPE::PYTHON:
                    start_command += Python_Program(file_path);
                    std::cout << start_command;
                    break;

            }
            std::system(start_command.c_str());
        }
};


#endif