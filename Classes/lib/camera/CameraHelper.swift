//
//  CameraHelper.swift
//  MJStory
//
//  Created by Thien Phuoc on 09/03/2022.
//
#if !os(macOS)
import Foundation

import SwiftUI
import ReplayKit
import UIKit
//import NextLevel
import AVFoundation
import Photos


@objc
class CameraHelperSwift:NSObject//, NextLevelVideoDelegate,NextLevelDelegate, ReplayKit.RPPreviewViewControllerDelegate
{
    
//    func nextLevel(_ nextLevel: NextLevel, didUpdateVideoConfiguration videoConfiguration: NextLevelVideoConfiguration) {
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didUpdateAudioConfiguration audioConfiguration: NextLevelAudioConfiguration) {
//    }
    
//    func nextLevelSessionWillStart(_ nextLevel: NextLevel) {
//        print("NextLevel nextLevelSessionWillStart:");
//    }
//
//
//    func nextLevelSessionDidStart(_ nextLevel: NextLevel) {
//
//        videoOrientation(isInitial: true)
//
//        print("NextLevel nextLevelSessionDidStart:");
//    }
//
//    func nextLevelSessionDidStop(_ nextLevel: NextLevel) {
//        print("NextLevel nextLevelSessionDidStop:");
//
//    }
//
//    func nextLevelSessionWasInterrupted(_ nextLevel: NextLevel) {
//
//        print("NextLevel nextLevelSessionWasInterrupted:");
//    }
    
//    func nextLevelSessionInterruptionEnded(_ nextLevel: NextLevel) {
//        print("NextLevel nextLevelSessionInterruptionEnded:");
//    }
//
//    func nextLevelCaptureModeWillChange(_ nextLevel: NextLevel) {
//
//        print("NextLevel nextLevelCaptureModeWillChange:");
//    }
//
//    func nextLevelCaptureModeDidChange(_ nextLevel: NextLevel) {
//
//        print("NextLevel nextLevelCaptureModeDidChange:");
//    }
    
//    func nextLevel(_ nextLevel: NextLevel, didCompleteClip clip: NextLevelClip, inSession session: NextLevelSession) {
//
//        print("NextLevel didCompleteClip:");
//
//        if !CameraHelperSwift.savingVidePaths.isEmpty {
//
//            let firstElement =  CameraHelperSwift.savingVidePaths.removeFirst()
//
//            if let url = clip.url {
//
//                print("NextLevel didCompleteClip: save success hihi. ");
//
//                self.saveVideo(withURL: url,withFileName:firstElement)
//            }
//        }
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didUpdateVideoZoomFactor videoZoomFactor: Float) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, willProcessRawVideoSampleBuffer sampleBuffer: CMSampleBuffer, onQueue queue: DispatchQueue) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, renderToCustomContextWithImageBuffer imageBuffer: CVPixelBuffer, onQueue queue: DispatchQueue) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, willProcessFrame frame: AnyObject, timestamp: TimeInterval, onQueue queue: DispatchQueue) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didSetupVideoInSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didSetupAudioInSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didStartClipInSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didAppendVideoSampleBuffer sampleBuffer: CMSampleBuffer, inSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didSkipVideoSampleBuffer sampleBuffer: CMSampleBuffer, inSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didAppendVideoPixelBuffer pixelBuffer: CVPixelBuffer, timestamp: TimeInterval, inSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didSkipVideoPixelBuffer pixelBuffer: CVPixelBuffer, timestamp: TimeInterval, inSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didAppendAudioSampleBuffer sampleBuffer: CMSampleBuffer, inSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didSkipAudioSampleBuffer sampleBuffer: CMSampleBuffer, inSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didCompleteSession session: NextLevelSession) {
//
//    }
//
//    func nextLevel(_ nextLevel: NextLevel, didCompletePhotoCaptureFromVideoFrame photoDict: [String : Any]?) {
//
//    }
    func previewController(_ previewController: RPPreviewViewController, didFinishWithActivityTypes activityTypes: Set<String>) {
        
    }
    static let nextLevelAlbumTitle = "NextLevel"
    static var previewView: UIView?
    static var isCameraAdded = false;
    
    static var isBusyingSavingVideo = false;
    
    static var savingVidePaths : [String] = []
    
    static var isHighlightEnable = false;
    static var pPPreviewViewController: RPPreviewViewController?
    
    public typealias SavingVideoFileCompletionHandler = (Bool) -> Void
    
    public typealias RemoveCameraCompletionHandler = () -> Void
    public typealias StopCameraRecorderCompletionHandler = ((RPPreviewViewController?) -> Void)
//    @objc
//    func preSetup(outputPath:String) {
//
//        if outputPath.isEmpty {
//            return
//        }
//
//        NextLevel.shared.session?.outputDirectory = outputPath
//    }
    
//    @objc
//    func enableAudioWhenRecordCamera(isSoundEnable:Bool) {
//
//        if(isSoundEnable) {
//
//            NextLevel.shared.enableAudioInputDevice();
//        }
//        else
//        {
//            NextLevel.shared.disableAudioInputDevice();
//        }
//    }
    
//    @objc
//    func startCapture(isHighlightEnable:Bool,isSoundEnable:Bool) {
//
//        if CameraHelperSwift.isBusyingSavingVideo {
//
//            print("NextLevel startCapture: busyingSavingVideo....");
//        }
//
//        CameraHelperSwift.isHighlightEnable = isHighlightEnable;
//
//        print("NextLevel startCapture: record");
//
//        if(isHighlightEnable) {
//
//            CameraHelperSwift.previewView?.layer.borderWidth = 2.0
//
//            CameraHelperSwift.previewView?.layer.borderColor = UIColor(red:0 , green:204/255, blue:255/255, alpha: 255/255).cgColor
//        }
//        NextLevel.shared.record()
//    }
    
//    @objc
//    func stopCapture() {
//
//
//        print(" NextLevel stopCapture:  ");
//
//        if !NextLevel.shared.isRecording {
//
//            print(" NextLevel stopCapture:  !NextLevel.shared.isRecording return");
//
//            return;
//        }
//
//        if let session = NextLevel.shared.session {
//
//            session.removeAllClips();
//        }
//        NextLevel.shared.pause()
//    }
//
//    @objc
//    func resetCapture() {
//
//        print(" NextLevel stopCapture:  ");
//
//        NextLevel.shared.stop()
//    }
//

    @objc
    func setVisible( isVisible:Bool ) {
        
        print(" NextLevel stopCapture:  ");
        
        CameraHelperSwift.previewView?.isHidden = !isVisible;
    }
//    @objc
//    func pauseCapture() {
//        NextLevel.shared.pause()
//    }
//
//    @objc
//    func removeCamera(_ ch:@escaping RemoveCameraCompletionHandler) {
//
//        CameraHelperSwift.isCameraAdded = false;
//
//        NextLevel.shared.previewLayer.removeFromSuperlayer();
//
//        CameraHelperSwift.previewView?.removeFromSuperview();
//
//        ch();
//    }
    @objc
    func enableBorder(width:CGFloat,r:CGFloat,g:CGFloat,b:CGFloat,a:CGFloat) {
        
        CameraHelperSwift.previewView?.layer.borderWidth = width
        CameraHelperSwift.previewView?.layer.borderColor = UIColor(red:r/255, green:g/255, blue:b/255, alpha: a/255).cgColor
        
    }
    
    @objc
    func moveFrontCameraToPos(x:CGFloat,y:CGFloat) {
        
        UIView.animate(withDuration: 2) {
            
            CameraHelperSwift.previewView?.frame.origin.x = x;
            CameraHelperSwift.previewView?.frame.origin.y = y;
        }
    }
    
//    @objc
//    func endCapture(fileName:String,_ ch:@escaping SavingVideoFileCompletionHandler) {
//
//        print("NextLevel endCapture:", fileName);
//
//        if !NextLevel.shared.isRecording {
//
//            print("NextLevel endCapture:  !NextLevel.shared.isRecording return\n\n", fileName);
//
//            ch(false);
//
//            return;
//        }
//
//        if CameraHelperSwift.isHighlightEnable {
//
//            CameraHelperSwift.previewView?.layer.borderWidth = 1.5
//
//            CameraHelperSwift.previewView?.layer.borderColor = UIColor.lightGray.cgColor
//        }
//
//        CameraHelperSwift.isHighlightEnable = false;
//
//        CameraHelperSwift.isBusyingSavingVideo = true;
//
//        CameraHelperSwift.savingVidePaths.append(fileName);
//
//        NextLevel.shared.pause(withCompletionHandler: {
//
//            print("NextLevel endCapture: pause callback call", fileName);
//
//            CameraHelperSwift.isBusyingSavingVideo = false;
//
//        });
//
//    }
//
//    @objc
//    func record() {
//
//        authorization();
//
//        NextLevel.shared.record()
//    }
//    @objc
//    func getAuthorizationStatus()-> Bool{
//
//        if NextLevel.authorizationStatus(forMediaType: AVMediaType.video) == .authorized &&
//            NextLevel.authorizationStatus(forMediaType: AVMediaType.audio) == .authorized {
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }
//    @objc
//    func authorization(){
//
//        if NextLevel.authorizationStatus(forMediaType: AVMediaType.video) == .authorized &&
//            NextLevel.authorizationStatus(forMediaType: AVMediaType.audio) == .authorized {
//            do {
//                try NextLevel.shared.start()
//            } catch {
//                print("NextLevel, failed to start camera session")
//            }
//        } else {
//            NextLevel.requestAuthorization(forMediaType: AVMediaType.video) { (mediaType, status) in
//                print("NextLevel, authorization updated for media \(mediaType) status \(status)")
//                if NextLevel.authorizationStatus(forMediaType: AVMediaType.video) == .authorized &&
//                    NextLevel.authorizationStatus(forMediaType: AVMediaType.audio) == .authorized {
//                    do {
//                        let nextLevel = NextLevel.shared
//                        try nextLevel.start()
//                    } catch {
//                        print("NextLevel, failed to start camera session")
//                    }
//                } else if status == .notAuthorized {
//                    // gracefully handle when audio/video is not authorized
//                    print("NextLevel doesn't have authorization for audio or video")
//                }
//            }
//            NextLevel.requestAuthorization(forMediaType: AVMediaType.audio) { (mediaType, status) in
//                print("NextLevel, authorization updated for media \(mediaType) status \(status)")
//                if NextLevel.authorizationStatus(forMediaType: AVMediaType.video) == .authorized &&
//                    NextLevel.authorizationStatus(forMediaType: AVMediaType.audio) == .authorized {
//                    do {
//                        let nextLevel = NextLevel.shared
//                        try nextLevel.start()
//                    } catch {
//                        print("NextLevel, failed to start camera session")
//                    }
//                } else if status == .notAuthorized {
//                    // gracefully handle when audio/video is not authorized
//                    print("NextLevel doesn't have authorization for audio or video")
//                }
//            }
//        }
//    }
    
    var result = false;
    @objc
    func getAuthorizationNotifyStatus()-> Bool{
        
        if #available(iOS 10.0, *) {
            UNUserNotificationCenter.current().getNotificationSettings { (settings) in
                if settings.authorizationStatus == .authorized {
                    // Already authorized
                    self.result = true;
                }
                else {
                    self.result = false;
                }}
        } else {
            // Fallback on earlier versions
        };
        return result;
    }
    
    @objc
    func authorizationNotifyStatus(){
        if #available(iOS 10.0, *) {
            UIApplication.shared.open(URL(string: UIApplication.openSettingsURLString)!, options: [:], completionHandler: nil)
        } else {
            // Fallback on earlier versions
        };
    }
