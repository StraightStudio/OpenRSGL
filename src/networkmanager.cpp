#include <networkmanager.h>

int NetworkManager::maxClients = 4; // Maximum 4 players

NetworkManager::NetworkManager(bool serve) // To serve, or to connect as client?
{
	serverSocket = NULL;
	//
	SDLNet_Init();

	server = serve;

	clientSocket.resize(maxClientCount());
	for(int i=0; i < (int)clientSocket.capacity(); i++)
		clientSocket[i] = NULL;

	sockets = SDLNet_AllocSocketSet(maxClients);
	myip = new IPaddress;
	if (server)
	{
		if (myip)
		{
			SDLNet_ResolveHost(myip, INADDR_ANY, GAME_PORT);
			serverSocket = SDLNet_TCP_Open(myip);
			if (!serverSocket)
			{
				Logger::err("NetworkManager",
						"Failed to bind to port: " + to_string(GAME_PORT));
			}
			else
				SDLNet_TCP_AddSocket(sockets, serverSocket); // Add myself to list
		}
	}
}

NetworkManager::~NetworkManager()
{
	SDLNet_FreeSocketSet(sockets);

	if (serverSocket)
		SDLNet_TCP_Close(serverSocket);

	for(int i=0; i < (int)clientSocket.size(); i++) // Must disconnect all CONNECTED sockets
	{
		if(clientSocket[i] != NULL) // Connected?
			SDLNet_TCP_Close(clientSocket[i]);
	}

	if (myip != nullptr)
		delete myip;

	SDLNet_Quit();
}

unistring NetworkManager::ipToStr(IPaddress *addr)
{
	return unistring(
			to_string((addr->host & 0xff)) + "."
					+ to_string(((addr->host & 0xff) >> 8)) + "."
					+ to_string(((addr->host & 0xff) >> 16)) + "."
					+ to_string(((addr->host & 0xff) >> 24)));
}

unistring NetworkManager::ipToStr(uint32_t addr)
{
	return unistring(
			to_string((addr & 0xff)) + "." + to_string(((addr & 0xff) >> 8))
					+ "." + to_string(((addr & 0xff) >> 16)) + "."
					+ to_string(((addr & 0xff) >> 24)));
}

void NetworkManager::sendMsg(unistring msg)
{
	int dataLen = msg.length() + 2; // 2 additional for identification of data type (msg, pinfo, serverStatus, etc...)
	char* buff = (char*) calloc(dataLen, sizeof(char));
	sprintf(buff, "%d %s", 0x02, msg.c_str()); // 0x02 - msg, 0x06 - some data
	if (server)
	{
		// Must send to all clients
		for(int i=0; i < (int)clientSocket.size(); i++)
		{
			if(clientSocket[i] != NULL)
				SDLNet_TCP_Send(clientSocket[i], (void *) buff, dataLen);
		}
	}
	else
		SDLNet_TCP_Send(serverSocket, (void *) buff, dataLen);
	free(buff);
}

void NetworkManager::sendData(void* pdata)
{
}

bool NetworkManager::isClientConnected(int id)
{
	if (clientSocket[id] != NULL)
		return true;
	return false;
}

void NetworkManager::connect(unistring ip, uint16_t port)
{
	if (!myip)
		myip = new IPaddress;
	SDLNet_ResolveHost(myip, ip.c_str(), port);
	serverSocket = SDLNet_TCP_Open(myip);
	if (!serverSocket)
	{
		Logger::err("NetworkManager",
				"Failed to bind to port: " + to_string(GAME_PORT));
	}
	else
		SDLNet_TCP_AddSocket(sockets, serverSocket); // Add myself to list
}

bool NetworkManager::isClient()
{
	return !server;
}

bool NetworkManager::isServer()
{
	return server;
}

