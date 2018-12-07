#ifndef REST_SVR_H
#define REST_SVR_H

#include <cstdint>
#include <memory>
#include <list>

#include <restbed>
#include "Logger.h"

namespace oflkr {

class DataLayer;


class RESTsvr
{
  public:
    RESTsvr(const int port_, const int workers_) noexcept;

    RESTsvr(const RESTsvr&) = delete;
    RESTsvr& operator=(const RESTsvr&) = delete;


    void  start();
    void  stop();

  private:
    restbed::Service  _svc;
    std::shared_ptr<restbed::Settings>  _settings;
    std::list<std::shared_ptr<restbed::Resource>>  _resources;

    oflkr::Logger&  _log;

    // endpoints
    //   GET
    static void _geStream       (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _geStreamRecent (RESTsvr&, const std::shared_ptr<restbed::Session>);

    static void _geAlbums       (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _geAlbum        (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _geDocument     (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _geDocumentMeta (RESTsvr&, const std::shared_ptr<restbed::Session>);

    //   PUT
    static void _peAlbum        (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _peDocument     (RESTsvr&, const std::shared_ptr<restbed::Session>);

    //   PUT
    static void _deAlbum        (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _deDocument     (RESTsvr&, const std::shared_ptr<restbed::Session>);

    std::shared_ptr<DataLayer>  _dal;
};

}

#endif
