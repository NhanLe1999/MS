//
//  GlobalConstants.h
//  MonkeyJunior
//
//  Created by developer on 2/29/16.
//
//

#ifndef GlobalConstants_h
#define GlobalConstants_h

#include "cocos2d.h"
using namespace std;

//lien quan den cau hinh khi build
static int time_update = 3600; //1 tieng update all 1 lan


/*
 storeFile          /Users/anhmantk/cocos2d-x/MonkeyNew/Edu.keystore
 storePassword      abc123
 keyAlias           mykey
 keyPassword        edu123
 */
static string android_key = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvx+99TnlD1b7oLgbg2WpaYXWRL+Yohx7VgHtbxqK2e+e3244o0/hbten44rc+Azu9uH6+Y6QK/GRu6skWGzNX/cAP0bHyms6CLZPI9UBmonXAgDmjT06q/YeeArIyiMK8VpsQ6LHi6Wn1IX8dbA9ZnjFoeEJZ8egfdFGnOQM3EH6z5wi7Py7+GsYMuISefS/hTafyHXSRC/PYXoPz9gWCAgBGUG2MYO+B6TiBDlpolmRPqmEL9Vjd6C6fQ8PugOd0ZauQQ207jPtEff8yqaufBz6vl1EHVjCBZEtU84QqsIwDCQrmeVNDY8TMZf1VS6tISerxOS+lK9WIyt3bMibBwIDAQAB";

static bool os_amazon = false;
static string rate_url_ios = "itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=1236664909";
static string rate_url_android = "https://play.google.com/store/apps/details?id=com.earlystart.android.monkeyjunior";
static string rate_url_amazon = "amzn://apps/android?p=com.earlystart.android.monkeyjunior";


static string msg_title_errorUpdateFeed = "Connection Error";
static string msg_errorConnect = "You are not connected to the Internet, please check your Internet connection.";
static string button_detail = "Try Again";
static string button_cancel = "Cancel";
static string lato_black = "fonts/Lato-Black.ttf";
static string lato_bold = "fonts/Lato-Bold.ttf";
static string lato_bold_italic = "fonts/Lato-BoldItalic.ttf";
static string lato_heavy = "fonts/Lato-Heavy.ttf";
static string lato_heavy_italic = "fonts/Lato-HeavyItalic.ttf";
static string lato_italic = "fonts/Lato-Italic.ttf";
static string lato_regular = "fonts/Lato-Regular.ttf";

static string phonic_font = "fonts/phonicfont.ttf";
static string phonic_font_vn = "fonts/arial.ttf";

//nhac hoc
static string config_reminder = "config_reminder";//true

static string general_lang = "general";

#define kTag_API_Home 201
#define kTag_API_Explore 202
#define kTag_API_Story 203
#define kTag_API_search 204
#define kTag_API_RateStories 205
#define kTag_API_getFreeStories 206
#define kTag_API_getmessage 207
#define kTag_API_deviceID 208
#define kTag_API_pushToken 209
#define kTag_API_sentDownloadSuccess 210
#define kTag_API_loadupdate 211
#define kTag_API_levelinfo 212
#define kTag_API_rated 213
#define kTag_API_appinfo 214

#define max_number_favorite 50

#endif /* GlobalConstants_h */
