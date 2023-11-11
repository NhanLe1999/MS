//
//  Created by nguyenhoangthienphuoc on 16/02/20.
//
#pragma once

#include "APBaseAPI.h"

using namespace ap;

class PostingReviewAPI final : public ap::api::RequestDelegate
{
public:

	static void call(
		const int & ticket_group_id,
		const std::string & comment_customer,
		const int & rateType,
		const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);

private:
    void send() const;
	
	PostingReviewAPI(
		const int & ticket_group_id,
		const std::string & comment_customer,
		const int & rateType,
		const std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> & callback = nullptr);
            
    std::function<void(bool is_success, std::string msg, const rapidjson::Value &data)> m_callback;
            
    void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
    void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
	ap::api::Base * _request = nullptr;
};
