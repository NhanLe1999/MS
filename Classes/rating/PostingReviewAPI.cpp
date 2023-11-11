//
//  Created by nguyenhoangthienphuoc on 16/02/20.
//

#include "PostingReviewAPI.h"
#include "cocos2d.h"
#include "APStoreErrorAPI.h"
#include "StoryDataModel.h"
#include "APAuth.h"

using namespace ap;

void PostingReviewAPI::call(
	const int & ticket_group_id,
	const std::string & comment_customer,
	const int & rateType,
	const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback)
{
	api::APIConfig config = api::APIConfig();
	ap::api::Base::setGlobalConfiguration(config);
	auto refresh_call = new PostingReviewAPI(ticket_group_id, comment_customer, rateType, callback);
	refresh_call->send();
}

PostingReviewAPI::PostingReviewAPI(
		const int & ticket_group_id,
		const std::string & comment_customer,
		const int & rateType,
		const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback) :
		m_callback(callback)
{
    _request = new api::Base(api::PlatformType::NEW, "account/send-rate-app", 
	{
		{"ticket_group_id",ticket_group_id==-1?"":std::to_string(ticket_group_id)},
		{"comment_customer", comment_customer},
		{"status_rate", std::to_string(rateType)}
    }, api::RequestType::POST);
    _request->setDelegate(this);
    _request->enableLog();
}
        
void PostingReviewAPI::send() const
{
    if (_request) 
	{
        _request->send();
    }
}
        
void PostingReviewAPI::onRequestSuccess(int /*tag*/, int /*code*/, const std::string & message, const rapidjson::Value & data)
{
	int user_id = 0;

	if (m_callback) 
	{
        m_callback(true, message, data);
    }

    delete this;
}

void PostingReviewAPI::onRequestError(int /*tag*/, int /*error_code*/, const std::string & message, const rapidjson::Value & data)
{
	api::StoreError::call(_request->getURL(), message,"account/send-rate-app", _request->getParam());
            
	if (m_callback)
	{
		m_callback(false, message, data);
    }

    delete this;
}
