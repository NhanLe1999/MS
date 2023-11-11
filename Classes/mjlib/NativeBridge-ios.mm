#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <Foundation/Foundation.h>
#import "Reachability.h"

#import "AppController.h"
#include "NativeBridge.h"
#include "AppDelegate.h"
USING_NS_CC;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <MessageUI/MFMailComposeViewController.h>
@interface UISendMailHelper : NSObject
-(void)sendMail:(NSString*)subject WithContent:(NSString*)content;
@end

@implementation UISendMailHelper

-(void)sendMail:(NSString*)subject WithContent:(NSString*)content {
    if ([MFMailComposeViewController canSendMail]) {
        MFMailComposeViewController *mailViewController = [[MFMailComposeViewController alloc] init];
        mailViewController.mailComposeDelegate =(id)self;
        
        [mailViewController setSubject:subject];
        
        NSMutableString *emailBody = [[NSMutableString alloc] initWithString:content];
        
        
        NSArray *toRecipients = [NSArray arrayWithObject:@"monkeyxinchao@monkey.edu.vn"];
        [mailViewController setToRecipients:toRecipients];
        [mailViewController setMessageBody:emailBody isHTML:YES];
        
        AppController *app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app.viewController presentViewController:mailViewController animated:true completion:nil];
        mailViewController = nil;
    }
    
    else {
        
        UIAlertView *alertview=[[UIAlertView alloc] initWithTitle:@"Login Mail" message:@"You need to set up your mail on this device first." delegate:nil cancelButtonTitle:@"Close" otherButtonTitles: nil];
        [alertview show];
        
    }
}

- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError*)error
{
    AppController *appct=(AppController*)[[UIApplication sharedApplication] delegate];
    [appct.viewController dismissModalViewControllerAnimated:YES];    
}

@end

#endif


void NativeBridge::keepBackLightOn(bool keep)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (keep) {
        [UIApplication sharedApplication].idleTimerDisabled = YES;
    } else {
        [UIApplication sharedApplication].idleTimerDisabled = NO;
    }
#endif
}

bool NativeBridge::utf8StringCompare(string str1, string str2) {
    NSString *my_str1 = [NSString stringWithUTF8String: str1.c_str()];
    NSString *my_str2 = [NSString stringWithUTF8String: str2.c_str()];
    my_str1 = [my_str1 lowercaseString];
    my_str2 = [my_str2 lowercaseString];
//    NSLog(@"str1: %@ - %d", my_str1, [my_str1 length]);
//    NSLog(@"str2: %@ - %d", my_str2, [my_str2 length]);
    if([my_str1 isEqualToString:my_str2]) {
        return true;
    } else {
        return false;
    }
}

int NativeBridge::utf8LengthOfString(string str) {
    NSString *my_str1 = [NSString stringWithUTF8String: str.c_str()];
    return [my_str1 length];
}

string NativeBridge::utf8characterAtIndex(string str, int index) {
    NSString *my_str1 = [NSString stringWithUTF8String: str.c_str()];
    NSString *newString = [my_str1 substringWithRange:NSMakeRange(index, 1)];
    string str_return([newString UTF8String]);
    return str_return;
}

string NativeBridge::string_towupper(string str) {
    if (str == "") {
        return str;
    }
    NSString *my_str = [NSString stringWithUTF8String: str.c_str()];
    my_str = [my_str uppercaseString];
    string str_return([my_str UTF8String]);
    return str_return;
}
string NativeBridge::string_towlower(string str) {
    if (str == "") {
        return str;
    }
    NSString *my_str = [NSString stringWithUTF8String: str.c_str()];
    my_str = [my_str lowercaseString];
    string str_return([my_str UTF8String]);
    return str_return;
}

bool NativeBridge::networkAvaiable() {
    Reachability *networkReachability = [Reachability reachabilityForInternetConnection];
    
    
    // Internet is reachable
    networkReachability.reachableBlock = ^(Reachability*reach)
      {
          // Update the UI on the main thread
          dispatch_async(dispatch_get_main_queue(), ^{
              NSLog(@"Yayyy, we have the interwebs!");
          });
      };

      // Internet is not reachable
    networkReachability.unreachableBlock = ^(Reachability*reach)
      {
          // Update the UI on the main thread
          dispatch_async(dispatch_get_main_queue(), ^{
              NSLog(@"Someone broke the internet :(");
          });
      };

    [networkReachability startNotifier];
    
    NetworkStatus networkStatus = [networkReachability currentReachabilityStatus];
    if (networkStatus == NotReachable) {
        return false;
    } else {
        return true;
    }
}

NativeBridge* NativeBridge::instance;

NativeBridge* NativeBridge::getInstance()
{
    if (instance == nullptr) {
        instance = new NativeBridge();
    }
    return instance;
}

