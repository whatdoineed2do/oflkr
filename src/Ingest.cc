#include "Ingest.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <sstream>
#include <exception>
#include <system_error>


namespace  oflkr {

std::shared_ptr<Document>  Ingest::ingest(const char* file_) throw (std::invalid_argument, std::system_error)
{
    if (file_ == nullptr) {
        throw std::invalid_argument("Ingest - nullptr for file");
    }

    int  fd = -1;
    char*  buf = nullptr;
    std::shared_ptr<Document>  p;

    try
    {
	struct stat  st;
	memset(&st, 0, sizeof(st));
	if ( (fd = open(file_, O_RDONLY)) < 0) {
	    std::ostringstream  err;
	    err << "Ingest - failed to open file, " << file_ << " - " << strerror(errno);
	    throw std::invalid_argument(err.str());
	}

	try
	{
	    buf = new char[st.st_size];
	    char*  pb = buf;
	    size_t  n, remain = st.st_size;
	    while (remain > 0)
	    {
		if ( (n = read(fd, pb, remain > 8192 ? 8192 : remain)) < 0) {
		    if (errno == EINTR) {
			continue;
		    }
		    //throw std::system_error(std::make_error_code(errno), "Ingest - failed to read data");
		    throw std::system_error(errno, std::system_category());
		}
	        pb += n;
		remain -= n;
	    }

	    p = ingest(buf, st.st_size);
	}
	catch (const std::bad_alloc& ex)
	{
	    std::ostringstream  err;
	    err << "Ingest - failed to map file for ingesting, " << file_ << " - " << strerror(errno);
	    throw std::invalid_argument(err.str());
	}
    }
    catch (...)
    {
	close(fd);
	delete [] buf;
        throw;
    }
    return p;
}

std::shared_ptr<Document>  Ingest::ingest(const char* buf_, size_t bufsz_)  throw (std::invalid_argument, std::system_error)
{
    /* generate an ingest id, get the object's meta and return the Document
     */
    std::shared_ptr<Document>  p;
    std::shared_ptr<Document::Meta>  pm(_mp.parse(buf_, bufsz_));
    return p;
}

}
