#include "Test.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <ppltasks.h>



using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;


namespace daemon {
	pplx::task<void> Test::test()
	{
		http_client client(L"http://127.0.0.1/api/repo/e3c5337a-9149-4539-bd8a-d8974529841c/lock");
		return client.request(methods::GET).then([](http_response response) -> pplx::task<json::value>
		{
			if (response.status_code() == status_codes::OK)
			{
				std::cout << "status ok";
				response.headers().set_content_type(L"application/json");

				return response.extract_json();
			}

			// Handle error cases, for now return empty json value... 


			std::cout << "get json data";

			return pplx::task_from_result(json::value());
		})
			.then([](pplx::task<json::value> previousTask)
		{
			try
			{
				std::cout << "parse json data";

				const json::value& v = previousTask.get();


				utility::string_t jsonval = v.serialize();

				json::value root;

				//  DisplayJSONValue(v);

				std::wcout << jsonval;
				//   wcout<<jsonval.data.id;
					// Perform actions here to process the JSON value...

					//std::cout << v;

					//IterateJSONValue();


			}
			catch (const http_exception& e)
			{
				// Print error.
				std::wostringstream ss;
				ss << e.what() << std::endl;
				std::wcout << ss.str();
			}
		});
	}
}