
#ifndef CURL_EXT_UTILS_CURLAPI_H
#define CURL_EXT_UTILS_CURLAPI_H


#include "curl/curl.h"
#include "am-string.h"
#include "supported_opt.h"

/**
 * Function checks if CURLoption with type CURL_EXT_* is supported
 * respectively three functions below are checking option with type in name
 */

bool inline curl_ext_is_option(CURLoption option, int type) {
#define _(op, t) (op == option && type == t) ||
  return SUPPORTED_OPT_LIST(_) /*||*/ false;            
#undef _                      
} 

extern bool inline curl_ext_is_cell_option(CURLoption option) {	
	return curl_ext_is_option(option, CURL_EXT_CELL);
}

extern bool inline curl_ext_is_handle_option(CURLoption option) {	
	return curl_ext_is_option(option, CURL_EXT_HANDLE);
}

extern bool inline curl_ext_is_string_option(CURLoption option) {	
	return curl_ext_is_option(option, CURL_EXT_STRING);
}


#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);   \
  void operator=(const TypeName&)


class WriteData
{
 public:
 	WriteData(const char* path);
	~WriteData();
	size_t Write(const char* data, size_t size);
	void GetBuffer(ke::AString& dest);
	void GetFile(FILE** ptr);
 private:
 	char buffer_[BUFSIZ];
 	FILE* file_;
};

class IReadData
{
 public:
	virtual ~IReadData() {	
	};

	virtual size_t Read(char* buffer, size_t size) = 0;
};

class Curl
{
 public:
	Curl(CURL* curl);	
	~Curl();	

	static Curl* Initialize();

	CURLcode last_error() const {
		return last_error_;	
	}

	Curl* MakeDuplicate();	
	bool Exec();	
	void Reset();	

	inline const char* StringifyLastError() {  
		return curl_easy_strerror(last_error_);
	}

	bool SetOptionHandle(CURLoption option, void* handle);
	bool SetOptionInteger(CURLoption option, int /*int32_t*/ param);
	bool SetOptionString(CURLoption option, const char* str);

	inline void set_write_data(WriteData* write_data) {  
		write_data_ = write_data;
	}
	inline void set_read_data(IReadData* read_data) {	
		read_data_ = read_data;
	}
	inline void set_last_error(CURLcode error) {	
		last_error_ = error;
	}

	void UrlEncode(const char* url, ke::AString* out);
	void UrlDecode(const char* url, ke::AString* out);

 private:
	CURL* curl_;
	WriteData* write_data_;
	IReadData* read_data_;
	CURLcode last_error_;

	DISALLOW_COPY_AND_ASSIGN(Curl);
};

#endif // CURL_EXT_UTILS_CURLAPI_H
