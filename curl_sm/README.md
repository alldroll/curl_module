##Sourcemod CURL extension

####based on ([php_curl extension](https://github.com/php/php-src/tree/master/ext/curl))

###Natives


####Options

#####WRITEDATA
WRITEDATA is Handle recieved by calling curl_writedata(Handle:curl). The following natives are available :
	curl_writedata_tofile(Handle:file);
	curl_writedata_tostr(String:buf[], buflen);

#####READDATA
READDATA is Handle recieved by calling curl_readdata(Handle:curl). The following natives are 
available :
	curl_readdata_appendstr(const String:data[]);
	curl_readdata_appendfile(Handle:file);



