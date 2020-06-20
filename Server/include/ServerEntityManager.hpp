#ifndef FEUD_SERVERENTITYMANAGER_HPP
#define FEUD_SERVERENTITYMANAGER_HPP

#include <queue>

struct ServerEntityManager
{
    // pairs of ServerEvents and destinations
    std::queue<std::pair<ServerEvent, std::string>> pendingServerEvents;
    std::queue<std::pair<ClientEvent, std::string>> clientEvents;
};


#endif // FEUD_SERVERENTITYMANAGER_HPP