void NativeBridge::sendMail(string subject, string content) {
    
    NSString *my_subject = [NSString stringWithUTF8String: subject.c_str()];
    NSString *my_content = [NSString stringWithUTF8String: content.c_str()];
    
    _mailHelper = [[UISendMailHelper alloc] init];
    [((UISendMailHelper*)_mailHelper) sendMail:my_subject WithContent:my_content];
}


void NativeBridge::PushReminder(vector<string> messages, vector<int> listDate) {
    UIApplication *app = [UIApplication sharedApplication];
    [app cancelAllLocalNotifications];
    for(int index = 0; index < messages.size(); index++) {
        int date = listDate.at(index);
        UILocalNotification *notification = [[UILocalNotification alloc] init];
        notification.fireDate = [NSDate dateWithTimeIntervalSince1970:date];
        notification.timeZone = [NSTimeZone defaultTimeZone];
        
        NSString *notificationMessage = [NSString stringWithUTF8String: messages.at(index).c_str()];
        notification.alertBody = notificationMessage;
        notification.soundName = @"monkey.aiff";
        [app scheduleLocalNotification:notification];
    }
}

void NativeBridge::initPush() {
}
void NativeBridge::initAlarm(string json) {
}
float NativeBridge::getTimeZoneOffset() {
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    return (float)timeinfo->tm_gmtoff/3600;
}
string NativeBridge::getDeviceToken() {
    return "";
}

void NativeBridge::addSkipBackupAttributeToItemAtPath(string filePathString)
{
    NSString *myPath = [NSString stringWithUTF8String: filePathString.c_str()];
    if([[NSFileManager defaultManager] fileExistsAtPath:myPath]){
        NSURL* URL= [NSURL fileURLWithPath: myPath];
        
        NSError *error = nil;
        BOOL success = [URL setResourceValue: [NSNumber numberWithBool: YES]
                                      forKey: NSURLIsExcludedFromBackupKey error: &error];
        if(!success){
            NSLog(@"Error excluding %@ from backup %@", [URL lastPathComponent], error);
        }
    }
}

vector<int> NativeBridge::findStringOnString(std::string subject_str, std::string find_str, bool isCheckValid){
//    vector<int> positions; // holds all the positions that sub occurs within str
//    size_t pos = subject_str.find(find_str, 0);
//    while(pos != string::npos)
//    {
//        if (checkValidPost(pos, subject_str, find_str)) {
//            positions.push_back((int)pos);
//        }
//        pos = subject_str.find(find_str,pos+1);
//    }
//    return positions;
    //ios
    std::vector<int> results;
    NSString *substring = [NSString stringWithUTF8String:subject_str.c_str()];
    NSString *findstring = [NSString stringWithUTF8String:find_str.c_str()];
    NSUInteger count = 0, length_sub = [substring length], length_find = findstring.length;
    NSRange range = NSMakeRange(0, length_sub);
    while(range.location != NSNotFound)
    {
        range = [substring rangeOfString:findstring options:0 range:range];
        if(range.location != NSNotFound)
        {
            range = NSMakeRange(range.location + range.length, length_sub - (range.location + range.length));
            //NSLog(@"position %d", (int)range.location - length_find);
            auto pos = (int)(range.location - length_find);
            if (!isCheckValid || checkValidPost(pos, subject_str, find_str)) {
                results.push_back(pos);
            }
            count++;
        }
    }
    return results;
}

std::string NativeBridge::hsStringSub(std::string wordgr, int indexChar, int length){
    NSString *substring = [NSString stringWithUTF8String:wordgr.c_str()];
    NSString *subStr = [substring substringWithRange:NSMakeRange(indexChar, length)];
    std::string str_result = std::string([subStr UTF8String]);
    //NSLog(@"sub result = %@", substring);
    //CCLOG("sub result = %s", str_result.c_str());
    return str_result;
}

