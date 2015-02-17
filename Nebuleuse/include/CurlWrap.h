#include "curl/curl.h"
#include <string>
#include <mutex>

namespace Neb{
	size_t writer(const char *data, size_t size, size_t nmemb, std::string *buffer);

	class CurlWrap {
	public:
		CurlWrap();
		~CurlWrap();
		void addPost(const std::string &name, const std::string &content);
		std::string fetchPage(const std::string &url, bool ispost = false);
		const std::string lastError();

		void Lock();
		void Unlock();

	private:
		std::mutex mtx_;
		CURL *handle;
		curl_httppost* post;
		curl_httppost* last;
		struct curl_slist *headerlist = NULL;
		std::string postStr;
		std::string writeBuffer;

		char errorBuffer[CURL_ERROR_SIZE];
	};
}