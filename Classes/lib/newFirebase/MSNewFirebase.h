#ifndef MSNewFirebase_hpp
#define MSNewFirebase_hpp

#include <firebase/app.h>
#include <firebase/firestore.h>

#define MSFIRESTORE FirebaseFireStore::getInstance()

class FirebaseFireStore {
public:
    static FirebaseFireStore* getInstance();

    void init();

    void updateStories(int userId, int profileId, int storyId, std::function<void()> callback);
    void updateListStories(int userId, int profileId, std::vector<int> listStoryId, std::function<void()> callback);
    void getDataStories(int userId, int profileId, std::function<void()> callback);

private:
    FirebaseFireStore();
    static FirebaseFireStore* m_instance;
    ::firebase::firestore::Firestore* _fireStoreDatabase;
};

#endif 
