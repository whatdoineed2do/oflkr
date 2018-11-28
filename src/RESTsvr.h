#ifndef REST_SVR_H
#define REST_SVR_H

#include <memory>
#include <list>

#include <restbed>


class RESTsvr
{
  public:
    RESTsvr();

    RESTsvr(const RESTsvr&) = delete;
    RESTsvr& operator=(const RESTsvr&) = delete;


    void  start() noexcept;
    void  stop()  noexcept;

  private:
    restbed::Service  _svc;
    std::shared_ptr<restbed::Settings>  _settings;
    std::list<std::shared_ptr<restbed::Resource>>  _resources;

    static void  _sighdlr(RESTsvr&, const int);

    // endpoints
    //   GET
    static void _geStream       (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _geStreamRecent (RESTsvr&, const std::shared_ptr<restbed::Session>);

    static void _geAblums       (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _geAblum        (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _geMedia        (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _geMediaMeta    (RESTsvr&, const std::shared_ptr<restbed::Session>);

    //   PUT
    static void _peAblum        (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _peMedia        (RESTsvr&, const std::shared_ptr<restbed::Session>);

    //   PUT
    static void _deAblum        (RESTsvr&, const std::shared_ptr<restbed::Session>);
    static void _deMedia        (RESTsvr&, const std::shared_ptr<restbed::Session>);
};

#endif
