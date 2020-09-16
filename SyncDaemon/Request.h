#pragma once

#include "Response.h"

#include "TokenController.h"

namespace daemon {
	class Request {
	public:
		template <class _Fx, class... _Types>
		static std::shared_ptr<Response> process(_Fx&& _Func, _Types&& ... _Args) {
			auto call = std::_Binder<std::_Unforced, _Fx, _Types...>(_STD forward<_Fx>(_Func), _STD forward<_Types>(_Args)...);
			
		/*	Response* response = NULL;
			bool status = true;
			Response newResponse = call();
			response = &newResponse;*/

			std::shared_ptr<Response> resp;
			
			bool success = true;
			do {
				resp = call();
				if (!resp) {
					success = false;
					continue;
				}
				success = true;
				/*
				Response* newResponse = response.get();
				
				std::cout << "c ";
				response = resp.get();
				Response newResponse = call();
				response = new Response(false, L"test", web::json::value::null());
				status = response->isSuccesed();
				if (response->getMessage() == L"EMPTY_TOKEN") {
					std::cout << "EMPTY_TOKEN" << std::endl;
					Request::process(TokenController::getToken, L"rettiwerpytania@gmail.com", L"123456");
					continue;
				}
				return ;*/
			} while (!success);

			

			return resp;
		}
	};
}