#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "luaenv/luaenv.h"


#ifndef NETWORK
    namespace Network {
        typedef enum {
            NONE,
            SERVER,
            CLIENT
        } NetworkState;
    }

    class Client {
        private:
            bool Valid = false;
            bool Configured = false;
            sockaddr_in Address;
            int Socket = -1;
            

        public:
            void Send(const uint64_t* data, const int len);
            bool DoClient(LuaInterface* env);
            void Configure(int port, std::string adr);
            bool Init();
            bool IsValid();

            ~Client() {
                Valid = false;

                if (Socket != -1) {
                    close(Socket);
                }
            }
    };

    // Server

    class Server {
        private:
            bool Valid = false;
            bool Configured = false;
            int Socket = -1;
            sockaddr_in Address;
            std::vector<int>ClientSockets;

        public:
            void Send(const int ClientSocket, const uint64_t* data, const int len);
            bool IsValid();
            bool Serve(LuaInterface* env);
            void Configure(int port, std::string adr);
            bool Init();

            ~Server() {
                Valid = false;

                if (Socket != -1) {
                    close(Socket);
                }
            }
    };

    #define NETWORK
#endif