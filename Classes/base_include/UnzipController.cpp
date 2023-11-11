//
//  UnzipController.cpp
//  MonkeyJunior
//
//  Created by developer on 3/10/16.
//
//

#include "UnzipController.h"
#include "base/CCDirector.h"
#include "base/CCScheduler.h"
#include "base/CCUserDefault.h"
#include "platform/CCFileUtils.h"

#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include <../external/unzip/unzip.h>
#endif

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "MacCppBridge.h"
#endif

UnzipController* UnzipController::instance;

UnzipController* UnzipController::getInstance()
{
    if (instance == nullptr) {
        instance = new UnzipController();
    }
    return instance;
}

bool UnzipController::unzipFile(string fileZipName)
{
    //mdg/sdg/aaa.zip
    vector<string> arr = GlobalFunc::explode(fileZipName, '/');
    fileZipName = arr.at(arr.size()-1);
    arr.erase(arr.begin() + arr.size()-1);
    
    string _storagePath = FileUtils::getInstance()->getWritablePath();
    if(arr.size() > 0) {
        _storagePath += GlobalFunc::joinStringVector(arr, "/") + "/";
    }
    // Open the zip file
    string outFileName = _storagePath + fileZipName;
    CCLOG("outFileName: %s", outFileName.c_str());
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        string error_message = StringUtils::format("can not open downloaded zip file %s", outFileName.c_str());
        unzipError(error_message);
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        string error_message = StringUtils::format("can not read file global info of %s", outFileName.c_str());
        unzipError(error_message);
        unzClose(zipfile);
        return false;
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing");
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  nullptr,
                                  0,
                                  nullptr,
                                  0) != UNZ_OK)
        {
            string error_message = StringUtils::format("can not read file info");
            unzipError(error_message);
            unzClose(zipfile);
            return false;
        }
        
        const string fullPath = _storagePath + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a directory, so create it.
            // If the directory exists, it will failed silently.
            
            
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
            if (!MAC_CPP_BRIDGE->createMacPathFileByString(fullPath))
            {
                string error_message = StringUtils::format("can not create directory %s", fullPath.c_str());
                unzipError(error_message);
                unzClose(zipfile);
                return false;
            }

#else
            if (!FileUtils::getInstance()->createDirectory(fullPath))
            {
                string error_message = StringUtils::format("can not create directory %s", fullPath.c_str());
                unzipError(error_message);
                unzClose(zipfile);
                return false;
            }
#endif
            
        }
        else
        {
            //There are not directory entry in some case.
            //So we need to test whether the file directory exists when uncompressing file entry
            //, if does not exist then create directory
            const string fileNameStr(fileName);
            
            size_t startIndex=0;
            
            size_t index=fileNameStr.find("/",startIndex);
            
            while(index != std::string::npos)
            {
                const string dir=_storagePath+fileNameStr.substr(0,index);
                
                FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(dir).c_str(), "r");
                
                if(!out)
                {
                    
                    #if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
                    if (!MAC_CPP_BRIDGE->createMacPathFileByString(dir))
                    {
                        string error_message = StringUtils::format("can not create directory %s", dir.c_str());
                        unzipError(error_message);
                        unzClose(zipfile);
                        return false;
                    }
                    else
                    {
                        CCLOG("create directory %s",dir.c_str());
                    }

                    #else
                    if (!FileUtils::getInstance()->createDirectory(dir))
                    {
                        string error_message = StringUtils::format("can not create directory %s", dir.c_str());
                        unzipError(error_message);
                        unzClose(zipfile);
                        return false;
                    }
                    else
                    {
                        CCLOG("create directory %s",dir.c_str());
                    }

                    #endif
                    
                    
                }
                else
                {
                    fclose(out);
                }
                
                startIndex=index+1;
                
                index=fileNameStr.find("/",startIndex);
                
            }
            
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                string error_message = StringUtils::format("can not open file %s", fileName);
                unzipError(error_message);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
            if (! out)
            {
                string error_message = StringUtils::format("can not open destination file %s", fullPath.c_str());
                unzipError(error_message);
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    string error_message = StringUtils::format("can not read zip file %s, error code is %d", fileName, error);
                    unzipError(error_message);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                string error_message = StringUtils::format("can not read next file");
                unzipError(error_message);
                unzClose(zipfile);
                return false;
            }
        }
        
        //"do not back up" attribute.
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
        //        const char* attrName = "com.apple.MobileBackup";
        //        u_int8_t attrValue = 1;
        //        setxattr(fullPath.c_str(), attrName, &attrValue, sizeof(attrValue), 0, 0);
#endif
        
    }
    
    CCLOG("end uncompressing");
    unzClose(zipfile);
    FileUtils::getInstance()->removeFile(outFileName);
    //GlobalFunc::insertDatabase();
    return true;
}

void UnzipController::unzipError(string msg) {
    CCLOG("Zip error: %s", msg.c_str());
}


