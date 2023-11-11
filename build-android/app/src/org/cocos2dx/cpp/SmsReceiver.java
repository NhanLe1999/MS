package org.cocos2dx.cpp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import com.google.android.gms.auth.api.phone.SmsRetriever;
import com.google.android.gms.common.api.CommonStatusCodes;
import com.google.android.gms.common.api.Status;

public class SmsReceiver extends BroadcastReceiver {

    public SmsBroadcastReceiverListener smsBroadcastReceiverListener;

    @Override
    public void onReceive(Context context, Intent intent) {

        if(intent != null)
        {
            if(intent.getAction() == SmsRetriever.SMS_RETRIEVED_ACTION){

                Bundle extras = intent.getExtras();
                if(extras != null)
                {
                    Status smsRetreiverStatus = (Status) extras.get(SmsRetriever.EXTRA_STATUS);

                    switch (smsRetreiverStatus.getStatusCode()){
                        case CommonStatusCodes
                                .SUCCESS:
                            Intent messageIntent = extras.getParcelable(SmsRetriever.EXTRA_CONSENT_INTENT);
                        if(messageIntent != null)
                        {
                            smsBroadcastReceiverListener.onSuccess(messageIntent);
                        }
                            break;
                        case CommonStatusCodes.TIMEOUT:
                            smsBroadcastReceiverListener.onFailure();
                            break;

                    }
                }
            }
        }
    }

    public interface SmsBroadcastReceiverListener{

        void onSuccess(Intent intent);

        void onFailure();


    }
}
