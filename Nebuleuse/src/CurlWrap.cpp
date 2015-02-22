#include "CurlWrap.h"
#include "curl\curl.h"

namespace Neb{
	size_t writer(const char *data, size_t size, size_t nmemb, std::string *buffer) {
		size_t result = 0;

		if (buffer != NULL) {
			buffer->append(data, size*nmemb);
			result = size * nmemb;
		}

		return result;
	}
	CurlWrap::~CurlWrap() {
		/* Clean up everything */
		curl_easy_cleanup(handle);
		curl_global_cleanup();
	}
	void CurlWrap::addPost(const std::string &name, const std::string &content) {
		postStr.append(name);
		postStr.append("=");
		postStr.append(content);
		postStr.append("&");
		return;
	}
	std::string CurlWrap::fetchPage(const std::string &url, bool ispost) {
		if (ispost) {
			curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postStr.c_str());
		}
		curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

		if (curl_easy_perform(handle) == CURLE_OK) {
			if (ispost) {
				postStr = "";
			}
			std::string retval(writeBuffer);
			writeBuffer = "";

			return retval;
		}
		else {
			return "";
		}
	}

	const std::string CurlWrap::lastError() {
		return errorBuffer;
	}

	CurlWrap::CurlWrap() {
		/* Initiate libcurl */
		curl_global_init(CURL_GLOBAL_ALL);

		/* Get connection handle */
		handle = curl_easy_init();

		/* Set all standard options */
		curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, &errorBuffer);
		curl_easy_setopt(handle, CURLOPT_HEADER, 0);
		curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, &writeBuffer);

		headerlist = curl_slist_append(headerlist, "Expect: 100-continue");
		curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(handle, CURLOPT_VERBOSE, 0L);

		postStr = "";

		/* NULL out post variables */
		post = NULL;
		last = NULL;
		errorBuffer[0] = '\0';
		writeBuffer = "";
	}
	void CurlWrap::Lock(){
		mtx_.lock();
	}
	void CurlWrap::Unlock(){
		mtx_.unlock();
	}

}