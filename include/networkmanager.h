#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#define DATA_MAX_SIZE 1024
#define MAX_NAME_SIZE 128
#define NO_PLACES "%d Sry buddy, no more places in room!\n" // Message with literals for sprintf
#define GREETING_MSG "%d Buddy %s join the game!\n" // Message with literals for sprintf

#include <depends.h>
#include <config.h>
#include <logger.h>
#include <core.h>

class NetworkManager
{
public:
    NetworkManager(bool server);
    ~NetworkManager();

    unistring ipToStr(IPaddress* addr);
    unistring ipToStr(uint32_t addr);
    void sendMsg(unistring msg);
    void sendData(void* pdata);
    uint32_t ipToUint32(IPaddress *ip);

    bool isClientConnected(int id);

    void connect(unistring ip, uint16_t port);

    bool isClient();
    bool isServer();

    void process();
    uint8_t isDead(int bytes);

    void greetClient(TCPsocket any_sock);
    void greetClient(int any_id);

    static int maxClientCount();

    unistring getNickByIP(uint32_t ip);
    unistring getNickByIP(IPaddress* ip);
private:
    bool server;
    SDLNet_SocketSet sockets;

    IPaddress* myip;
    TCPsocket serverSocket;

    // S E R V E R - O N L Y
    vector<TCPsocket> clientSocket; // For server
    map<uint32_t, unistring> nickList; // Nick list, for server!
    //

    static int maxClients;
};

#endif // NETWORKMANAGER_H
