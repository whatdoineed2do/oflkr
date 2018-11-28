#include "RESTsvr.h"

#include <sys/types.h>
#include <signal.h>
#include <string>
#include <map>
#include <iomanip>
#include <functional>


RESTsvr::RESTsvr(const int port_, const int workers_)
    : _log(log4cpp::Category::getInstance("RESTsvr"))
{
    LOG_DEBUG(_log) << "port=" << port_ << " workers=" << workers_;

    const std::string  GET = "GET";
    const std::string  PUT = "PUT";

    auto resource = std::make_shared<restbed::Resource>();
    resource->set_path("/api/stream" );
    resource->set_method_handler(GET, std::bind(RESTsvr::_geStream, std::ref(*this), std::placeholders::_1) );

    _svc.publish(resource);
    
    _settings = std::make_shared<restbed::Settings>();
    _settings->set_port(port_);
    _settings->set_worker_limit(workers_);
    _settings->set_default_header("Connection", "close");
    _settings->set_default_header("Server", "whatdoineed2do/oflkr" );
    _settings->set_default_header("Pragma", "no-cache" );
    _settings->set_default_header("Cache-Control", "private,max-age=0,no-cache,no-store" );
    _settings->set_case_insensitive_uris(true);

#if 0
    _svc.set_signal_handler(SIGINT,  std::bind(RESTsvr::_sighdlr, std::ref(*this), std::placeholders::_1) );
    _svc.set_signal_handler(SIGTERM, std::bind(RESTsvr::_sighdlr, std::ref(*this), std::placeholders::_1) );
    _svc.set_signal_handler(SIGHUP,  std::bind(RESTsvr::_sighdlr, std::ref(*this), std::placeholders::_1) );
#endif

    _svc.set_error_handler([this](const int i_, const std::exception& e_, const std::shared_ptr<restbed::Session> s_) {
        LOG_ERROR(_log) << "error handler: " << i_ << " - " << e_.what();
    });

    _svc.set_not_found_handler([this](const std::shared_ptr<restbed::Session> s_)
    {
        const auto&  req = s_->get_request();

        std::ostringstream  dump;
        dump << "BAD_REQUEST endpoint not found - " << req->get_method() << ' ' << req->get_path() << '\n';
        const std::string  err = std::move(dump.str());

        LOG_WARN(_log) << err;
        s_->close(restbed::BAD_REQUEST, err, {{ "Content-Length", std::to_string(err.length()) }} );
    });
}

void  RESTsvr::start() noexcept
{
    LOG_INFO(_log) << "staring svc " << std::hex << this;
    _svc.start(_settings);
}

void  RESTsvr::stop()  noexcept
{
    _svc.stop();
    LOG_INFO(_log) << "stopping svc " << std::hex << this;
}

void  RESTsvr::_sighdlr(RESTsvr& svr_, const int sig_)
{
    LOG_INFO(svr_._log) << "recv'd signal (" << sig_ << ") stopping " << std::hex << &svr_;
    svr_.stop();
}

void  RESTsvr::_geStream(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_)
{
    LOG_DEBUG(svr_._log) << "endpoint: stream";
    const auto&  req = s_->get_request();

    std::ostringstream  s;
    s << "endpoint:  geStream()\n";
    const std::string  body = std::move(s.str());

    const std::multimap< std::string, std::string >  hdrs
    {
        { "Content-Type", "text/html" },
        { "Content-Length", std::to_string(body.length()) }
    };

    s_->close(restbed::OK, body, hdrs);
}

void  RESTsvr::_geStreamRecent(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_)
{
}

void  RESTsvr::_geAblums(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }
void  RESTsvr::_geAblum (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }
void  RESTsvr::_geMedia(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }
void  RESTsvr::_geMediaMeta(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }

void  RESTsvr::_peAblum        (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }
void  RESTsvr::_peMedia        (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }

void  RESTsvr::_deAblum        (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }
void  RESTsvr::_deMedia        (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }


