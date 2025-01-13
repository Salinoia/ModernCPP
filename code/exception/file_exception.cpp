#include <iostream>
#include <fstream>
#include <stdexcept>

void outputFileContent(const std::string& filename){
    std::ifstream ifs(filename);
    std::string fileLineContent;
    if(ifs.is_open()){
        while(std::getline(ifs, fileLineContent)){ // 按行读取文件内容
            std::cout << fileLineContent << std::endl;
        }
        if(ifs.bad()){
            throw std::runtime_error("file is corrupted");
        }
        ifs.close();
    }
    else{
        if(ifs.fail()){
            throw std::runtime_error("file not exists");
        }
        else if(ifs.bad()){
            throw std::runtime_error("ifs is corrupted");
        }
    }
}


int main(){
    std::string str;
    std::cin >> str;
    try{
        outputFileContent(str);
    }
    catch(const std::runtime_error& e){
        std::cout << "Error: " << e.what() << std::endl; // 捕获输入流错误的异常
    }
    catch(...){
        std::cout << "Unknown error occurred." << std::endl; // 捕获其他未知异常
    }
    return 0;    
}