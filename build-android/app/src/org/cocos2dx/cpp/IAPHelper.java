package org.cocos2dx.cpp;

/*
 * Copyright 2020 Google LLC. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import android.app.Activity;
import android.app.Application;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.lifecycle.MutableLiveData;

import com.android.billingclient.api.AcknowledgePurchaseParams;
import com.android.billingclient.api.AcknowledgePurchaseResponseListener;
import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.PurchaseHistoryRecord;
import com.android.billingclient.api.PurchaseHistoryResponseListener;
import com.android.billingclient.api.PurchasesResponseListener;
import com.android.billingclient.api.PurchasesUpdatedListener;
import com.android.billingclient.api.SkuDetails;
import com.android.billingclient.api.SkuDetailsParams;
import com.android.billingclient.api.SkuDetailsResponseListener;
import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;

import static com.android.billingclient.api.BillingFlowParams.ProrationMode.DEFERRED;


public class IAPHelper implements PurchasesUpdatedListener, BillingClientStateListener, SkuDetailsResponseListener, PurchasesResponseListener, PurchaseHistoryResponseListener {

    enum ExcuteType
    {
        QUERY_PURCHASE_HISTORY_TO_INIT,
        QUERY_PURCHASE_HISTORY_TO_VERIFY,
        QUERY_PURCHASE_HISTORY_TO_PURCHASE,
        QUERY_PURCHASE_HISTORY_TO_RESTORE,
    };

    static final String[] SKUs = {
            "new.earlystart.stories.1month.40_off",
            "new.earlystart.stories.6month.40_off",
            "trial.earlystart.stories.1year.40_off",
            "new.earlystart.stories.1year.40_off",
            "new.earlystart.stories.lifetime.40_off",
            "new.earlystart.stories.1year.promotion",
            "new.earlystart.stories.6month.promotion",
            "new.earlystart.stories.1month.promotion",
            "new.earlystart.storie.lifetime.promotion"


    };

    static final String[] product_ids = {
            "new.earlystart.stories.1month.40_off",
            "new.earlystart.stories.6month.40_off",
            "trial.earlystart.stories.1year.40_off",
            "new.earlystart.stories.1year.40_off",
            "new.earlystart.stories.lifetime.40_off",
            "new.earlystart.stories.1year.promotion",
            "new.earlystart.stories.6month.promotion",
            "new.earlystart.stories.1month.promotion",
            "new.earlystart.storie.lifetime.promotion"
    };

    static final String[] SKUs_old = {
            "new.earlystart.stories.1month.40_off",
            "new.earlystart.stories.6month.40_off",
            "trial.earlystart.stories.1year.40_off",
            "new.earlystart.stories.1year.40_off",
            "new.earlystart.stories.lifetime.40_off",
            "new.earlystart.stories.1year.promotion",
            "new.earlystart.stories.6month.promotion",
            "new.earlystart.stories.1month.promotion",
            "new.earlystart.storie.lifetime.promotion"
    };

    private static final String TAG = "BillingLifecycle";

    private String _deferredPackageID = "";

    public List<PurchaseInfo> purchasedList                 = new ArrayList<>();

    public List<SkuDetails> skusWithSkuDetails              = new ArrayList<>();

    public static List<String>  _purchasedPackageIDs        = new ArrayList<>();//get from monkey server.

    private static AppActivity app                          = AppActivity._ac;

    private int skuDetailCount                              = 0; //INAPP,SUBS

    private  ExcuteType _excuteType                        = ExcuteType.QUERY_PURCHASE_HISTORY_TO_INIT;

    private static String _currentPurchasePackageID        = "";

    private static volatile IAPHelper INSTANCE;

    private static int _queryPurchaseHistoryCount                 = 0; //INAPP,SUBS

    private BillingClient billingClient;

    private IAPHelper() {
    }

    public static IAPHelper getInstance() {
        if (INSTANCE == null) {
            synchronized (IAPHelper.class) {
                if (INSTANCE == null) {
                    INSTANCE = new IAPHelper();
                    INSTANCE.create();
                }
            }
        }
        return INSTANCE;
    }

    public void create()
    {

        _purchasedPackageIDs.clear();

        _currentPurchasePackageID = "";

        Log.d(TAG, "ON_CREATE");
        billingClient = BillingClient.newBuilder(app)
                .setListener(this)
                .enablePendingPurchases() // Not used for subscriptions.
                .build();
        if (!billingClient.isReady()) {
            Log.d(TAG, "BillingClient: Start connection...");
            billingClient.startConnection(this);
        }
    }

    public void destroy()
    {
        Log.d(TAG, "ON_DESTROY");
        if (billingClient.isReady()) {
            Log.d(TAG, "BillingClient can only be used once -- closing connection");
            billingClient.endConnection();
        }
    }

    //startConnection
    @Override
    public void onBillingSetupFinished(BillingResult billingResult) {
        int responseCode = billingResult.getResponseCode();
        String debugMessage = billingResult.getDebugMessage();
        Log.d(TAG, "onBillingSetupFinished: " + responseCode + " " + debugMessage);
        if (responseCode == BillingClient.BillingResponseCode.OK) {
            // The billing client is ready. You can query purchases here.
            querySkuDetails();
            queryPurchaseHistory(ExcuteType.QUERY_PURCHASE_HISTORY_TO_INIT);
        }
    }

    //startConnection
    @Override
    public void onBillingServiceDisconnected() {
        Log.d(TAG, "onBillingServiceDisconnected");
        // TODO: Try connecting again with exponential backoff.
    }

    public void querySkuDetails() {

        skuDetailCount = 0;

        List<String> oneTimeProducts        = new ArrayList<>();

        List<String> subscriptionProducts   = new ArrayList<>();

        for (String sku : product_ids) {

            if (sku.contains("lifetime")) {
                oneTimeProducts.add(sku);
            } else {
                subscriptionProducts.add(sku);
            }
        }

        SkuDetailsParams oneTimeProductsParams = SkuDetailsParams.newBuilder()
                .setType(BillingClient.SkuType.INAPP)
                .setSkusList(oneTimeProducts)
                .build();
        billingClient.querySkuDetailsAsync(oneTimeProductsParams, this);

        SkuDetailsParams subscriptionProductsParams = SkuDetailsParams.newBuilder()
                .setType(BillingClient.SkuType.SUBS)
                .setSkusList(subscriptionProducts)
                .build();
        billingClient.querySkuDetailsAsync(subscriptionProductsParams, this);

    }

    //querySkuDetailsAsync. Lay thong tin cac goi mua tu store ve.
    @Override
    public void onSkuDetailsResponse(BillingResult billingResult, List<SkuDetails> skuDetailsList) {
        if (billingResult == null) {
            Log.wtf(TAG, "onSkuDetailsResponse: null BillingResult");
            return;
        }

        int responseCode    = billingResult.getResponseCode();
        String debugMessage = billingResult.getDebugMessage();
        switch (responseCode) {
            case BillingClient.BillingResponseCode.OK:
            {
                Log.i(TAG, "onSkuDetailsResponse: " + responseCode + " " + debugMessage);
                final int expectedSkuDetailsCount = Arrays.asList(product_ids).size();
                if (skuDetailsList == null) {
                    if(!_currentPurchasePackageID.isEmpty()) onCppBillingError("<onSkuDetailsResponse><OK> | skuDetailsList == null");
                    Log.e(TAG, "onSkuDetailsResponse: " +
                            "Expected " + expectedSkuDetailsCount + ", " +
                            "Found null SkuDetails. " +
                            "Check to see if the SKUs you requested are correctly published " +
                            "in the Google Play Console.");
                } else {

                    for (SkuDetails skuDetails : skuDetailsList) {

                        if(!skusWithSkuDetails.contains(skuDetails)) {
                            skusWithSkuDetails.add(skuDetails);
                        }
                    }

                    skuDetailCount+=1;

                    if(skuDetailCount >= 2) {

                        JSONArray jsonArray = new JSONArray();

                        for (SkuDetails skuDetails : skusWithSkuDetails) {

                            JSONObject jsonObject = new JSONObject();

                            if (skuDetails != null) {
                                try {
                                    jsonObject.put("product_id", skuDetails.getSku());
                                    jsonObject.put("price_sale", skuDetails.getPriceAmountMicros());//109k => 109000000000
                                    jsonObject.put("currency", skuDetails.getPriceCurrencyCode());
                                } catch (JSONException e) {
                                    continue;
                                }

                                jsonArray.put(jsonObject);
                            }

                        }

                        String jsonProducts = jsonArray.toString();

                        if(!_currentPurchasePackageID.isEmpty()) //truong hop query lai de lay thong tin goi mua.
                        {
                            onPurchase(_currentPurchasePackageID);
                        }
                        else{
                            onCppBillingInitialized(jsonProducts);
                        }

                    }
                }
                break;
            }
            case BillingClient.BillingResponseCode.BILLING_UNAVAILABLE:
            case BillingClient.BillingResponseCode.DEVELOPER_ERROR:
            case BillingClient.BillingResponseCode.ERROR:
            case BillingClient.BillingResponseCode.ITEM_UNAVAILABLE:
            case BillingClient.BillingResponseCode.FEATURE_NOT_SUPPORTED:
            case BillingClient.BillingResponseCode.USER_CANCELED:
                if(!_currentPurchasePackageID.isEmpty()) onCppBillingError("show_get_info_popup");
                break;
            case BillingClient.BillingResponseCode.ITEM_ALREADY_OWNED:
                if(!_currentPurchasePackageID.isEmpty()) onCppBillingError("ITEM_ALREADY_OWNED");
                break;
            case BillingClient.BillingResponseCode.SERVICE_DISCONNECTED:
                if(!_currentPurchasePackageID.isEmpty()) onCppBillingError("subscription.error.network");//onCppBillingError("<onSkuDetailsResponse><SERVICE_DISCONNECTED> | Play Store service is not connected now!");
                break;
            case BillingClient.BillingResponseCode.SERVICE_UNAVAILABLE:
                if(!_currentPurchasePackageID.isEmpty()) onCppBillingError("subscription.error.network");//onCppBillingError("<onSkuDetailsResponse><SERVICE_UNAVAILABLE> | Network connection is down!");
                break;
            case BillingClient.BillingResponseCode.SERVICE_TIMEOUT:
                if(!_currentPurchasePackageID.isEmpty()) onCppBillingError("subscription.error.network");//onCppBillingError("<onSkuDetailsResponse><SERVICE_TIMEOUT> | The request has reached the maximum timeout before Google Play responds.");
                break;
            default:
                if(!_currentPurchasePackageID.isEmpty()) onCppBillingError("subscription.error.network");//onCppBillingError("<onSkuDetailsResponse><UNDEFINED> | Some thing wrong!");
        }
    }

    //lay nhung goi da mua.
    public void queryPurchases() {

        if (!billingClient.isReady()) {
            onCppBillingError("subscription.error.billingclient");//("BillingClient is not ready");
        }

        billingClient.queryPurchasesAsync(BillingClient.SkuType.SUBS, this);
        billingClient.queryPurchasesAsync(BillingClient.SkuType.INAPP, this);
    }

    //queryPurchasesAsync
    @Override
    public void onQueryPurchasesResponse(@NonNull BillingResult billingResult,
                                         @NonNull List<Purchase> list) {
        processPurchases(list,false);
    }

    //launchBillingFlow
    public void onPurchasesUpdated(BillingResult billingResult, List<Purchase> purchases) {

        if (billingResult == null) {
            //onCppBillingError("<onPurchasesUpdated> billingResult is null!");
            return;
        }

        int responseCode = billingResult.getResponseCode();

        String debugMessage = billingResult.getDebugMessage();

        Log.d(TAG, String.format("onPurchasesUpdated: %s %s",responseCode, debugMessage));

        switch (responseCode) {
            case BillingClient.BillingResponseCode.OK:
                processPurchases(purchases,true);
                break;
            case BillingClient.BillingResponseCode.BILLING_UNAVAILABLE:
            case BillingClient.BillingResponseCode.DEVELOPER_ERROR:
            case BillingClient.BillingResponseCode.ERROR:
            case BillingClient.BillingResponseCode.FEATURE_NOT_SUPPORTED:
            case BillingClient.BillingResponseCode.USER_CANCELED:
                onCppBillingError("show_get_info_popup");
                break;
            case BillingClient.BillingResponseCode.ITEM_ALREADY_OWNED:
                onCppBillingError("ITEM_ALREADY_OWNED");
                break;
            case BillingClient.BillingResponseCode.SERVICE_DISCONNECTED:
                onCppBillingError("subscription.error.network");//onCppBillingError("<onPurchasesUpdated>Play Store service is not connected now!");
                break;
            case BillingClient.BillingResponseCode.SERVICE_UNAVAILABLE:
                onCppBillingError("subscription.error.network");//onCppBillingError("<onPurchasesUpdated>Network connection is down!");
                break;
            case BillingClient.BillingResponseCode.SERVICE_TIMEOUT:
                onCppBillingError("subscription.error.network");//onCppBillingError("<onPurchasesUpdated>The request has reached the maximum timeout before Google Play responds.");
                break;
        }
    }

    private void processPurchases(List<Purchase> purchasesList, boolean isBuy) {

        //truong hop upgrade/downgrade goi => purchasesList == null
        if(purchasesList == null || purchasesList.isEmpty()) {

            if(isBuy)
            {
                onCppProductPurchased(String.format("{\"defer_pid\":\"%s\"}",_deferredPackageID.isEmpty()?"empty":_deferredPackageID));
            }

            return;
        }

        if(isBuy) {

            for(final Purchase purchase: purchasesList) {

                int purchaseState = purchase.getPurchaseState();

                if (purchaseState == Purchase.PurchaseState.PURCHASED && !purchase.isAcknowledged()) {

                    //neu cua xac thuc goi => phai xac thuc.
                    billingClient.acknowledgePurchase(AcknowledgePurchaseParams.newBuilder()
                            .setPurchaseToken(purchase.getPurchaseToken())
                            .build(), new AcknowledgePurchaseResponseListener() {
                        @Override
                        public void onAcknowledgePurchaseResponse(@NonNull BillingResult billingResult) {
                            int responseCode = billingResult.getResponseCode();

                            if (responseCode == BillingClient.BillingResponseCode.OK) {

                                PurchaseInfo info = new PurchaseInfo(purchase.getSkus().get(0), purchase.getPurchaseToken(), purchase.getPurchaseTime());

                                if (!purchasedList.contains(info)) {
                                    purchasedList.add(info);
                                }

                                String productID = "";

                                for (String sku : purchase.getSkus()) {

                                    if (Arrays.asList(product_ids).contains(sku)) {

                                        productID = sku;

                                        break;
                                    }
                                }

                                JSONObject jsonObject = new JSONObject();

                                try {
                                    jsonObject.put("order_id", purchase.getOrderId());
                                    jsonObject.put("product_id", productID);
                                    jsonObject.put("purchase_token", purchase.getPurchaseToken());
                                    jsonObject.put("auto_renew", purchase.isAutoRenewing() ? "yes" : "no");
                                } catch (JSONException e) {
                                    e.printStackTrace();
                                    return;
                                }

                                onCppProductPurchased(jsonObject.toString());
                            } else {
                                switch (responseCode) {
                                    case BillingClient.BillingResponseCode.BILLING_UNAVAILABLE:
                                    case BillingClient.BillingResponseCode.DEVELOPER_ERROR:
                                    case BillingClient.BillingResponseCode.ERROR:
                                    case BillingClient.BillingResponseCode.FEATURE_NOT_SUPPORTED:
                                    case BillingClient.BillingResponseCode.USER_CANCELED:
                                        onCppBillingError("show_get_info_popup");
                                        break;
                                    case BillingClient.BillingResponseCode.ITEM_ALREADY_OWNED://store se thong bao => minh ko can thong bao.
                                        onCppBillingError("ITEM_ALREADY_OWNED");
                                        break;
                                    case BillingClient.BillingResponseCode.SERVICE_DISCONNECTED:
                                        onCppBillingError("subscription.error.network");// onCppBillingError("<acknowledgePurchase> Play Store service is not connected now!");
                                        break;
                                    case BillingClient.BillingResponseCode.SERVICE_UNAVAILABLE:
                                        onCppBillingError("subscription.error.network");// onCppBillingError("<acknowledgePurchase> Network connection is down!");
                                        break;
                                    case BillingClient.BillingResponseCode.SERVICE_TIMEOUT:
                                        onCppBillingError("subscription.error.network");//onCppBillingError("<acknowledgePurchase> The request has reached the maximum timeout before Google Play responds.");
                                        break;
                                    default:
                                        onCppBillingError("subscription.error.network");//onCppBillingError("<acknowledgePurchase> Something wrong!");
                                        break;
                                }
                            }
                        }
                    });
                }
                else if (purchaseState == Purchase.PurchaseState.PURCHASED &&  purchase.isAcknowledged())
                {
                    PurchaseInfo info = new PurchaseInfo(purchase.getSkus().get(0),purchase.getPurchaseToken(),purchase.getPurchaseTime());

                    if(!purchasedList.contains(info)) {
                        purchasedList.add(info);
                    }

                    String productID = "";

                    for(String sku : purchase.getSkus()) {

                        if(Arrays.asList(product_ids).contains(sku)) {

                            productID = sku;

                            break;
                        }
                    }

                    JSONObject jsonObject = new JSONObject();

                    try
                    {
                        jsonObject.put("order_id", purchase.getOrderId());
                        jsonObject.put("product_id", productID);
                        jsonObject.put("purchase_token", purchase.getPurchaseToken());
                        jsonObject.put("auto_renew", purchase.isAutoRenewing() ? "yes" : "no");
                    }
                    catch (JSONException e)
                    {
                        e.printStackTrace();
                        return;
                    }

                    onCppProductPurchased(jsonObject.toString());
                }
            }

        } else {

            for(final Purchase purchase: purchasesList) {

                PurchaseInfo info = new PurchaseInfo(purchase.getSkus().get(0),purchase.getPurchaseToken(),purchase.getPurchaseTime());

                if(!purchasedList.contains(info)) {
                    purchasedList.add(info);
                }
            }
        }
    }

    public void onPurchase(String packageID) {

        if (!billingClient.isReady()) {
            onCppBillingError("subscription.error.billingclient");//onCppBillingError("Billingclient chua ready,khong the tien hanh mua!");
            return;
        }

        _currentPurchasePackageID = packageID;

        _deferredPackageID = "";

        //lay thong tin goi mua tu android store dua tren packageID.
        SkuDetails skuDetails = null;

        for(SkuDetails s:skusWithSkuDetails ) {

            if(s.getSku().equals(packageID)) {
                skuDetails = s;
            }
        }

        //neu ko get duoc thong tin goi => query lai.
        if (skuDetails == null) {

            querySkuDetails();

            return;
        }

        BillingFlowParams billingFlowParams = null;

        if(!_purchasedPackageIDs.isEmpty()&& !packageID.contains("alllanguage")) //monkey server tra ve nhung goi da mua roi
        {
            List<PurchaseInfo> purchasedPackageInfos = new ArrayList<>();

            //tim thong tin goi da mua trong nhung goi cua store tra ve.
            for(String id: _purchasedPackageIDs) {

                for(PurchaseInfo p :  purchasedList) {

                    if(p.getProductId().equals(id)) {
                        purchasedPackageInfos.add(p);
                        break;
                    }
                }
            }

            //neu tim thay => se lay goi mua gan nhat.
            if(!purchasedPackageInfos.isEmpty()) {

                Collections.sort(purchasedPackageInfos, new Comparator<PurchaseInfo>() {

                    @Override
                    public int compare(PurchaseInfo o1, PurchaseInfo o2) {
                        return (o1.getPurchaseTime() > o2.getPurchaseTime() ? 1 : -1);
                    }
                });

                PurchaseInfo p = purchasedPackageInfos.get(purchasedPackageInfos.size()-1);

                boolean isLifeTimePackage = p.getProductId().contains("allanguage");

                if(isLifeTimePackage) {
                    onCppBillingError("<onPurchase>You purchased the lifetime package. Cannot buy more! Please press the restore button to restore your liscene. ");
                    return;
                }
                else
                {
                    if(packageID.equals(p.getProductId())) {

                        billingFlowParams = BillingFlowParams.newBuilder()
                                .setSkuDetails(skuDetails)
                                .build();
                    }
                    else
                    {
                        _deferredPackageID = skuDetails.getSku();

                        BillingFlowParams.SubscriptionUpdateParams.Builder sp = BillingFlowParams.SubscriptionUpdateParams.newBuilder()
                                .setOldSkuPurchaseToken(p.getPurchaseToken())
                                .setReplaceSkusProrationMode(DEFERRED);

                        billingFlowParams = BillingFlowParams.newBuilder()
                                .setSkuDetails(skuDetails)
                                .setSubscriptionUpdateParams(sp.build())
                                .build();
                    }
                }
            }
            else //khong tim thay duoc => goi nang cap
            {
                //1.NAG CAP DEVICE LEN ACCOUNT
                billingFlowParams = BillingFlowParams.newBuilder()
                        .setSkuDetails(skuDetails)
                        .build();
                //2.CHUA GET DUOC THONG TIN DO TU PHIA GOOGLE PLAY.
                //onCppBillingError("<onPurchase>We deteted purchased packages from Monkey server, but we can't get purchased packages from the CH-Play Store! Please delete CH-Play app data and try again! ");
                //return;
            }
        }
        else  //monkey server detect duoc user chua mua goi nao
        {

//            if(!purchasedList.isEmpty()) {
//
//                billingFlowParams = BillingFlowParams.newBuilder()
//                        .setSkuDetails(skuDetails)
//                        .build();
//            }
//            else
//            {
//                billingFlowParams = BillingFlowParams.newBuilder()
//                        .setSkuDetails(skuDetails)
//                        .build();
//            }

            billingFlowParams = BillingFlowParams.newBuilder()
                    .setSkuDetails(skuDetails)
                    .build();
        }

        BillingResult billingResult = billingClient.launchBillingFlow( this.app , billingFlowParams);

        int responseCode = billingResult.getResponseCode();

        String debugMessage = billingResult.getDebugMessage();

        Log.d(TAG, "launchBillingFlow: BillingResponse " + responseCode + " " + debugMessage);
    }

    public void onRestore()
    {
        _currentPurchasePackageID = "";
        queryPurchaseHistory(ExcuteType.QUERY_PURCHASE_HISTORY_TO_RESTORE);
    }

    public String getRecentPurchasedPackageToken() {

        String json = "{\"product_id\":\"\",\"purchase_token\":\"\" }";

        if(purchasedList.isEmpty() ) {
            return json;
        }

        //uu tien => nhung package dang con han => dua tren server tra ve.
        if(!_purchasedPackageIDs.isEmpty()) {

            List<PurchaseInfo> actualPurchasedPackageList = new ArrayList<>();

            for(String packageID: _purchasedPackageIDs) {

                for(PurchaseInfo p : purchasedList) {

                    if(packageID.equals(p.getProductId())) {

                        actualPurchasedPackageList.add(p);
                        break;
                    }
                }
            }

            if(!actualPurchasedPackageList.isEmpty()) {

                Collections.sort(actualPurchasedPackageList, new Comparator<PurchaseInfo>() {

                    @Override
                    public int compare(PurchaseInfo o1, PurchaseInfo o2) {
                        return (o1.getPurchaseTime() > o2.getPurchaseTime() ? 1 : -1);
                    }
                });

                PurchaseInfo recentPackage = actualPurchasedPackageList.get(actualPurchasedPackageList.size()-1);

                if (!recentPackage.getProductId().isEmpty()) {
                    String pID = recentPackage.getProductId();
                    String token = recentPackage.getPurchaseToken();
                    json = String.format("{\"product_id\":\"%s\",\"purchase_token\":\"%s\" }", pID, token);
                    return json;
                }
            }
        }

        Collections.sort(purchasedList, new Comparator<PurchaseInfo>() {

            @Override
            public int compare(PurchaseInfo o1, PurchaseInfo o2) {
                return (o1.getPurchaseTime() > o2.getPurchaseTime() ? 1 : -1);
            }
        });

        PurchaseInfo recentPackage = purchasedList.get(purchasedList.size()-1);

        if (!recentPackage.getProductId().isEmpty()) {
            String pID = recentPackage.getProductId();
            String token = recentPackage.getPurchaseToken();
            json = String.format("{\"product_id\":\"%s\",\"purchase_token\":\"%s\" }", pID, token);
        }

        return json;
    }

    public void queryPurchaseHistory(ExcuteType type) {

        if (!billingClient.isReady()) {
            onCppBillingError("subscription.error.billingclient");
            return;
        }

        _queryPurchaseHistoryCount = 0;

        _excuteType = type;

        billingClient.queryPurchaseHistoryAsync(BillingClient.SkuType.SUBS, this);
        billingClient.queryPurchaseHistoryAsync(BillingClient.SkuType.INAPP, this);
    }

    public static void updatePurchasedPackageIdsFromMonkeyServer(String i_purchasedPackageIDs)
    {
        _purchasedPackageIDs.clear();

        if(!i_purchasedPackageIDs.isEmpty()) {

            Map<String, Object> mapPurchasedPackageIDsFromServer  = new Gson().fromJson(i_purchasedPackageIDs, new TypeToken<HashMap<String, Object>>(){}.getType() );

            if(mapPurchasedPackageIDsFromServer.containsKey("package_ids"))
            {
                try
                {
                    List<String> packageIds = (ArrayList)mapPurchasedPackageIDsFromServer.get("package_ids");

                    if(!packageIds.isEmpty()) {

                        for(String id:packageIds) {

                            if(!_purchasedPackageIDs.contains(id)) {
                                _purchasedPackageIDs.add(id);
                            }
                        }
                    }

                }
                catch (Exception e) {

                }
            }

        }
    }

    public static void sPurchase(String packageID,String purchasedPackageIDs) {

        _currentPurchasePackageID = "";

        updatePurchasedPackageIdsFromMonkeyServer(purchasedPackageIDs);

        IAPHelper.getInstance().onPurchase(packageID);
    }

    public static void sRestore(String purchasedPackageIDs) {

        updatePurchasedPackageIdsFromMonkeyServer(purchasedPackageIDs);

        IAPHelper.getInstance().onRestore();
    }

    public static void sVerify(String purchasedPackageIDs ) {

        Log.d(TAG, "sVerifyyyyyy");

        updatePurchasedPackageIdsFromMonkeyServer(purchasedPackageIDs);

        IAPHelper.getInstance().queryPurchaseHistory(ExcuteType.QUERY_PURCHASE_HISTORY_TO_VERIFY);
    }

    public static native void onCppBillingInitialized(String json_products);
    public static native void onCppBillingVerify(String json_products);
    public static native void onCppBillingError(String json_products);
    public static native void onCppProductPurchased(String details);
    public static native void onCppPurchaseHistoryRestored(String details);

    //queryPurchaseHistoryAsync
    @Override
    public void onPurchaseHistoryResponse(@NonNull BillingResult billingResult, @Nullable List<PurchaseHistoryRecord> purchases) {

        if (billingResult == null) {
            Log.wtf(TAG, "onPurchasesUpdated: null BillingResult");
            return;
        }

        _queryPurchaseHistoryCount += 1;

        int responseCode = billingResult.getResponseCode();

        String debugMessage = billingResult.getDebugMessage();

        Log.d(TAG, String.format("onPurchasesUpdated: %s %s",responseCode, debugMessage));

        switch (responseCode) {

            case BillingClient.BillingResponseCode.OK: {

                if(purchases!= null && !purchases.isEmpty()) {

                    List<PurchaseHistoryRecord> mjPurchasedList = new ArrayList<>();

                    for(PurchaseHistoryRecord p :purchases ) {

                        if(Arrays.asList(product_ids).contains(p.getSkus().get(0))) {

                            mjPurchasedList.add(p);
                        }
                    }

                    if(!mjPurchasedList.isEmpty()) {

                        for(PurchaseHistoryRecord p : mjPurchasedList) {
                            PurchaseInfo pInfo = new PurchaseInfo(p.getSkus().get(0),p.getPurchaseToken(),p.getPurchaseTime());
                            if(!purchasedList.contains(pInfo)) {
                                purchasedList.add(pInfo);
                            }
                        }
                    }
                }

                if(_queryPurchaseHistoryCount >=2 ) {

                    _queryPurchaseHistoryCount = 0;

                    if(_excuteType == ExcuteType.QUERY_PURCHASE_HISTORY_TO_VERIFY) {

                        onCppBillingVerify(getRecentPurchasedPackageToken());
                    }
                    else if(_excuteType == ExcuteType.QUERY_PURCHASE_HISTORY_TO_RESTORE) {

                        onCppPurchaseHistoryRestored(getRecentPurchasedPackageToken());
                    }
                }

                break;
            }
            case BillingClient.BillingResponseCode.BILLING_UNAVAILABLE:
            case BillingClient.BillingResponseCode.DEVELOPER_ERROR:
            case BillingClient.BillingResponseCode.ERROR:
            case BillingClient.BillingResponseCode.FEATURE_NOT_SUPPORTED:
            case BillingClient.BillingResponseCode.USER_CANCELED:
                onCppBillingError("show_get_info_popup");
                break;
            case BillingClient.BillingResponseCode.ITEM_ALREADY_OWNED:
                onCppBillingError("ITEM_ALREADY_OWNED");
                break;
            case BillingClient.BillingResponseCode.SERVICE_DISCONNECTED:
                onCppBillingError("subscription.error.network");
                //onCppBillingError("<onPurchaseHistoryResponse> Play Store service is not connected now!");
                break;
            case BillingClient.BillingResponseCode.SERVICE_UNAVAILABLE:
                onCppBillingError("subscription.error.network");
                //onCppBillingError("<onPurchaseHistoryResponse> Network connection is down!");
                break;
            case BillingClient.BillingResponseCode.SERVICE_TIMEOUT:
                onCppBillingError("subscription.error.network");
                //onCppBillingError("<onPurchaseHistoryResponse> The request has reached the maximum timeout before Google Play responds.");
                break;
        }
    }
}
