#ifndef OFLKR_DATA_LAYER_H
#define OFLKR_DATA_LAYER_H

#include <cstdint>

#include "Document.h"

class DataLayer
{
  public:
    virtual ~DataLayer() = default;

    DataLayer(const DataLayer&) = delete;
    DataLayer& operator=(const DataLayer&) = delete;

    virtual Document::JSON  stream() = 0;

    virtual Document::JSON  albums() = 0;

    virtual Document::JSON  album(uint64_t) = 0;
    virtual Document::JSON  albumCr(const Document::JSON&) = 0;
    virtual void  albumUpd(uint64_t) = 0;
    virtual void  albumDel(uint64_t) = 0;

    virtual Document::JSON  media(uint64_t) = 0;
    virtual Document::JSON  mediaCr(const Document::JSON&) = 0;
    virtual Document::JSON  mediaRpl(uint64_t) = 0;
    virtual Document::JSON  mediaUpd(uint64_t) = 0;
    virtual Document::JSON  mediaDel(uint64_t) = 0;

  protected:
    DataLayer() = default;
};

class FileDataLayer : public DataLayer
{
  public:
    FileDataLayer();

    Document::JSON  stream();

    Document::JSON  albums();

    Document::JSON  album(uint64_t) ;
    Document::JSON  albumCr(const Document::JSON&) ;
    void  albumUpd(uint64_t) ;
    void  albumDel(uint64_t) ;

    Document::JSON  media(uint64_t) ;
    Document::JSON  mediaCr(const Document::JSON&) ;
    Document::JSON  mediaRpl(uint64_t) ;
    Document::JSON  mediaUpd(uint64_t) ;
    Document::JSON  mediaDel(uint64_t) ;
};

#endif
