// Include it before windows.h
#include "LNW/Socket.h"

// 
#include <Windows.h>
#include <iostream>
#include <ostream>

#define MPHeader(x) (LNW::PacketID*)x
#define MPH_Welcome MPHeader(1)

bool WelcomeMsgHandle(SOCKET socket, LNW::Buffer& out) {
    // Seek to start of the buffer
    out.Seek(LNW::RWState::Begin, 0);

    // Header
    //out << MPH_Welcome;

    // Text
    std::string str = "Privet";
    out.WriteBuffer(str.c_str(), str.length() + 1);
    //out.Seek(LNW::RWState::Begin, str.length() + 1);

    // Return true, to send message, false not to
    return true;
 };

int main() {
    LNW::MessageHandle::_MessageHandler MessageHandler;
    LNW::Socket sock(true, &MessageHandler);

    int a = 1;
    //std::cout << "Host? \n";
    //std::cin >> a;

    // Server
    if( a ) {
        //sock.SetOption(SO_REUSEADDR, '\0');

        // Override welcome message
        MessageHandler.WelcomeMsgHandle = WelcomeMsgHandle;

        // 
        if( sock.Create(25565) == 0 ) {
            LNW::DebugMessage("Succesfuly created server!");
        } else {
            // Error
            goto _Release;
        }

        // 
        LNW::DebugMessage("Trying to search for clients.");
        sock.Listen(16);

        // Endless loop for something
        LNW::DebugMessage("Force exit key: ESC");
        while( (GetAsyncKeyState(VK_ESCAPE) & 0x8000) == 0 ) {

        }
    }

    // Client
    if( !a ) {
        if( sock.Connect(25565, "127.0.0.1") == 0 ) {
            LNW::DebugMessage("Succesfuly connected client!");
        } else {
            // Error
            goto _Release;
        }

        LNW::DebugMessage("Force exit key: ESC");
        while( !(GetAsyncKeyState(VK_ESCAPE) & 0x8000) ) {

        }
    }

_Release:
    sock.Release();

    system("pause");
    return 0;
}

// ::main::A::