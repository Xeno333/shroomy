#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "network/network.h"
#include "shroomy.h"
#include <iostream>
#include <stdio.h>
#include <cstring>
#include "luaenv/luaenv.h"
#include <sstream>




// Server

void Server::Configure(int port, std::string adr) {
    Address.sin_family = AF_INET;

    Address.sin_port = htons(port);
    Address.sin_addr.s_addr = inet_addr(adr.c_str());
    Configured = true;
}

bool Server::Init() {
    if (!Configured)
        return false;

    Socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (Socket == -1) {
        std::cout << "Error starting server: Could not open to socket!" << std::endl;
        return false;
    }

    if (bind(Socket, (sockaddr*)&Address, sizeof(Address)) == -1) {
        std::cout << "Error starting server: Could not bind to socket!" << std::endl;
        close(Socket);
        Socket = -1;
        return false;
    }

    if (listen(Socket, 1) == -1) {
        std::cout << "Error starting server: Could not listen on socket!" << std::endl;
        close(Socket);
        Socket = -1;
        return false;
    }

    Valid = true;
    return true;
}

bool Server::Serve(LuaInterface* env) {
    if (!Valid) {
        Init();
        return false;
    }

    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    int new_client_sock = accept(Socket, (sockaddr*)&client_address, &client_address_len);
    if (new_client_sock != -1) {
        #ifdef SHROOMY_DEBUG
            std::cout << "[Server] Client has conected." << std::endl;
        #endif
        ClientSockets.push_back(new_client_sock);
    }

    uint64_t data[128] = {0};
    int index = 0;
    std::vector<int> discconected_clients_index;

    for (const int ClientSocket : ClientSockets) {
        // Run a few times to make avoid falling behind and filling up que
        for (int it = 5; it != 0; it--) {
            ssize_t ret = recv(ClientSocket, &data, sizeof(data), MSG_DONTWAIT);
            if (ret > 0) {
                std::stringstream tbl("");

                tbl << ClientSocket << ", {";

                for (int i = 0; i != ret / 8;) {
                    tbl << (uint64_t)data[i];

                    if (++i != ret / 8)
                        tbl << ",";
                }
                env -> RunString("Server(" + tbl.str() + "})");
            } else if (ret == -1) {
                break;
            } else if (ret == 0) {
                #ifdef SHROOMY_DEBUG
                    std::cout << "[Server] Client has disconected." << std::endl;
                #endif

                discconected_clients_index.push_back(index);

                std::stringstream tbl("");
                tbl << ClientSocket;
                close(ClientSocket);

                env -> RunString("Server(" + tbl.str() + ",nil)");
            } else {
                #ifdef SHROOMY_DEBUG
                    std::cout << "[Server] Client has disconected." << std::endl;
                #endif

                discconected_clients_index.push_back(index);

                std::stringstream tbl("");
                tbl << ClientSocket;
                close(ClientSocket);

                env -> RunString("Server(" + tbl.str() + ",nil)");
            }
        }

        index++;
    }

    for (const int i : discconected_clients_index) {
        ClientSockets.erase(ClientSockets.begin() + i);
    }

    return true;
}

bool Server::IsValid() {
    return Valid & Configured;
}

void Server::Send(const int ClientSocket, const uint64_t* data, const int len) {
    if (Valid) {
        char test;
        if (recv(ClientSocket, &test, sizeof(char), MSG_PEEK | MSG_DONTWAIT) != 0)
            send(ClientSocket, data, len * sizeof(uint64_t), 0);

        // Will be auto removed by server
    }
}




// Client

void Client::Configure(int port, std::string adr) {
    Address.sin_family = AF_INET;

    Address.sin_port = htons(port);
    Address.sin_addr.s_addr = inet_addr(adr.c_str());
    Configured = true;
}

bool Client::Init() {
    if (!Configured)
        return false;

    Socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (Socket == -1) {
        std::cout << "Client connection error: Could not open to socket!" << std::endl;
        return false;
    }

    
    if (connect(Socket, (sockaddr*)&Address, sizeof(Address)) == -1 && errno != EINPROGRESS) {
        std::cout << "Client connection error: Could not conect to server!" << std::endl;
        close(Socket);
        Socket = -1;
        return false;
    }

    fd_set fd;
    FD_ZERO(&fd);
    FD_SET(Socket, &fd);

    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int select_rc = select(Socket+1, NULL, &fd, NULL, &timeout);
    if (select_rc <= 0) {
        std::cout << "Client connection error: Could not conect to server (Likely timed out)!" << std::endl;
        close(Socket);
        Socket = -1;
        return false;
    }

    int err = 0;
    socklen_t error_len = sizeof(err);
    if (getsockopt(Socket, SOL_SOCKET, SO_ERROR, &err, &error_len) != 0) {
        std::cout << "Client connection error: Could not conect to server!" << std::endl;
        close(Socket);
        Socket = -1;
        return false;
    }
    if (err != 0) {
        std::cout << "Client connection error: Could not conect to server!" << std::endl;
        close(Socket);
        Socket = -1;
        return false;
    }


    std::cout << "Client has connected to server." << std::endl;

    Valid = true;

    return true;
}

void Client::Send(const uint64_t* data, const int len) {
    if (Valid) {
        char test;
        if (recv(Socket, &test, sizeof(char), MSG_PEEK | MSG_DONTWAIT) != 0) {
            send(Socket, data, len * sizeof(uint64_t), 0);
        } else {
            Valid = false;
            close(Socket);
            Socket = -1;
            std::cout << "Client pipe error: Pipe broken!" << std::endl;
        }
    }
}

bool Client::DoClient(LuaInterface* env) {
    if (!Valid) {
        Init();
        return false;
    }

    uint64_t data[128] = {0};

    // Run until caught up to avoid filling que
    while (true) {
        ssize_t ret = recv(Socket, &data, sizeof(data), MSG_DONTWAIT);

        if (ret > 0) {
            std::stringstream tbl("");

            for (int i = 0; i != ret / 8;) {
                tbl << (uint64_t)data[i];

                if (++i != ret / 8)
                    tbl << ",";
            }
            env -> RunString("Client({" + tbl.str() + "})");
        } else if (ret == -1) {
            break;
        } else if (ret == 0) {
            Valid = false;
            close(Socket);
            Socket = -1;
            std::cout << "Client pipe error: Pipe broken!" << std::endl;

            return false;
        } else {
            Valid = false;
            close(Socket);
            Socket = -1;
            std::cout << "Client pipe error: " << strerror(errno) << "!" << std::endl;

            return false;
        }
    }

    return true;
}

bool Client::IsValid() {
    return Valid & Configured;
}