//    @objc
//    func addCameraToView(x:CGFloat,y:CGFloat, width:CGFloat,heigh:CGFloat,parent:UIWindow) {
//
//        if(CameraHelperSwift.isCameraAdded) {
//
//            print("Camera is added !");
//
//            return;
//        }
//
//        CameraHelperSwift.isCameraAdded = true;
//
//        authorization();
//
//        if(NextLevel.shared.devicePosition ==  .back ) {
//
//            NextLevel.shared.flipCaptureDevicePosition()
//        }
//
//        var screenBounds = UIScreen.main.bounds
//
//        screenBounds.origin.x = x;
//        screenBounds.origin.y = y;
//        screenBounds.size.width = width;
//        screenBounds.size.height = heigh;
//
//        NextLevel.shared.videoConfiguration.bitRate = 12*1024;
//        NextLevel.shared.videoDelegate  = self;
//        NextLevel.shared.delegate = self;
//        NextLevel.shared.videoZoomFactor = 0.5;
//
//        CameraHelperSwift.previewView = UIView(frame: screenBounds)
//        setVideoOrientationLockStatus(isOrientationLock: true);
//        if let previewView = CameraHelperSwift.previewView {
//
//            previewView.autoresizingMask = [.flexibleWidth, .flexibleHeight]
//            previewView.backgroundColor = UIColor.black
//            NextLevel.shared.previewLayer.frame = previewView.bounds
//            NextLevel.shared.previewLayer.cornerRadius = 10
//            previewView.layer.addSublayer(NextLevel.shared.previewLayer)
//            videoOrientation(isInitial: true)
//            previewView.layer.cornerRadius = 10
//            //ask the system to start notifying when interface change
////           UIDevice.current.beginGeneratingDeviceOrientationNotifications()
////           //add the observer
////           NotificationCenter.default.addObserver(self,selector: #selector(orientationChanged(notification:)),name: UIDevice.orientationDidChangeNotification, object: nil)
//            parent.addSubview(previewView)
//
//        }
//    }
//    var _isOrientationLock = true;
//
//    @objc
//    func setVideoOrientationLockStatus(isOrientationLock: Bool){
//        _isOrientationLock = isOrientationLock
//    }
//    @objc
//    func getVideoOrientationLockStatus()-> Bool {
//        return _isOrientationLock
//    }
//    @objc
//    func orientationChanged(notification : NSNotification) {
//        if let previewView = CameraHelperSwift.previewView {
//
//            if let photoOutputConnection = NextLevel.shared.previewLayer.connection{
//                                        // USE the below function HERE
//                videoOrientation(isInitial: false)
//            }
//
//        }
//    }
    @objc
    func getOrientation() -> Int{
        var orientationbar: UIInterfaceOrientation = UIInterfaceOrientation.portrait
        if #available(iOS 15.0, *) {
            if let interfaceOrientation = UIApplication.shared.windows.first(where: { $0.isKeyWindow })?.windowScene?.interfaceOrientation {
                // Use interfaceOrientation
                orientationbar = interfaceOrientation
            }
            else
            {
                orientationbar = UIApplication.shared.statusBarOrientation
            }
        }
        else
        {
            orientationbar = UIApplication.shared.statusBarOrientation
        }
        return orientationbar.rawValue;
    }
    @objc
    func getOSVersion15Support() -> Bool{
        if #available(iOS 15.4, *) {
            return true;
        }
        return false;
    }
