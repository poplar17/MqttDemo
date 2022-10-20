#include "LOG_SYS.h"
#include <ctime>
#include <regex>

Logger::Logger(){
    // 默认构造函数
    this->target = terminal;
    this->level = debug;
    cout << "[WELCOME] " << FILE_INFO << " " << getCurrTime() << " : " << "=== Start logging ===" << endl;
}

Logger::Logger(log_target target, log_level level, string path, string fileName){
    this->target = target;
    this->path = path;
    this->level = level;
    string tmp = "";  // 双引号下的常量不能直接相加，所以用一个string类型做转换
    string welcome_dialog = tmp + "[Welcome] " + fileName + " " + getCurrTime() + " : " + "=== Start logging ===\n";
    if (target != terminal){
        this->outfile.open(path, ios::out | ios::app);   // 打开输出文件
        this->outfile << welcome_dialog;
    }
    if (target != file){
        // 如果日志对象不是仅文件
        cout << welcome_dialog;
    }
}
string getCurrTime(){
    // 获取当前时间，并规范表示，
    time_t now = time(0);
    char *dt = ctime(&now);
    string time = dt;
    std::regex newlines_re("\n+");
    return std::regex_replace(time, newlines_re, "");
}

void Logger::output(string text, log_level act_level, string fileName){
    string prefix;
    if(act_level == debug) prefix = "[DEBUG]   ";
    else if(act_level == info) prefix = "[INFO]    ";
    else if(act_level == warning) prefix = "[WARNING] ";
    else if(act_level == error) prefix = "[ERROR]   ";
    else prefix = "";
    prefix += fileName;
    prefix += " ";
    string output_content = prefix + getCurrTime() + " : " + text + "\n";
    if(this->level <= act_level && this->target != file){
        // 当前等级设定的等级才会显示在终端，且不能是只文件模式
        cout << output_content;
    }
    if(this->target != terminal)
        outfile << output_content;
}


void Logger::DEBUG(string text, string fileName){
    this->output(text, debug, fileName);
}

void Logger::INFO(string text, string fileName){
    this->output(text, info, fileName);
}

void Logger::WARNING(string text, string fileName){
    this->output(text, warning, fileName);
}

void Logger::ERROR(string text, string fileName){
    this->output(text, error, fileName);
}
