//
//  MJStory
//
//  Created by Thien Phuoc on 09/03/2022.
//

import Foundation
#if !os(macOS)
import UIKit
import AppsFlyerLib
#endif
import Speech
import AVKit
import AppTrackingTransparency



@available(iOS 10.0, *)
open class MJHelperSwitf: NSObject {
    
    public typealias UpdateTextSpeedToText = (String?) -> Void
    
    let speechRecognizer        = SFSpeechRecognizer(locale: Locale(identifier: "en-US"))
    var recognitionRequest      : SFSpeechAudioBufferRecognitionRequest?
    var recognitionTask         : SFSpeechRecognitionTask?
    let audioEngine             = AVAudioEngine()
    
    static var s_finalText = ""

    //    public func speechRecognizer(_ speechRecognizer: SFSpeechRecognizer, availabilityDidChange available: Bool) {
    //        if available {
    //            // self.btnStart.isEnabled = true
    //        } else {
    //            //  self.btnStart.isEnabled = false
    //        }
    //    }
    //
    @objc
    func setupSpeech() {
        
        //self.speechRecognizer?.delegate = self
        
        SFSpeechRecognizer.requestAuthorization { (authStatus) in
            
            switch authStatus {
            case .authorized:
                print("ok!")
            case .denied:
                print("User denied access to speech recognition")
            case .restricted:
                print("Speech recognition restricted on this device")
            case .notDetermined:
                print("Speech recognition not yet authorized")
            @unknown default:
                fatalError()
            }
        }
    }
#if !os(macOS)
    @objc
    func stopRecording(_ ch:@escaping UpdateTextSpeedToText) {
        
        if audioEngine.isRunning {
            self.audioEngine.stop()
            self.recognitionRequest?.endAudio()
        }
        
        let audioSession = AVAudioSession.sharedInstance()
        do {
            try audioSession.setCategory(AVAudioSession.Category.playback, mode: AVAudioSession.Mode.default, options: AVAudioSession.CategoryOptions.defaultToSpeaker)
        } catch {
            print("audioSession properties weren't set because of an error.")
        }
      
        ch(MJHelperSwitf.s_finalText);
        
        MJHelperSwitf.s_finalText = "";
    }
    
    
    @objc
    func startRecording(_ ch:@escaping UpdateTextSpeedToText) {
        
        if audioEngine.isRunning {
            
            MJHelperSwitf.s_finalText = "";

            self.audioEngine.stop()
            self.recognitionRequest?.endAudio()
            return;
            //self.btnStart.isEnabled = false
            //self.btnStart.setTitle("Start Recording", for: .normal)
        } else {
            // self.startRecording()
            //self.btnStart.setTitle("Stop Recording", for: .normal)
        }
        
        if recognitionTask != nil {
            recognitionTask?.cancel()
            recognitionTask = nil
        }
        
        // Create instance of audio session to record voice
        let audioSession = AVAudioSession.sharedInstance()
        do {
            try audioSession.setCategory(AVAudioSession.Category.record, mode: AVAudioSession.Mode.measurement, options: AVAudioSession.CategoryOptions.defaultToSpeaker)
            try audioSession.setActive(true, options: .notifyOthersOnDeactivation)
        } catch {
            print("audioSession properties weren't set because of an error.")
        }
        
        self.recognitionRequest = SFSpeechAudioBufferRecognitionRequest()
        
        let inputNode = audioEngine.inputNode
        
        guard let recognitionRequest = recognitionRequest else {
            fatalError("Unable to create an SFSpeechAudioBufferRecognitionRequest object")
        }
        
        recognitionRequest.shouldReportPartialResults = true
        
        self.recognitionTask = speechRecognizer?.recognitionTask(with: recognitionRequest, resultHandler: { (result, error) in
            
            var isFinal = false
            
            if result != nil {
                
                let textResult = result?.bestTranscription.formattedString;
                
                MJHelperSwitf.s_finalText = textResult! ;
                ch(textResult)
                //self.lblText.text = result?.bestTranscription.formattedString
                isFinal = (result?.isFinal)!
            }
            
            if error != nil || isFinal {
                                
                MJHelperSwitf.s_finalText = "";
                
                self.audioEngine.stop()
                inputNode.removeTap(onBus: 0)
                
                self.recognitionRequest = nil
                self.recognitionTask = nil
                
                //self.btnStart.isEnabled = true
            }
        })
        
        let recordingFormat = inputNode.outputFormat(forBus: 0)
        inputNode.installTap(onBus: 0, bufferSize: 1024, format: recordingFormat) { (buffer, when) in
            self.recognitionRequest?.append(buffer)
        }
        
        self.audioEngine.prepare()
        
        do {
            try self.audioEngine.start()
        } catch {
            print("audioEngine couldn't start because of an error.")
        }
        
        //self.lblText.text = "Say something, I'm listening!"
    }
    
    @objc func ShowATTrackingNotification() {
        // start is usually called here:
        if #available(iOS 14, *) {
        }
        else
        {
            self.sendLaunch()
        }
    }

    @objc func sendLaunch() {
        AppsFlyerLib.shared().appsFlyerDevKey = "HEX6DVgSUPuyhSY6Z3PDRh"
        AppsFlyerLib.shared().appleAppID = "1236664909"
        AppsFlyerLib.shared().isDebug = true
        AppsFlyerLib.shared().start()
    }
    
    @objc
    func showIntent(message: NSString, imgPath: NSString, onShareDoneCallback: @escaping (Bool)->Void)
    {
        let imgName: String = imgPath as String
        var text: String = message as String
        
        let img = UIImage(named: imgName)
        let rootView = UIApplication.shared.windows.first!.rootViewController
        let imgData = img?.pngData()
        let textToShare: [Any] = [ text, imgData]
        let activityViewController = UIActivityViewController(activityItems: textToShare, applicationActivities: nil)
                    
        //activityViewController.popoverPresentationController?.sourceView = rootView?.view // so that iPads won't crash
        if let popoverController = activityViewController.popoverPresentationController {
            popoverController.sourceRect = CGRect(x: UIScreen.main.bounds.width/2, y: UIScreen.main.bounds.height/2, width: 0, height: 0);
            popoverController.sourceView = rootView?.view;
            popoverController.permittedArrowDirections = UIPopoverArrowDirection(rawValue: 0);
        }
        rootView?.present(activityViewController, animated: true, completion: nil)
        activityViewController.completionWithItemsHandler =  {
            (activity: UIActivity.ActivityType?, completed: Bool, returnedItems: [Any]?, error: Error?) in
            text = "asdasd"
            onShareDoneCallback(completed)
        }
    }
#endif
}
