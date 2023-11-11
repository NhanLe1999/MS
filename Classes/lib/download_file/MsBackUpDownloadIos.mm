
#import "MsBackUpDownloadIos.h"
#include "MsNewDownloadFile.h"
#include "cocos2d.h"

#define STATUS_CODE_SUCCESS 200

@implementation MsBackUpDownloadIos

- (void)loadView {

}

- (void) dealloc {
    [_responseData release];
    [super dealloc];
}

#pragma mark -

-(void)setInfoDownload:(NSURL *)url andString:(NSString*)pathFile {
    
    _isReturn = true;
    
    _urlDownload = url;

    _pathFileDownload = pathFile;
    
    _isSucess = true;
    _statusCode = -1;
    _listError = {};
    [self loadFileAtURL];
}

- (void)loadFileAtURL{
    // allocate data buffer
    _responseData = [[NSMutableData alloc] init];
    sizeFileReceive = 0;
    precenBegin = 0.0;
    // create URLRequest
    req = [[NSMutableURLRequest alloc] init];
    [req setURL:_urlDownload];
    [req setTimeoutInterval:60];
    [req setHTTPMethod:@"GET"];

    nzbConnection = [[NSURLConnection alloc] initWithRequest:req delegate:self startImmediately:YES];
    [nzbConnection setDelegateQueue:[[NSOperationQueue alloc] init]];
    [nzbConnection start];
    [req release];
    req = nil;
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    sizeFileReceive = [response expectedContentLength];
    NSLog(@"%lld sum file size",sizeFileReceive);
    
    NSHTTPURLResponse* httpResponse = (NSHTTPURLResponse*)response;
    _statusCode = [httpResponse statusCode];
    httpResponse = nil;
 }


#pragma mark -

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
    
    if(_statusCode == STATUS_CODE_SUCCESS && _isSucess)
    {
        if (connection == nzbConnection)
        {
            
            if (_responseData == nil)
                _responseData = [[NSMutableData alloc] initWithCapacity:sizeFileReceive];
            
            [_responseData appendData:data];
            long long dataSize = [data length];
            sumFileSize = sumFileSize + dataSize;
            
            double precen = (double)sumFileSize / (double)sizeFileReceive;
            double precenProcess = abs(precen - precenBegin);
            
            if(precenProcess > 0.05 || sumFileSize == sizeFileReceive)
            {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                    
                    //[[NSOperationQueue mainQueue] addOperationWithBlock:^{ // Your code to run on the main queue/thread
                        precenBegin = precen;
                        ms::NewDownload::getInstance().processDownload(precenProcess);
                    //}];
                    
                   
                });
            }
        }
    }else{
        _isSucess = false;
        NSString *responseString = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
        NSLog(@"ResponseString:%@",responseString);
        _listError.push_back(responseString);
        [self downloadError];
        responseString = nil;
    }
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection {
    if(_statusCode == STATUS_CODE_SUCCESS && _isSucess)
    {
        if (connection == nzbConnection) {
            [nzbConnection release];
            nzbConnection = nil;

            // Print received data
            if(_responseData != nil)
            {
                NSLog(@"%@",_responseData);
                [_responseData writeToFile:_pathFileDownload atomically:YES];
                [_responseData release];
                
                if(sumFileSize == sizeFileReceive)
                {
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                        ms::NewDownload::getInstance().processDownload(1.0);
                        
                        ms::NewDownload::InfoFileDownload info;
                        
                        ms::NewDownload::getInstance().downloadSucess(info);
                    });
                }else{
                    if(!_isReturn)
                    {
                        _isReturn = false;
                        [self stopConnection];
                        [self setInfoDownload:_urlDownload andString:_pathFileDownload];
                    }
                    else{
                        
                    }
                }
            }
        }
    }else{
        [self stopConnection];
    }
}


- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    // Something went wrong ...
    NSLog(@"Connection failed! Error - %@", [error localizedDescription]);
    
    if (connection == nzbConnection) {
        [nzbConnection release];
        [_responseData release];
    }
    _listError.push_back([error localizedDescription]);
    _listError.push_back([error localizedRecoverySuggestion]);
    _listError.push_back([error localizedFailureReason]);
    [self downloadError];
}

-(void) stopConnection
{
    if(nzbConnection)
    {
        [nzbConnection cancel];
        nzbConnection = nil;
    }
    req = nil;
    
    //delete File;
    NSError *error = nil;
    NSFileManager *manager = [NSFileManager defaultManager];
    [manager removeItemAtPath:_pathFileDownload error:&error];
    
    _responseData = nil;
    _pathFileDownload = nil;
    _urlDownload = nil;
}

