#pragma once

#include <ws2tcpip.h>
#include <winsock2.h>
#include <string>
#include <iostream>

#include "Debug.h"
#include "Buffer.h"
//#include "Server.h"
//#include "Client.h"

#pragma comment (lib, "Ws2_32.lib")

namespace LNW {
    namespace MessageHandle {
        typedef bool(*WelcomeMessageHandle)(SOCKET socket, LNW::Buffer& out);
        typedef void(*ServerMessageHandle )(SOCKET socket, const LNW::Buffer* In, LNW::Buffer& out);
        typedef void(*ClientMessageHandle )(SOCKET socket, const LNW::Buffer* In, LNW::Buffer& out);

        struct _MessageHandler {
            WelcomeMessageHandle WelcomeMsgHandle = nullptr;

            union {
                ClientMessageHandle ClientMsgHandle = nullptr;
                ServerMessageHandle ServerMsgHandle;
            };

            const bool Wmsg() const { return WelcomeMsgHandle != nullptr; }
            const bool Cmsg() const { return ClientMsgHandle  != nullptr; }
            const bool Smsg() const { return ServerMsgHandle  != nullptr; }
        } _MessageHandler;
    };

    // Socket handle class
    class Socket {
    private:
        union {
            SOCKET Sock;
            SOCKET SockListen;
        };

        bool isTCP;
        bool isValid;

        Buffer In;
        Buffer Out;

        union {
            /*Client client;
            Server server;*/
        };

    public:
        Socket(): Sock(INVALID_SOCKET), isTCP(false), isValid(false) {};
        Socket(bool tcp = true);

        // Turn DNS string to IP string
        std::string Resolve(std::string dns);

        // Client
        bool Connect(USHORT port, std::string ip);

        // Server
        void SetOption(UINT flag, const char* val); // Settings
        bool Create(USHORT port);                   // Create server
        bool Listen(int MaxConnections=8);          // Launch listening socket

        // 
        void Send();
        void* Recieve();

        // End of session
        void Release();
    };

}
