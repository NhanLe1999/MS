//
//  MSServerManager.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/19/18.
//

#include "MSServerManager.h"
#include "MJHelper.h"
#include "MJDefinitions.h"
#include "MJDefault.h"

namespace ms {
    ServerManager::ServerManager() {
        _current_id = 0;
        _ids = mj::helper::stringToVectorInt(MJDEFAULT->getStringForKey(key_server_id_download, ""));
    }
    
    void ServerManager::setServerIds(std::vector<int> servers) {
        _ids = servers;
    }
    
    void ServerManager::reset() {
        _current_id = 0;
        _ids = mj::helper::stringToVectorInt(MJDEFAULT->getStringForKey(key_server_id_download, ""));
    }
    
    void ServerManager::saveServerIds(std::vector<int> servers) {
        _ids = servers;
        MJDEFAULT->setStringForKey(key_server_id_download, mj::helper::vectorIntToString(_ids));
    }
    
    int ServerManager::getCurrentServerId() {
        if (_ids.empty()) {
            return -1;
        }
        
        return _ids[_current_id];
    }
    
    int ServerManager::moveToNextServer() {
        if (_ids.empty()) {
            return -1;
        }
        _current_id ++;
        if (_current_id == (int)_ids.size()) {
            _current_id = 0;
        }
        return _ids[_current_id];
    }
    
    int ServerManager::getNumberOfServers() {
        return (int) _ids.size();
    }
    
    std::vector<int> ServerManager::getServers() {
        return _ids;
    }
}