-(void) downloadError
{
    if(_statusCode == -1)
    {
        ms::NewDownload::getInstance().downloadError({});
        return;
    }
    NSString *host = @"None";
    NSString *ipAdress = @"None";
    
    if(_urlDownload)
    {
        host = [_urlDownload host];
    }
    
    if(host != @"None")
    {
//#if TARGET_OS_IOS
//        NSData *ipAdressNSData = [[NSHost hostWithName:host]address];
//        ipAdress = [[NSString alloc] initWithData:ipAdressNSData encoding:NSASCIIStringEncoding];
//#else
        ipAdress = [[NSHost hostWithName:host]address];
//#endif
    }
    
    std::string messageError = "None";
    
    
    std::string remoteAdrress = "None";
    
    if(ipAdress != nil)
    {
        remoteAdrress = std::string([ipAdress UTF8String]);
    }
    std::string statusCode = std::to_string(_statusCode);
    std::vector<std::string> data, listMessageError;
    data.push_back(statusCode);
    data.push_back(remoteAdrress);
    if(_listError.size() == 1)
    {
        messageError = std::string([_listError[0] UTF8String]);
        _listError[0]  = nil;
    }else if(_listError.size() > 1)
    {
        for(int i = 0; i < _listError.size(); i++)
        {
            if(_listError[i] != nil)
            {
                listMessageError.push_back([_listError[i] UTF8String]);
            }else{
                listMessageError.push_back("None");
            }
            _listError[i] = nil;
        }
    }
    data.push_back(messageError);
    _listError.clear();
    data.push_back("lost_connection");
    ms::NewDownload::getInstance().downloadError(data, listMessageError);
}

-(void) pushEventLostConnectionToClevertab:(NSString*) link andString:(NSString*)errorType andString:(NSString*) type andString:(NSString*)errorcode andString:(NSString*)countRetry andString:(NSString*)ipAndspeed
{
    //return;
    //link = @"http://platformv1.monkeyjunior.com/live/game-data/config/2F0y6sFCNUKfCmYjPz9gkLtSSEvauWHsK.zip";
    NSURL *url = [[NSURL alloc] initWithString:link];
    std::string idLink = std::string([link UTF8String]);
    std::string idErrorType = std::string([errorType UTF8String]);
    std::string idtype = std::string([type UTF8String]);
    std::string iderrorcode = std::string([errorcode UTF8String]);
    std::string idcountRetry = std::string([countRetry UTF8String]);
    std::string idipAndspeed = std::string([ipAndspeed UTF8String]);
    if(url == NULL)
    {
        mj::helper::pushEvnetLostConnection(idLink, idErrorType, idtype, iderrorcode, idcountRetry, idipAndspeed);
        return;
    }
    NSMutableURLRequest *urlRequest = [[NSMutableURLRequest alloc] initWithURL:url];
    
    if(urlRequest == NULL)
    {
        mj::helper::pushEvnetLostConnection(idLink, idErrorType, idtype, iderrorcode, idcountRetry, idipAndspeed);
        return;
    }
    [urlRequest setHTTPMethod:@"GET"];
    NSURLResponse *response;
    NSError *error;
    NSData *responseData = [NSURLConnection sendSynchronousRequest:urlRequest returningResponse:&response error:&error];
    if(response == NULL)
    {
        mj::helper::pushEvnetLostConnection(idLink, idErrorType, idtype, iderrorcode, idcountRetry, idipAndspeed);
        return;
    }
    NSHTTPURLResponse* httpResponse = (NSHTTPURLResponse*)response;
    if(httpResponse == NULL)
    {
        mj::helper::pushEvnetLostConnection(idLink, idErrorType, idtype, iderrorcode, idcountRetry, idipAndspeed);
        return;
    }
    
    long code = [httpResponse statusCode];
    
    NSString *host = [url host];
//#if TARGET_OS_IOS
//    NSData *ipAdressNSData = [[NSHost hostWithName:host]address];
//    NSString *ipAdress = [[NSString alloc] initWithData:ipAdressNSData encoding:NSASCIIStringEncoding];
//#else
    NSString * ipAdress = [[NSHost hostWithName:host]address];
//#endif
    if(ipAdress == NULL)
    {
        ipAdress = @"None";
    }
    
    std::string idStatusCode = std::to_string(code);
    std::string remoteAddress = std::string([ipAdress UTF8String]);
    
    mj::helper::pushEvnetLostConnection(idLink, idErrorType, idtype, iderrorcode, idcountRetry, idipAndspeed, idStatusCode, remoteAddress);
}

- (BOOL)renameFileFrom:(NSString*)oldName to:(NSString *)newName
{
    if(oldName == nil || newName == nil)
    {
        return NO;
    }

    NSFileManager *fileMan = [NSFileManager defaultManager];
    NSError *error = nil;
    if (![fileMan moveItemAtPath:oldName toPath:newName error:&error])
    {
        NSLog(@"Failed to move '%@' to '%@': %@", oldName, newName, [error localizedDescription]);
        return NO;
    }
    return YES;
}

@end
