
#import <Foundation/Foundation.h>
#if TARGET_OS_IOS
@interface MsBackUpDownloadIos : UIViewController<NSURLConnectionDelegate, NSURLConnectionDataDelegate>{
    @protected
    NSMutableURLRequest* req;
    NSMutableData* _responseData;
    NSURLConnection* nzbConnection;
    NSString * _pathFileDownload;
    NSURLResponse * _urlResponseData;
    NSError * _errorResPonseData;
    long long sumFileSize;
    long long sizeFileReceive;
    double precenBegin;
    NSURL* _urlDownload;
    int _statusCode;
    BOOL _isSucess;
    BOOL _isReturn;
    std::vector<NSString*> _listError;
}

- (void)loadFileAtURL;
- (void)setInfoDownload:(NSURL *)url andString:(NSString*)pathFile;
- (void)stopConnection;
- (void) downloadError;
-(void) pushEventLostConnectionToClevertab:(NSString*) link andString:(NSString*)errorType andString:(NSString*) type andString:(NSString*)errorcode andString:(NSString*)countRetry andString:(NSString*)ipAndspeed;
- (BOOL)renameFileFrom:(NSString*)oldName to:(NSString *)newName;

@end
#else
@interface MsBackUpDownloadIos : NSViewController<NSURLConnectionDelegate, NSURLConnectionDataDelegate>{
    @protected
    NSMutableURLRequest* req;
    NSMutableData* _responseData;
    NSURLConnection* nzbConnection;
    NSString * _pathFileDownload;
    NSURLResponse * _urlResponseData;
    NSError * _errorResPonseData;
    long long sumFileSize;
    long long sizeFileReceive;
    double precenBegin;
    NSURL* _urlDownload;
    int _statusCode;
    BOOL _isSucess;
    BOOL _isReturn;
    std::vector<NSString*> _listError;
}

- (void)loadFileAtURL;
- (void)setInfoDownload:(NSURL *)url andString:(NSString*)pathFile;
- (void)stopConnection;
- (void) downloadError;
-(void) pushEventLostConnectionToClevertab:(NSString*) link andString:(NSString*)errorType andString:(NSString*) type andString:(NSString*)errorcode andString:(NSString*)countRetry andString:(NSString*)ipAndspeed;
- (BOOL)renameFileFrom:(NSString*)oldName to:(NSString *)newName;

@end
#endif
