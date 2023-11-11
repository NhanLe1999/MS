//
//  UnzipController.h
//  MonkeyJunior
//
//  Created by developer on 3/10/16.
//
//

#ifndef __UnzipController__H__
#define __UnzipController__H__
#include "base_include.h"
class UnzipController
{
public:
    static UnzipController* getInstance();
    static bool unzipFile(string fileZipName);
    static void unzipError(string msg);
private:
    static UnzipController* instance;
};
#endif /* __UnzipController__H__ */