bool NativeBridge::checkValidPost(int pos, std::string subject_str, std::string find_str){
    auto length_subject = utf8LengthOfString(subject_str);
    auto length_find = utf8LengthOfString(find_str);
    if (length_subject == length_find) {
        return true;
    }else{
        auto daudacbiet = std::string("\n“:.!-?,;\"' ");
        if (pos == 0) {
            auto str_check_end = hsStringSub(subject_str, pos + length_find, 1);
            auto daudacbiet_end = daudacbiet;
            
            for (int j = 0; j < (int) daudacbiet_end.length(); j ++) {
                if (str_check_end == daudacbiet_end.substr(j, 1)) {
                    return true;
                }
            }
            
        }else if (pos + length_find == length_subject){
            auto str_check_start = hsStringSub(subject_str, pos-1, 1);
            auto daudacbiet_start = daudacbiet;
            for (int i = 0; i < (int) daudacbiet_start.length(); i ++) {
                if (str_check_start == daudacbiet_start.substr(i, 1)) {
                    return true;
                }
            }
            
        }else{
            auto str_check_start = hsStringSub(subject_str, pos-1, 1);
            auto daudacbiet_start = daudacbiet;
            auto check_start = false;
            for (int i = 0; i < (int) daudacbiet_start.length(); i ++) {
                if (str_check_start == daudacbiet_start.substr(i, 1)) {
                    check_start = true;
                    break;
                }
            }
            
            if (check_start) {
                auto str_check_end = hsStringSub(subject_str, pos + length_find, 1);
                auto daudacbiet_end = daudacbiet;
                
                for (int j = 0; j < (int) daudacbiet_end.length(); j ++) {
                    if (str_check_end == daudacbiet_end.substr(j, 1)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void NativeBridge::changeDeviceOrientation(bool isPortrait) {
    if (isPortrait) {
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
    } else {
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight];
    }
}

string NativeBridge::hsStringToKhongDau(std::string subject_str, int lang_id){
    std::string str_chuthuongx = "áàảãạăắằẳẵặâấầẩẫậđéèẻẽẹêếềểễệíìỉĩịóòỏõọôốồổỗộơớờởỡợúùủũụưứừửữựýỳỷỹỵæçëîïœûüÿñ";
    std::string str_khongdau = "aaaaaaaaaaaaaaaaadeeeeeeeeeeeiiiiiooooooooooooooooouuuuuuuuuuuyyyyyæçëîïœûüÿñ";
    int size_str_template = 77;
    string re_str;
    subject_str = string_towlower(subject_str);
    for (int i = 0; i < utf8LengthOfString(subject_str); i++) {
        auto sub_str = utf8characterAtIndex(subject_str, i);
        auto check = false;
        for (int j = 0; j < size_str_template; j++) {
            auto compare_str = utf8characterAtIndex(str_chuthuongx, j);
            if (sub_str == compare_str) {
                check = true;
                re_str += utf8characterAtIndex(str_khongdau, j);
                break;
            }
        }
        if (!check) {
            re_str += sub_str;
        }
    }
    return re_str;
}

void NativeBridge::setCrashlyticsKeyValue(std::string key, std::string value){
#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG > 0
    return;
#endif
    [XAppDelegate setCrashlyticsWithKey:[NSString stringWithCString:key.c_str()
                                                           encoding:[NSString defaultCStringEncoding]] andValue:[NSString stringWithCString:value.c_str()
                                                                                                                                   encoding:[NSString defaultCStringEncoding]]];
}

#include "StoryConfigManager.h"
void NativeBridge::rateAppMJStories(){
//    Application::getInstance()->openURL("itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=1236664909");
    if (CONFIG_MANAGER->isVietnameseStories()) {
        Application::getInstance()->openURL("https://itunes.apple.com/app/id1388216940");
    }else Application::getInstance()->openURL("https://itunes.apple.com/app/id1236664909");
}


void NativeBridge::pushDeviceTokenToServer(){
    AppDelegate *app = (AppDelegate*)Application::getInstance();
    app->pushTokenToServer();
    app = nullptr;
}

bool NativeBridge::isTabletDevice(){
    if ( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
    {
        return true; /* Device is iPad */
    }
    return false;
}

bool NativeBridge::isRetinaDevice(){
//    auto device_size = Director::getInstance()->getOpenGLView()->getFrameSize();
//    device_size = device_size/2;
//    float duongcheo = sqrt(device_size.width*device_size.width + device_size.height*device_size.height);
//    float duongcheo_inch = duongcheo/96;
    
    // Normal Bounds - Detect Screen size in Points.
    float width  = [UIScreen mainScreen].bounds.size.width;
    float height = [UIScreen mainScreen].bounds.size.height;
    
    // Native Bounds - Detect Screen size in Pixels.
    float _width  = [UIScreen mainScreen].nativeBounds.size.width;
    float _height = [UIScreen mainScreen].nativeBounds.size.height;
    
    CCLOG("duongcheo_inch = %f | %f with %f | %f", width, height, _width, _height);

    return true;
}

bool NativeBridge::isRamWarning(){
    auto memory_size = [NSProcessInfo processInfo].physicalMemory*1.0f/(1000*1000*1000);
    return memory_size < 0.5f;
}

void NativeBridge::callProscoreAPI(std::vector<std::pair<std::string,std::string>> params,std::function<void(std::string)> onSucceed, std::function<void()> onConnectionError) {

//    NSString* pathAudio = [NSString stringWithUTF8String:path.c_str()];
//    NSString* textCate = [NSString stringWithUTF8String:text.c_str()];

    // Dictionary that holds post parameters. You can set your post parameters that your server accepts or programmed to accept.
    NSMutableDictionary* _params = [[NSMutableDictionary alloc] init];
//    [_params setObject:textCate forKey:[NSString stringWithString:@"category"]];
//    [_params setObject:[NSString stringWithString:@"4"] forKey:[NSString stringWithString:@"device_type"]];
//    [_params setObject:[NSString stringWithFormat:@"vi-VN"] forKey:[NSString stringWithString:@"lang"]];
//    [_params setObject:[NSString stringWithFormat:@"2"] forKey:[NSString stringWithString:@"app_id"]];
    
//    [_params setObject:[NSString stringWithFormat:@"1"] forKey:[NSString stringWithString:@"profile_id"]];
//    [_params setObject:[NSString stringWithFormat:@"1"] forKey:[NSString stringWithString:@"lesson_id"]];
//    [_params setObject:textCate forKey:[NSString stringWithString:@"sentence"]];
    for(int i = 0;i<params.size()-1;i++){ //last element is file
        auto bodyPart = params[i];
        NSString* key = [NSString stringWithUTF8String:bodyPart.first.c_str()];
        NSString* value = [NSString stringWithUTF8String:bodyPart.second.c_str()];
        [_params setObject:value forKey:key];
    }
    // the boundary string : a random string, that will not repeat in post data, to separate post data fields.
    NSString *BoundaryConstant = [NSString stringWithString:@"------------------proscoreBoundary"];

    // string constant for the post parameter 'file'. My server uses this name: `file`. Your's may differ
    NSString* audioKey =[NSString stringWithUTF8String:params[params.size()-1].first.c_str()];
    NSString* audioPath =[NSString stringWithUTF8String:params[params.size()-1].second.c_str()];
//    NSString* FileParamConstant = [NSString stringWithString:@"audio_file"];
    NSString* FileParamConstant = audioKey;

    //#FIXME
    NSURL* requestURL = false ? [NSURL URLWithString:@"https://api.dev.monkeyuni.com/lesson/api/v1/speech/verification?is_web=1&app_id=40"] : [NSURL URLWithString:@"https://app.monkeyuni.net/lesson/api/v1/speech/verification?is_web=1&app_id=40"];

    // create request
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
    [request setCachePolicy:NSURLRequestReloadIgnoringLocalCacheData];
    [request setHTTPShouldHandleCookies:NO];
    [request setTimeoutInterval:30];
    [request setHTTPMethod:@"POST"];

    // set Content-Type in HTTP header
    NSString *contentType = [NSString stringWithFormat:@"multipart/form-data; boundary=%@", BoundaryConstant];
    [request setValue:contentType forHTTPHeaderField: @"Content-Type"];
//    [request addValue:token forHTTPHeaderField: @"token"];

    // post body
    NSMutableData *body = [NSMutableData data];

    // add params (all params are strings)
    for (NSString *param in _params) {
        [body appendData:[[NSString stringWithFormat:@"--%@\r\n", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"\r\n\r\n", param] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"%@\r\n", [_params objectForKey:param]] dataUsingEncoding:NSUTF8StringEncoding]];
    }

    // add image data
    NSData *audioData = [NSData dataWithContentsOfFile: audioPath];
    if (audioData) {
        [body appendData:[[NSString stringWithFormat:@"--%@\r\n", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithFormat:@"Content-Disposition: form-data; name=\"%@\"; filename=\"audiomj.wav\"\r\n", FileParamConstant] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:[[NSString stringWithString:@"Content-Type: wav\r\n\r\n"] dataUsingEncoding:NSUTF8StringEncoding]];
        [body appendData:audioData];
        [body appendData:[[NSString stringWithFormat:@"\r\n"] dataUsingEncoding:NSUTF8StringEncoding]];
    }

    [body appendData:[[NSString stringWithFormat:@"--%@--\r\n", BoundaryConstant] dataUsingEncoding:NSUTF8StringEncoding]];

    // setting the body of the post to the reqeust
    [request setHTTPBody:body];

    // set the content-length
    NSString *postLength = [NSString stringWithFormat:@"%lu",(unsigned long) [body length]];
    [request setValue:postLength forHTTPHeaderField:@"Content-Length"];

    // set URL
    [request setURL:requestURL];
    
//    NSData *returnData = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    [NSURLConnection sendAsynchronousRequest:request queue:queue completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError) {
        if (connectionError) {
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
          
                if(onConnectionError) onConnectionError();
                
            });
            
            return;
        }else{
            NSString *myString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            std::string respone = std::string([myString UTF8String]);
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
          
                if(onSucceed) onSucceed(respone);

            });

        }
    }];
}

#endif

