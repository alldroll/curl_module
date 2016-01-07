#ifndef CURL_EXT_UTILS_STREAMBUFFER_H
#define CURL_EXT_UTILS_STREAMBUFFER_H

#include <stdio.h>
#include <string.h>
#include <assert.h>

/**
 * TODO
 * 1. Think about stringstream
 * 2. Fast copying
 */

class BaseStream
{
 public:
 	BaseStream(FILE* file) : file_(file), filled_(0) { 
 	}
 	~BaseStream() {	
 	}

 	const char* buffer() {	
 		buffer_[filled_] = 0;
 		return buffer_;
 	}

 	const size_t filled() {
 		return filled_;
 	}

 protected:
 	char buffer_[BUFSIZ];
 	FILE* file_;
 	size_t filled_;
};

class OStream : public BaseStream
{
 public:

 	enum FCloseT
 	{
 		LEAVE,
 		REMOVE
 	};

 	OStream(FILE* file, FCloseT type) 
 			: BaseStream(file), type_(type) {	
 		setbuf(file_, buffer_);
 	}
 	~OStream() { 
 		fclose(file_);
 		if (type_ == REMOVE) { 
 			remove(filename_);
 		}
 	}

 	static OStream* Initialize(const char* filename) {	
 		FILE* file = NULL;
 		char* filename_ = (char*)filename;
 		FCloseT type = LEAVE;

 		if (!filename_ || !strlen(filename_)) {	
 			filename_ = tmpnam(NULL);
 			type = REMOVE;
 		}

 		file = fopen(filename_, "wb+");

 		if (file == NULL) {	
 			return NULL;
 		}

 		OStream* out = new OStream(file, type);

 		out->set_filename(filename_);

 		return out;
 	}

 	void set_filename(const char* filename) {	
 		size_t length = strlen(filename);
 		assert(length < FILENAME_MAX);
 		memcpy(filename_, filename, length);
 		filename_[length] = '\0';
 	}

 	const char* filename() { 
 		return filename_;
 	}

 	void Flush() {
 		fflush(file_);
 		filled_ = 0;
 	}

 	size_t Write(const char* data, size_t bytes, size_t nitems) {	
 		size_t result = fwrite(data, bytes, nitems, file_);
 		filled_ += result;

 		if (filled_ >= BUFSIZ) {	
 			filled_ -= BUFSIZ;
 		}

 		return result;
 	}

 	size_t WriteString(const char* data) { 
 		return Write(data, 1, strlen(data));
 	}

 private:

 	FCloseT type_;
 	char filename_[FILENAME_MAX];
};

// class InStreamBuffer : public StreamBuffer 
// {
//  public:
//  	InStreamBuffer(FILE* file) : StreamBuffer(file), 
//  			method_(FROMFILE), readed_(0) {	
//  		if (file != NULL) {	
//  			setbuf(file_, buffer_);
//  		}
//  	}
//  	InStreamBuffer(const char* data) : StreamBuffer(NULL), 
//  			method_(FROMSTRING), readed_(0) {	
//  		filled_ = strlen(data);
//  		if (filled_ >= BUFSIZ) {	
//  			filled_ = BUFSIZ;
//  		}
//  		memcpy(chars_, str, filled_);
//  		buffer_[filled_ - 1] = '\0';
//  	}
//  	~InStreamBuffer() {
//  		//TODO
//  	}

//  	static InStreamBuffer* InitializeFromFile(const char* path) {	
//  		FILE* file = NULL;

//  		if (path && strlen(path)) {	
//  			file = fopen(path, "rb");
//  		}

//  		if (file == NULL) {	
//  			return NULL;
//  		}

//  		return new InStreamBuffer(file);
//  	}

//  	static InStreamBuffer* InitializeFromString(const char* data) {	

//  		return new InStreamBuffer(file);
//  	}


//  	static InStreamBuffer* Initialize(const char* path) {	
//  		FILE* file = NULL;

//  		// if (!path || !strlen(path)) {	
//  		// 	file = tmpfile();
//  		// } else {	
//  		// 	file = fopen(path, "a");
//  		// }

//  		if (file == NULL) {	
//  			return NULL;
//  		}

//  		return new InStreamBuffer(file);
//  	}

//  	size_t Read(char* buffer, size_t bytes, size_t nitems) {	
//  		return 0;
//  	}

//  private:
//  	enum {
//  		FROMSTRING,	
//  		FROMFILE
//  	} InputT;

//  	InputT method_;
//  	size_t readed_;
// };


#endif // CURL_EXT_UTILS_STREAMBUFFER_H
