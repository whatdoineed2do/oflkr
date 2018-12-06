#ifndef OFLKR_META_PARSER_H
#define OFLKR_META_PARSER_H

#include <sys/types.h>
#include <sys/stat.h>

#include "Document.h"

class MetaParser
{
  public:
    MetaParser() = default;
    ~MetaParser() = default;

    MetaParser(const MetaParser&) = delete;
    MetaParser& operator=(const MetaParser&) = delete;


    Document::Meta*  parse(const char* filename_)
        throw (std::invalid_argument, std::range_error, std::underflow_error, std::overflow_error);

    Document::Meta*  parse(const void* data_, size_t dsz_)
        throw (std::invalid_argument, std::range_error, std::underflow_error, std::overflow_error);

  private:
    struct _Parser {
        _Parser() = default;
        virutal ~_Parser() = default;

        virtual Document::Meta*  parse() = 0;
    };
    struct _ImgParser : public _Parser {
        Document::Meta*  parse() override;
    };
    struct _VidParser : public _Parser {
        Document::Meta*  parse() override;
    };
};

#endif
