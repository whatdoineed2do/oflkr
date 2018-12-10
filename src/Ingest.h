#ifndef OFLKR_INGEST_H
#define OFLKR_INGEST_H

#include <sys/types.h>
#include <memory>
#include <exception>

#include "Document.h"
#include "MetaParser.h"


namespace oflkr {

class DataLayer;

class Ingest
{
  public:
    Ingest(DataLayer& dl_) : _dl(dl_)  { }

    ~Ingest() = default;

    Ingest(const Ingest&) = delete;
    Ingest& operator=(const Ingest&) = delete;


    std::shared_ptr<Document>  ingest(const char* file_) throw (std::invalid_argument, std::system_error);
    std::shared_ptr<Document>  ingest(const char* buf_, size_t bufsz_)  throw (std::invalid_argument, std::system_error);


  private:
    DataLayer&  _dl;
    MetaParser  _mp;
};

}
#endif