//    func videoOrientation(isInitial: Bool){
//
//        if let previewView = CameraHelperSwift.previewView {
//
//            DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
//
//                if let photoOutputConnection = NextLevel.shared.previewLayer.connection{
//                    if(photoOutputConnection.isVideoOrientationSupported){
//                        let orientation: UIDeviceOrientation = UIDevice.current.orientation
//                        var orientationbar: UIInterfaceOrientation = UIInterfaceOrientation.portrait
//                        if #available(iOS 15.0, *) {
//                            if let interfaceOrientation = UIApplication.shared.windows.first(where: { $0.isKeyWindow })?.windowScene?.interfaceOrientation {
//                                // Use interfaceOrientation
//                                orientationbar = interfaceOrientation
//                            }
//                            else
//                            {
//                                orientationbar = UIApplication.shared.statusBarOrientation
//                            }
//                        } else {
//                            // Fallback on earlier versions
//                            orientationbar = UIApplication.shared.statusBarOrientation
//                        }
//
//                        print("NextLevel orientation dev",orientation)
//                        var avCaptureVideoOrientation: AVCaptureVideoOrientation = AVCaptureVideoOrientation.landscapeRight
//                        switch orientationbar {
//                        case .unknown:
//                            avCaptureVideoOrientation = .landscapeRight
//                        case .portrait:
//                            avCaptureVideoOrientation = .landscapeRight
//                        case .portraitUpsideDown:
//                            avCaptureVideoOrientation = .landscapeRight
//                        case .landscapeRight:
//                            avCaptureVideoOrientation = .landscapeRight
//
//                        case .landscapeLeft:
//                            avCaptureVideoOrientation = .landscapeLeft
//
//                        @unknown default:
//                            avCaptureVideoOrientation = .landscapeRight
//
//                        }
//                        if(photoOutputConnection.videoOrientation != avCaptureVideoOrientation)
//                        {
//                            self.setVideoOrientationLockStatus(isOrientationLock: false)
//                            NextLevel.shared.previewLayer.connection?.videoOrientation = avCaptureVideoOrientation
//                        }
//
//                    }
//
//                }
//            }
//        }
//    }
//
    internal func albumAssetCollection(withTitle title: String) -> PHAssetCollection? {
        let predicate = NSPredicate(format: "localizedTitle = %@", title)
        let options = PHFetchOptions()
        options.predicate = predicate
        let result = PHAssetCollection.fetchAssetCollections(with: .album, subtype: .any, options: options)
        if result.count > 0 {
            return result.firstObject
        }
        return nil
    }
    
    internal func getDocumentsDirectory() -> URL {
        // find all possible documents directories for this user
        let paths = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask)
        
        // just send back the first one, which ought to be the only one
        return paths[0]
    }
    
    
    internal func writeXX(outputFileNamePath: String, data: NSData?) throws {
        try data?.write(toFile: outputFileNamePath, atomically: false)
    }
    
    internal func saveVideo(withURL url: URL,withFileName outputFileNamePath:String) {
        
        let videoData = NSData(contentsOf: url)
        
        do
        {
            try writeXX(outputFileNamePath: outputFileNamePath, data: videoData);
            
            print("NextLevel saveVideo success save video to document data. Path :",outputFileNamePath)
        } // throws or not depending on if something was written or not
        catch let error as NSError
        {
            print("NextLevel saveVideo cannot save video file : \(error.debugDescription)")
        }
        
    }
    @objc
    func shareVideoIOSWithUrl(videoPath:String, parent:UIWindow, completion:(() -> Void)? = {}) {

        let fileURL = NSURL(fileURLWithPath: videoPath)

        // Create the Array which includes the files you want to share
        var filesToShare = [Any]()

        // Add the path of the file to the Array
        filesToShare.append(fileURL)

        // Make the activityViewContoller which shows the share-view
        let activityViewController = UIActivityViewController(activityItems: filesToShare, applicationActivities: nil)
        if UIDevice.current.userInterfaceIdiom == .pad {
            activityViewController.popoverPresentationController?.sourceRect = CGRect(x: UIScreen.main.bounds.width / 2, y: UIScreen.main.bounds.height / 2, width: 0, height: 0)
            activityViewController.popoverPresentationController?.sourceView  = parent;
            activityViewController.popoverPresentationController?.permittedArrowDirections = UIPopoverArrowDirection(rawValue: 0)
         }
            parent.rootViewController?.present(activityViewController, animated: true, completion: completion)
    }
    public typealias dissmisShareVideo = ((RPPreviewViewController?) -> Void)
    
    
