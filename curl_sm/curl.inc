#if defined _curl_included
 #endinput
#endif
#define _curl_included

enum CURLcode
{

};

enum CURLoption
{

};

funcenum CURL_CompleteCallback
{
	public (Handle:curl, CURLcode:error, any:userdata)
};

/**
 *
 */
native Handle:curl_copy_handle(Handle:curl);

/**
 *
 */
native CURLcode:curl_errno(Handle:curl);

/**
 *
 */
native curl_error(Handle:curl, String:buffer[], buflen);

/**
 *
 */
native curl_escape(Handle:curl, const String:url[], String:buffer[], buflen);

/**
 *
 */
native bool:curl_exec(Handle:curl, CURL_CompleteCallback:func, any:userdata);

/**
 *
 */
native Handle:curl_getinfo(Handle:curl); 

/**
 *
 */
native Handle:curl_init(const String:url = "");

/**
 *
 */
native curl_reset(Handle:curl);

/**
 *
 */
native bool:curl_setopt_cell(Handle:curl, CURLoption option, value);

/**
 *
 */
native bool:curl_setopt_string(Handle:curl, CURLoption option, const String:value[]);

/**
 *
 */
native bool:curl_setopt_handle(Handle:curl, CURLoption option, Handle:handle);

/**
 *
 */
native bool:curl_thread_exec(Handle:curl, CURL_CompleteCallback:func, any:userdata);

/**
 *
 */
native curl_unescape(Handle:curl, const String:url[], String:buffer[], buflen);



public Extension:__ext_curl = 
{
	name = "curl",
	file = "curl.ext",

#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif

#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};