void NetworkManager::process()
{
	int socketStatus = SDLNet_CheckSockets(sockets, 0); // not time to check
	if (socketStatus > 0) // Someone active
	{
		if (server) // Server-side
		{
			if (serverSocket) // If connection is alive
			{
				if (SDLNet_SocketReady(serverSocket)) // Check if someone connected
				{
					TCPsocket tmpSock = SDLNet_TCP_Accept(serverSocket); // Let client connect
					if (tmpSock) // If client is not NULL
					{
						if ((int)clientSocket.size() < maxClients)
						{
							SDLNet_TCP_AddSocket(sockets, tmpSock);
							clientSocket.push_back(tmpSock);
							greetClient(tmpSock);
						}
						else
						{
							size_t dataLen = strlen(NO_PLACES);
							char* dataMsg = (char*) calloc(dataLen, sizeof(char));
							sprintf(dataMsg, NO_PLACES, 0x02);
							if (isDead(SDLNet_TCP_Send(tmpSock,(void*) dataMsg, dataLen))) // True, if socket is 'dead' (disconnected or smthn)
								Logger::err("NetworkManager","Failed to send data to"+ ipToStr(SDLNet_TCP_GetPeerAddress(tmpSock)));
							free(dataMsg);
						}
					}
				}
				// Iterate through clients
				for(int i=0; i < (int)clientSocket.size(); i++)
				{
					TCPsocket sock = clientSocket[i];
					if (sock) // Not null!!
					{
						if (SDLNet_SocketReady(sock)) // Client send message to us!
						{
							char* buff = (char*) calloc(DATA_MAX_SIZE,
									sizeof(char));
							if (isDead(SDLNet_TCP_Recv(sock, buff, DATA_MAX_SIZE))) // Client dead, disconnect him!
							{
								Core::sharedPtr()->onDisconnect(ipToStr(SDLNet_TCP_GetPeerAddress(sock)));
								SDLNet_TCP_DelSocket(sockets, sock);
								SDLNet_TCP_Close(sock);
								sock = NULL;
							}
							else // All ok, read msg
							{
								if (buff[0] == 0x02)
								{
									fprintf(stdout, "[%s]: %s\n",ipToStr(SDLNet_TCP_GetPeerAddress(sock)), buff);
									fflush(stdout);
								}
								else if (buff[0] == 0x06)
								{

								}
							}
							free(buff);
						}
					}
					else
					{
						Logger::err("NetworkManager", "Lost server connection...");
					}
				}
			}
		}
		else        // Client-side
		{
			if (serverSocket) // If server is alive
			{
				if (SDLNet_SocketReady(serverSocket)) // Check if server sent smthn
				{
					char* buff = (char*) calloc(DATA_MAX_SIZE, sizeof(char));

					if (isDead(
							SDLNet_TCP_Recv(serverSocket, buff, DATA_MAX_SIZE))) // Socket is dead (Disconnected or smth)
					{
						SDLNet_TCP_DelSocket(sockets, serverSocket);
						SDLNet_TCP_Close(serverSocket);
						serverSocket = NULL;

						Logger::warn("NetworkManager",
								"Server has closed the connection.");
					}
					else
					{
						if (buff[0] == 0x02) // ASCII Start of TeXt (STX)
						{
							fprintf(stdout, "[SERVER]: %s\n", buff);
							fflush(stdout);
						}
						else if (buff[0] == 0x06) // ASCII ACKnowledge (ACK)
						{
						}
					}
					free(buff);
				}
			}
		}
	}
}

uint8_t NetworkManager::isDead(int bytes)
{
	if (bytes <= 0)
		return 1;
	else
		return 0;
}

void NetworkManager::greetClient(TCPsocket any_sock)
{
	if(any_sock) // Not null!
	{
		char* buff = (char*)calloc(unistring(GREETING_MSG).length(), sizeof(char));
		sprintf(buff, GREETING_MSG, 0x02, getNickByIP(SDLNet_TCP_GetPeerAddress(any_sock)));
		free(buff);
	}
}

void NetworkManager::greetClient(int any_id)
{
	if(any_id < clientSocket.size())
	{
		if(clientSocket[any_id]) // Not NULL!
			greetClient(clientSocket[any_id]);
	}
}

int NetworkManager::maxClientCount()
{
	return maxClients;
}

uint32_t NetworkManager::ipToUint32(IPaddress* ip)
{
	return ip->host;
}

unistring NetworkManager::getNickByIP(uint32_t ip)
{
	if(nickList.find(ip) != nickList.end()) // Found nick!
	{
		return nickList.at(ip);
	}
	return unistring("Guest");
}

unistring NetworkManager::getNickByIP(IPaddress* ip)
{
	return getNickByIP(ipToUint32(ip));
}
