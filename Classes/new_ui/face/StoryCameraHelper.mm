//
//  StoryCameraHelper.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/11/19.
//

#include "StoryCameraHelper.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

@interface PhotoPickerHelper : NSObject<UIImagePickerControllerDelegate> {
}
+ (id) sharedHelper;
+ (UIImage *)imageWithImage:(UIImage *)image scaledToSize:(CGSize)newSize;
@end

@implementation PhotoPickerHelper
+ (id)sharedHelper {
    static PhotoPickerHelper * shared_helper = nil;
    @synchronized (self) {
        if (shared_helper == nil) {
            shared_helper = [[self alloc] init];
        }
        
        return shared_helper;
    }
}

+ (UIImage *)imageWithImage:(UIImage *)image scaledToSize:(CGSize)newSize {
    //UIGraphicsBeginImageContext(newSize);
    // In next line, pass 0.0 to use the current device's pixel scaling factor (and thus account for Retina resolution).
    // Pass 1.0 to force exact pixel size.
    UIGraphicsBeginImageContextWithOptions(newSize, NO, 0.0);
    [image drawInRect:CGRectMake(0, 0, newSize.width, newSize.height)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    UIImage * chosenImage = info[UIImagePickerControllerEditedImage];
    if (!chosenImage)
        chosenImage = info[UIImagePickerControllerOriginalImage];
    
    auto original_size = [chosenImage size];
    auto max_size = cocos2d::Configuration::getInstance()->getMaxTextureSize();
    if (original_size.width > max_size || original_size.height > max_size) {
        if (original_size.width > original_size.height) {
            auto ratio = original_size.width / original_size.height;
            original_size.width = max_size;
            original_size.height = original_size.width / ratio;
        } else {
            auto ratio = original_size.width / original_size.height;
            original_size.height = max_size;
            original_size.width = original_size.height * ratio;
        }
    }
    
    original_size.width = original_size.width / [UIScreen mainScreen].scale;
    original_size.height = original_size.height / [UIScreen mainScreen].scale;
    chosenImage = [PhotoPickerHelper imageWithImage:chosenImage scaledToSize:original_size];
    auto path = cocos2d::FileUtils::getInstance()->getWritablePath() + "face.jpg";
    [UIImageJPEGRepresentation(chosenImage, 0.7f) writeToFile:[NSString stringWithUTF8String:path.c_str()] atomically:YES];
    [picker dismissViewControllerAnimated:YES completion:NULL];
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.change.face");
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker {
    [picker dismissViewControllerAnimated:YES completion:NULL];
}

@end

namespace ms { namespace helper {
    
    void showPhotoLibrary() {
        UIImagePickerController *picker = [[UIImagePickerController alloc] init];
        picker.delegate = [PhotoPickerHelper sharedHelper];
        picker.allowsEditing = NO;
        picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        picker.modalPresentationStyle = UIModalPresentationOverCurrentContext;

        
        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:picker animated:YES completion:NULL];
    }
    
    void showCamera() {
        UIImagePickerController *picker = [[UIImagePickerController alloc] init];
        picker.delegate = [PhotoPickerHelper sharedHelper];
        picker.allowsEditing = YES;
        picker.sourceType = UIImagePickerControllerSourceTypeCamera;
        picker.modalPresentationStyle = UIModalPresentationOverCurrentContext;
        
        [[[[UIApplication sharedApplication] keyWindow] rootViewController] presentViewController:picker animated:YES completion:NULL];
    }
}}

#endif
