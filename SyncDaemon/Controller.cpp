#include "Controller.h"

namespace daemon {

	std::shared_ptr<Response> Controller::executeRequest(std::vector<std::wstring> uri, method method, bool authorizationToken, json::value body) {
		std::shared_ptr<Response> response;

		http_client httpClient(prepareUrl(uri));

		http_request req(method);

		if (authorizationToken)
			req.headers().add(L"Authorization", L"token");

		if (body != NULL)
			req.set_body(body);

		req.headers().add(L"Accept", L"application/json");

		httpClient.request(req).then([&response](http_response httpResponse) -> pplx::task<json::value>
		{
			if (httpResponse.status_code() == status_codes::Unauthorized || httpResponse.status_code() == status_codes::OK)
				return httpResponse.extract_json();

			std::wostringstream ss;
			ss << "Error code: " << httpResponse.status_code() << ", message: " << httpResponse.extract_string().get();
			
			response = std::make_shared<Response>(Response(false, ss.str(), json::value::null()));
			return pplx::task_from_result(json::value());
		}).then([&response](pplx::task<json::value> previousTask) mutable {
			try
			{
				if (previousTask.get().is_null())
					return;

				const json::value json = previousTask.get();

				response = processResponse(json);
			}
			catch (const http_exception& e)
			{
				std::wostringstream ss;
				ss << e.what() << std::endl;
				
				response = std::make_shared<Response>(Response(false, ss.str(), json::value::null()));
			}
		}).get();
		return response;
	}

	std::shared_ptr<Response> Controller::processResponse(web::json::value json) {
		bool success = json.at(L"success").as_bool();
		std::wstring message = json.at(L"message").as_string();
		if (json.has_object_field(L"data")) {
			json::value data = json.at(L"data");
			
			return std::make_shared<Response>(Response(success, message, data));
		}
		return std::make_shared<Response>(Response(success, message, json::value::null()));
	}


	std::wstring Controller::prepareUrl(std::vector<std::wstring> uri) {
		std::wstringstream urlBuilder;
		urlBuilder << L"http://127.0.0.1/api/";
		for (std::wstring part : uri) {
			urlBuilder << part;
		}
		return urlBuilder.str();
	}
}