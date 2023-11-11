//
//  MMQRScan.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/18/18.
//

#include "MMQRScan.h"
#include "MJDefault.h"
#include "StoryLanguageManager.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import <FirebaseDynamicLinks/FirebaseDynamicLinks.h>
#import <AVFoundation/AVFoundation.h>

#define QR_ALERT_TAG 3491832

@interface QRHelper : NSObject<AVCaptureMetadataOutputObjectsDelegate> {
    AVCaptureSession * _session;
    AVCaptureDevice * _device;
    AVCaptureDeviceInput * _input;
    AVCaptureMetadataOutput * _output;
    AVCaptureVideoPreviewLayer * _previewLayer;
    std::string _detectionString;
}
- (void) hideCamera;
@end

@implementation QRHelper
- (id) init {
    self = [super init];
    _session = [AVCaptureSession new];
    _device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    NSError * error = nil;
    _input = [AVCaptureDeviceInput deviceInputWithDevice:_device error:&error];
    if (_input) {
        [_session addInput:_input];
    } else {
        NSLog(@"Error : %@", error);
    }
    
    NSString *mediaType = AVMediaTypeVideo;
    
    AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:mediaType];
    
    if(authStatus == AVAuthorizationStatusAuthorized)
    {
      // do your logic
    }
    else if(authStatus == AVAuthorizationStatusDenied)
    {
        // denied
        //NSLog(@"XXX denied %@", mediaType);
      
        NSString *alertText = @"It looks like your privacy settings are preventing us from accessing your camera to do barcode scanning. You can fix this by doing the following:\n\n1. Touch the Go button below to open the Settings app.\n\n2. Turn the Camera on.\n\n3. Open this app and try again.";

        NSString *alertCancelButtonText = @"Canncel";
        NSString *alertGoButtonText = @"Go";

        if(LANGUAGE_MANAGER->getDisplayLangId()==4)
        {
            alertText = @"Có vẻ như cài đặt quyền riêng tư của bạn đang ngăn chúng tôi truy cập vào máy ảnh của bạn để thực hiện quét mã vạch. Bạn có thể khắc phục điều này bằng cách thực hiện như sau: \n\n1. Chạm vào nút Đi bên dưới để mở ứng dụng Cài đặt. \n\n2. Bật máy ảnh lên. \n\n3. Mở ứng dụng này và thử lại.";
            
            alertGoButtonText = @"Đi";
            alertCancelButtonText = @"Huỷ";
        }
        
        UIAlertView *alert = [[UIAlertView alloc]
                            initWithTitle:@"Error"
                            message:alertText
                            delegate:self
                            cancelButtonTitle:alertGoButtonText
                            otherButtonTitles:alertCancelButtonText, nil
                            ];
        alert.tag = QR_ALERT_TAG;
        [alert show];
    }
    else if(authStatus == AVAuthorizationStatusRestricted)
    {
      // restricted, normally won't happen
    } else if(authStatus == AVAuthorizationStatusNotDetermined){
      // not determined?!
      [AVCaptureDevice requestAccessForMediaType:mediaType completionHandler:^(BOOL granted) {
        if(granted){
          //NSLog(@"XXX Granted access to %@", mediaType);
        } else {
          //NSLog(@"XXX Not granted access to %@", mediaType);
           cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(event_scan_closed);
        }
      }];
    } else {
      // impossible, unknown authorization status
    }
    
    _output = [AVCaptureMetadataOutput new];
    [_output setMetadataObjectsDelegate:self queue:dispatch_get_main_queue()];
    [_session addOutput:_output];
    [_output setMetadataObjectTypes:[_output availableMetadataObjectTypes]];
    
    _previewLayer = [AVCaptureVideoPreviewLayer layerWithSession:_session];
    _previewLayer.frame = CGRectMake(100, 100, 500, 500);
    _previewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
//    _previewLayer.opacity = 0.1;
    auto orientation = [[UIDevice currentDevice] orientation];
    if (orientation == UIDeviceOrientationLandscapeLeft || orientation == UIDeviceOrientationPortrait)
        [_previewLayer.connection setVideoOrientation:AVCaptureVideoOrientationLandscapeRight];
    else if (orientation == UIDeviceOrientationLandscapeRight)
        [_previewLayer.connection setVideoOrientation:AVCaptureVideoOrientationLandscapeLeft];
    
    [[[[UIApplication sharedApplication] keyWindow] rootViewController].view.layer addSublayer:_previewLayer];
    [_session startRunning];
    
    return self;
}

- (void) hideCamera {
    [_previewLayer setOpacity:0];
}

- (void) showCamera {
    [_previewLayer setOpacity:1];
}

