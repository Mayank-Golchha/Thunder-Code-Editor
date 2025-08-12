// it is a logging system used for debugging
#include <string>
#include <fstream>
#include <ctime>

enum class LOG_TYPE{
    LOGGER_ERROR,
    LOGGER_INFO,
    LOGGER_WARNING
};


class Logger{
    public:
        Logger(std::string file_path){
            // log_file.open(file_path, std::ios::app);
            log_file.open(file_path);

            if (!log_file.is_open()){
                throw std::runtime_error("[ERROR] : LOGGER FILE NOT ABLE TO OPEN");
            }
        }

        ~Logger(){
            log_file.close();
        }

        std::string current_time(){
            time_t now = time(0);
            tm* timeinfo = localtime(&now);
            char timestamp[20];
            strftime(timestamp, sizeof(timestamp),"%Y-%m-%d %H:%M:%S", timeinfo);
            return std::string(timestamp);
        }

        int log(std::string message,LOG_TYPE type){   
            if (log_file.is_open()){
                std::string log_message = "[" + current_time() + "] [" + log_type(type) + "] : " + message;
                log_file << log_message << std::endl;
                // flushing is required so that message gets written in the file immadiately
                // so even if app crashes log will be written in the file
                log_file.flush();
                return 1;
            }
            return 0;
        }

        int info_log(std::string message){
            //information log
            return log(message,LOG_TYPE::LOGGER_INFO);
        }

        int warning_log(std::string message){
            //information log
            return log(message,LOG_TYPE::LOGGER_WARNING);
        }

        int error_log(std::string message){
            //information log
            return log(message,LOG_TYPE::LOGGER_ERROR);
        }

    private:
        std::ofstream log_file;
        
        std::string log_type(LOG_TYPE type){
            switch (type){
                case LOG_TYPE::LOGGER_ERROR:
                    return "ERROR";
                case LOG_TYPE::LOGGER_INFO:
                    return "INFO";
                case LOG_TYPE::LOGGER_WARNING:
                    return "WARNING";
            }
            return "";
        }
};