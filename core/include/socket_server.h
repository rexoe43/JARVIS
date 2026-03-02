#pragma once
#include <string>

class SocketClient {
    
        public:
        bool connect_to_brain();
        std::string send_command(const std::string& command);
    private:
    int sock;
    
};