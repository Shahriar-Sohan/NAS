#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
extern "C" {
    #include "include/sync_engine.h"
}

int main(){
    std::string source, backup;
    int interval = 5;

    std::ifstream config("config.txt");
    if(!config){
        std::cerr << "Config file not found!\n";
        return 1;
    }

    std::string line;

    while(std::getline(config, line)){
        size_t eq = line.find('=');
        
        if(eq == std::string::npos)continue;

        std::string key = line.substr(0,eq);
        std::string value = line.substr(eq + 1);
        
        if( key == "source") source = value;
        else if (key == "backup") backup = value;
        else if (key == "interval") interval = std::stoi(value);
    }
    std::cout << "Source: " << source << std::endl;
    std::cout << "Backup: " << backup << std::endl;
    std::cout << "Interval: " << interval << std::endl;

    while (true){
        int result = sync_folder(source.c_str(), backup.c_str());

        if(result == 0){
            std::cout << "[✓] Synced successfully or already up-to-date.\n";
        } else {
            std::cerr << "[!] Sync error. Code: " << result << std::endl;
        }
        sleep(interval);
    }

    return 0;
}