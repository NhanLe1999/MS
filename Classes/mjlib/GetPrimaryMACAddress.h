#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/network/IOEthernetInterface.h>
#include <IOKit/network/IONetworkInterface.h>
#include <IOKit/network/IOEthernetController.h>
#include <iostream>
namespace MacOSInfo{
    class MacOSInfo {
    public:
        static kern_return_t FindEthernetInterfaces(io_iterator_t *matchingServices)
        {
            kern_return_t           kernResult;
            CFMutableDictionaryRef	matchingDict;
            CFMutableDictionaryRef	propertyMatchDict;
            matchingDict = IOServiceMatching(kIOEthernetInterfaceClass);
            if (NULL == matchingDict) {
                printf("IOServiceMatching returned a NULL dictionary.\n");
            }
            else {
                propertyMatchDict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                              &kCFTypeDictionaryKeyCallBacks,
                                                              &kCFTypeDictionaryValueCallBacks);
                
                if (NULL == propertyMatchDict) {
                    printf("CFDictionaryCreateMutable returned a NULL dictionary.\n");
                }
                else {
                    CFDictionarySetValue(propertyMatchDict, CFSTR(kIOPrimaryInterface), kCFBooleanTrue);
                    CFDictionarySetValue(matchingDict, CFSTR(kIOPropertyMatchKey), propertyMatchDict);
                    CFRelease(propertyMatchDict);
                }
            }
            kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, matchingServices);
            if (KERN_SUCCESS != kernResult) {
                printf("IOServiceGetMatchingServices returned 0x%08x\n", kernResult);
            }
            
            return kernResult;
        }
        
        static kern_return_t GetMACAddress(io_iterator_t intfIterator, UInt8 *MACAddress, UInt8 bufferSize)
        {
            io_object_t		intfService;
            io_object_t		controllerService;
            kern_return_t	kernResult = KERN_FAILURE;
            
            if (bufferSize < kIOEthernetAddressSize) {
                return kernResult;
            }
            bzero(MACAddress, bufferSize);
            
            while ((intfService = IOIteratorNext(intfIterator)))
            {
                CFTypeRef	MACAddressAsCFData;
                
                kernResult = IORegistryEntryGetParentEntry(intfService,
                                                           kIOServicePlane,
                                                           &controllerService);
                
                if (KERN_SUCCESS != kernResult) {
                    printf("IORegistryEntryGetParentEntry returned 0x%08x\n", kernResult);
                }
                else {
                    // Retrieve the MAC address property from the I/O Registry in the form of a CFData
                    MACAddressAsCFData = IORegistryEntryCreateCFProperty(controllerService,
                                                                         CFSTR(kIOMACAddress),
                                                                         kCFAllocatorDefault,
                                                                         0);
                    if (MACAddressAsCFData) {
                        CFShow(MACAddressAsCFData); // for display purposes only; output goes to stderr
                        
                        // Get the raw bytes of the MAC address from the CFData
                        CFDataGetBytes((CFDataRef)MACAddressAsCFData, CFRangeMake(0, kIOEthernetAddressSize), MACAddress);
                        
                        CFRelease(MACAddressAsCFData);
                    }
                    
                    // Done with the parent Ethernet controller object so we release it.
                    (void) IOObjectRelease(controllerService);
                }
                
                // Done with the Ethernet interface object so we release it.
                (void) IOObjectRelease(intfService);
            }
            
            return kernResult;
        }

        
        static std::string hsmacAddress()
        {
            std::string macStr = "";
            kern_return_t	kernResult = KERN_SUCCESS;
            io_iterator_t	intfIterator;
            UInt8			MACAddress[kIOEthernetAddressSize];
            
            kernResult = FindEthernetInterfaces(&intfIterator);
            
            if (KERN_SUCCESS != kernResult) {
                printf("FindEthernetInterfaces returned 0x%08x\n", kernResult);
            }
            else {
                kernResult = GetMACAddress(intfIterator, MACAddress, sizeof(MACAddress));
                
                if (KERN_SUCCESS != kernResult) {
                    printf("GetMACAddress returned 0x%08x\n", kernResult);
                }
                else {
                    
                    char str2[80] = { 0 };
                    sprintf(str2, "%02X:%02X:%02X:%02X:%02X:%02X", MACAddress[0], MACAddress[1], MACAddress[2], MACAddress[3], MACAddress[4], MACAddress[5]);
                    printf("%s", str2);
                    macStr = std::string(str2);
                }
            }
            
            (void) IOObjectRelease(intfIterator);	// Release the iterator.
//            return "hoisungtestIAP15";
            return macStr;
        }
        
    };
}

#endif /*(CC_TARGET_PLATFORM == CC_PLATFORM_MAC*/


