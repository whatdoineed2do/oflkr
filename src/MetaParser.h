#ifndef OFLKR_META_PARSER_H
#define OFLKR_META_PARSER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <list>

#include "Logger.h"
#include "Document.h"


namespace Exiv2 {
    class Image;
}

namespace oflkr
{
struct DocumentIdentifier;
struct _Parser;


class MetaParser
{
  public:
    using _Parsers = std::list<oflkr::DocumentIdentifier>;

    MetaParser();
    ~MetaParser() = default;

    MetaParser(const MetaParser&) = delete;
    MetaParser& operator=(const MetaParser&) = delete;


    Document::Meta*  parse(const char* filename_) const
        throw (std::invalid_argument, std::range_error, std::underflow_error, std::overflow_error);

    Document::Meta*  parse(const char* data_, size_t dsz_) const
        throw (std::invalid_argument, std::range_error, std::underflow_error, std::overflow_error);

  protected:
    oflkr::Logger&  _log;

  private:
    MetaParser::_Parsers  _parsers;

    Document::Meta*  _parse(Exiv2::Image&) const
        throw (std::invalid_argument, std::range_error, std::underflow_error, std::overflow_error);
};


struct DocumentIdentifier {
    std::shared_ptr<oflkr::_Parser>  p;
    std::function<bool (const Exiv2::Image&)>  handles;
};
}

#endif
