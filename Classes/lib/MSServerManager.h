//
//  MSServerManager.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/19/18.
//

#ifndef MSServerManager_hpp
#define MSServerManager_hpp

#include <vector>

namespace ms {
    class ServerManager {
    public:
        static ServerManager & getInstance() {
            static ServerManager serverManager;
            return serverManager;
        }
        
        void reset();
        
        void setServerIds(std::vector<int> servers);
        void saveServerIds(std::vector<int> servers);
        int getCurrentServerId();
        int moveToNextServer();
        int getNumberOfServers();
        std::vector<int> getServers();
        
    protected:
        explicit ServerManager();
        
    private:
        ServerManager(ServerManager const &);
        void operator = (ServerManager const &);
        
        std::vector<int> _ids;
        int _current_id;
    };
}
#endif /* MSServerManager_hpp */