//    @objc
//    func shareVideoIOSWithOutUrl(parent:UIWindow, completion:(() -> Void)? = {}) {
//
//        let previewVC = CameraHelperSwift.pPPreviewViewController!;
//        if (previewVC == nil)
//        {
//            return;
//        }
//        let activityViewController = previewVC
//        activityViewController.previewControllerDelegate = self;
//        if UIDevice.current.userInterfaceIdiom == .pad {
//            activityViewController.popoverPresentationController?.sourceRect = CGRect(x: UIScreen.main.bounds.width / 2, y: UIScreen.main.bounds.height / 2, width: 0, height: 0)
//            activityViewController.popoverPresentationController?.sourceView  = parent;
//            activityViewController.popoverPresentationController?.permittedArrowDirections = UIPopoverArrowDirection(rawValue: 0)
//
//         }
//
//        parent.rootViewController?.present(activityViewController, animated: true, completion: nil)
//    }
    @objc
    func previewControllerDidFinish(_ previewController: RPPreviewViewController) {
            previewController.dismiss(animated: true, completion: nil)
        }
    
//    @objc
//    func startVideoCapture() {
//        let recorder = RPScreenRecorder.shared()
//        recorder.startRecording{ [unowned self] (error) in
//             if let unwrappedError = error {
//                 print(unwrappedError.localizedDescription)
//             }
//         }
//    }
    @objc
    func stopVideoCaptureWithoutOutputUrl(parent:UIWindow,  completion: @escaping StopCameraRecorderCompletionHandler) {
        let recorder = RPScreenRecorder.shared()

               recorder.stopRecording { [unowned self] (preview, error) in

                   if let previewVC = preview {
                       completion(previewVC)
                       CameraHelperSwift.pPPreviewViewController = previewVC;
                   }
                   
                   
                   
                   
                   if let error = error {
                       print( false)
                       //completion(nil)
                   }
                   
               }
    }
    @objc
    func stopVideoCaptureWithOutputUrl() {
        let recorder = RPScreenRecorder.shared()

        let outputURL: URL  = tempURL()!
        if #available(iOS 14.0, *) {
            recorder.stopRecording(withOutput: outputURL) { (error) in
                guard error == nil else{
                    print("Failed to save ")
                    return
                }
                print(outputURL)
            }
        } else {
            // Fallback on earlier versions
        }
    }
    func tempURL() -> URL? {
        let directory = NSTemporaryDirectory() as NSString
            
        if directory != "" {
            let path = directory.appendingPathComponent(NSUUID().uuidString + ".mp4")
            return URL(fileURLWithPath: path)
        }
        return nil
    }
}
#endif
