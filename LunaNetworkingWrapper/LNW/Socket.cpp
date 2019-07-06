#include "Socket.h"

//LNW::MessageHandle::_MessageHandler MessageHandler = LNW::MessageHandle::_MessageHandler();

LNW::Socket::Socket(bool tcp, MessageHandle::_MessageHandler* msg) {
    //MessageHandle::MessageHandler.WelcomeMsgHandle = [](SOCKET socket, LNW::Buffer& out) { return false; };

    Sock = INVALID_SOCKET;
    isTCP = tcp;
    MessageHandler = msg;

    // Start WSA
    WSADATA wsaData = {};
    if( WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 ) {
        DebugError(WSAGetLastError(), __FILENAME__, __LINE__);
        WSACleanup();
        return;
    }

    // Create socket
    Sock = socket(AF_INET, tcp ? SOCK_STREAM : SOCK_DGRAM, 0);
    if( Sock == INVALID_SOCKET ) {
        DebugError(WSAGetLastError(), __FILENAME__, __LINE__);
        WSACleanup();
        return;
    }
}

std::string LNW::Socket::Resolve(std::string dns) {
    return dns;
}

// Returns 0 when ok
bool LNW::Socket::Connect(USHORT port, std::string ip) {
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = port;
    inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

    // Connect socket
    if( connect(Sock, (SOCKADDR*)&hint, sizeof(sockaddr_in)) == SOCKET_ERROR ) {
        DebugError(WSAGetLastError(), __FILENAME__, __LINE__);
        WSACleanup();
        return true;
    }

    return false;
}

void LNW::Socket::SetOption(UINT flag, const char* val) {
    setsockopt(Sock, SOL_SOCKET, flag, val, sizeof(val));
}

// Server
bool LNW::Socket::Create(USHORT port) {
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    // Bind socket
    if( bind(SockListen, (SOCKADDR*)&hint, sizeof(hint)) == SOCKET_ERROR ) {
        DebugError(WSAGetLastError(), __FILENAME__, __LINE__);

        if( closesocket(SockListen) == SOCKET_ERROR ) {
            DebugError(WSAGetLastError(), __FILENAME__, __LINE__);
        }

        WSACleanup();
        return true;
    }

    return false;
}

bool LNW::Socket::Listen(int MaxConnections) {
    if( listen(SockListen, SOMAXCONN) == SOCKET_ERROR ) {
        DebugError(WSAGetLastError(), __FILENAME__, __LINE__);
        return true;
    };

    if( MaxConnections == 1 ) {
        sockaddr_in addr;
        int len = sizeof(addr);
        SOCKET client = accept(SockListen, (sockaddr*)&addr, &len);
        if( client != INVALID_SOCKET ) {
            DebugMessage("New client connecting...");

            // Connected
            char Host[NI_MAXHOST];
            char Serv[NI_MAXSERV];

            ZeroMemory(&Host, NI_MAXHOST);
            ZeroMemory(&Serv, NI_MAXSERV);

            if( getnameinfo((sockaddr*)&addr, sizeof(addr), Host, NI_MAXHOST, Serv, NI_MAXSERV, 0) == 0 ) {
                std::cout << Host << " connected on port " << Serv << std::endl;
            } else {
                inet_ntop(AF_INET, &addr.sin_addr, Host, NI_MAXHOST);
            }

        } else {
            // 
            DebugMessage("Invalid socket");
            DebugError(WSAGetLastError(), __FILENAME__, __LINE__);
        }
    } else {
        fd_set master;
        FD_ZERO(&master);
        FD_SET(SockListen, &master);

        // Listen for clients
        while( true ) {
            // Copy fd_set and get number of sockets
            fd_set temp = master;
            int sockenN = select(NULL, &temp, nullptr, nullptr, nullptr);
            
            // Iterate through all sockets
            for( int i = 0; i < sockenN; i++ ) {
                SOCKET in = temp.fd_array[i];

                if( in == SockListen ) {
                    // ------ New connection ------
                    // Add new client
                    SOCKET clientSocket = accept(SockListen, nullptr, nullptr);

                    // 
                    FD_SET(clientSocket, &master);

                    // Send welcome message
                    // Check if function is avaliable
                    //if( MessageHandle::MessageHandler.WelcomeMsgHandle != nullptr ) 
                    {
                        if( MessageHandler->WelcomeMsgHandle(clientSocket, Out) ) {
                            send(clientSocket, Out.Data(), Out.Tell(), 0);
                        }
                    }

                    // 
                    DebugMessage((std::string("New client connected ") + std::to_string(master.fd_count + i - 2)).c_str());
                } else {
                    // ------ Receive data ------
                    int sz = recv(in, In.DataPtr(), In.Size(), 0);

                    if( sz <= 0 ) {
                        // ------ Disconnection ------
                        closesocket(in);
                        FD_CLR(in, &master);

                        // 
                        DebugMessage((std::string("Disconnected client ") + std::to_string(i)).c_str());
                    } else {
                        // ------ Receive and handle data ------
                        // Send message to other clients and not to SockListen

                        for( u_int i = 0; i < master.fd_count; i++ ) {
                            SOCKET out = master.fd_array[i];

                            if( out == SockListen || out == in ) { continue; }

                            // Send message to other clients
                            send(out, In.Data(), sz, 0);
                        }
                    }

                }
            }
        }
    }

    return false;
}

void LNW::Socket::Release() {
    if( closesocket(Sock) == SOCKET_ERROR ) {
        DebugError(WSAGetLastError(), __FILENAME__, __LINE__);
    }

    WSACleanup();
}