- (void) dealloc {
    [_session stopRunning];
    [_previewLayer removeFromSuperlayer];
    [super dealloc];
}

- (void) setFrame: (CGRect) frame {
    [_previewLayer setFrame:frame];
}

- (void)captureOutput:(AVCaptureOutput *)output didOutputMetadataObjects:(NSArray<__kindof AVMetadataObject *> *)metadataObjects fromConnection:(AVCaptureConnection *)connection {
    CGRect highlightViewRect = CGRectZero;
    AVMetadataMachineReadableCodeObject *barCodeObject;
    NSArray *barCodeTypes = @[AVMetadataObjectTypeUPCECode, AVMetadataObjectTypeCode39Code, AVMetadataObjectTypeCode39Mod43Code,
                              AVMetadataObjectTypeEAN13Code, AVMetadataObjectTypeEAN8Code, AVMetadataObjectTypeCode93Code, AVMetadataObjectTypeCode128Code,
                              AVMetadataObjectTypePDF417Code, AVMetadataObjectTypeQRCode, AVMetadataObjectTypeAztecCode];
    
    for (AVMetadataObject *metadata in metadataObjects) {
        for (NSString *type in barCodeTypes) {
            if ([metadata.type isEqualToString:type])
            {
                barCodeObject = (AVMetadataMachineReadableCodeObject *)[_previewLayer transformedMetadataObjectForMetadataObject:(AVMetadataMachineReadableCodeObject *)metadata];
                highlightViewRect = barCodeObject.bounds;
                auto nsstring = [(AVMetadataMachineReadableCodeObject *)metadata stringValue];
                if (nsstring != nil) {
                    std::string _detection = [[(AVMetadataMachineReadableCodeObject *)metadata stringValue] UTF8String];
                    if (!_detection.empty() && _detection != _detectionString) {
                        _detectionString = _detection;
                    }
                }
                break;
            }
        }
    }
}

- (std::string) getDetectedString {
    return _detectionString;
}

- (void) cleanDetectedString {
    _detectionString = "";
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if (alertView.tag == QR_ALERT_TAG)
    {
        if (buttonIndex == [alertView cancelButtonIndex])
        {
             //cancel clicked ...do your action
            [[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];
        }
        else
        {
            [alertView dismissWithClickedButtonIndex:0 animated:YES];
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(event_scan_closed);
             //reset clicked
        }
    }
}

@end
QRHelper * _qr_helper;
std::string _deepLinkString;

void MMQRScan::startCamera(cocos2d::Rect bb_screen) {
    _qr_helper = [[QRHelper alloc] init];

    auto ratio = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().width / [[UIScreen mainScreen] bounds].size.width;
    [_qr_helper setFrame:CGRectMake(bb_screen.origin.x / ratio, bb_screen.origin.y / ratio, bb_screen.size.width / ratio, bb_screen.size.height / ratio)];
}

void MMQRScan::hideCamera() {
    [_qr_helper hideCamera];
}

void MMQRScan::stopCamera(){
    [_qr_helper release];
}

void MMQRScan::showCamera() {
    [_qr_helper showCamera];
}

//Handle scan QR code
std::string MMQRScan::getQRLink() {
    return [_qr_helper getDetectedString];
}

void MMQRScan::cleanQRLink() {
    [_qr_helper cleanDetectedString];
}

//Deep Link Handled
void MMQRScan::parseDeepLink(const std::string & qrLink) {
    NSString* result = [NSString stringWithUTF8String:qrLink.c_str()];
    [[FIRDynamicLinks dynamicLinks]
     handleUniversalLink:[NSURL URLWithString:result]
     completion:^(FIRDynamicLink * _Nullable dynamicLink,NSError * _Nullable error)
     {
         if (dynamicLink != nil && [dynamicLink url] != nil) {
             NSString* deeplink = [[dynamicLink url] absoluteString];
             _deepLinkString = std::string([deeplink UTF8String]);
         } else {
             _deepLinkString = "error";
         }
         
         cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(event_deeplink_handled);
     }];
}

std::string MMQRScan::getDeepLink() {
    return _deepLinkString;
}

void MMQRScan::cleanDeepLink() {
    _deepLinkString = "";
}

#include "MSFirebasePlugin.h"
//Handle: received deeplink from other app
std::string MMQRScan::getReadedDeepLink() {
    return ms::analytics::Firebase::QR_DEEP_LINK;
//    return MJDEFAULT->getStringForKey(key_qrcode_received, "");
}

void MMQRScan::cleanReadedDeepLink() {
    ms::analytics::Firebase::QR_DEEP_LINK = "";
//    MJDEFAULT->setStringForKey(key_qrcode_received, "");
}
#endif
