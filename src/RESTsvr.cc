#include "RESTsvr.h"

#include <sys/types.h>
#include <signal.h>
#include <string>
#include <map>
#include <iomanip>
#include <functional>

#include <iostream>  // TODO remove


//void set_method_handler( const std::string& method, 
    //const std::function< void ( const std::shared_ptr< Session > ) >& callback );

RESTsvr::RESTsvr()
{
    const std::string  GET = "GET";
    const std::string  PUT = "PUT";

    auto resource = std::make_shared<restbed::Resource>();
    resource->set_path("/api/stream" );
    resource->set_method_handler(GET, std::bind(RESTsvr::_geStream, std::ref(*this), std::placeholders::_1) );

    _svc.publish(resource);
    
    _settings = std::make_shared<restbed::Settings>();
    _settings->set_port(1984);
    _settings->set_worker_limit(10);
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

    _svc.set_error_handler([](const int i_, const std::exception& e_, const std::shared_ptr<restbed::Session> s_) {
        std::cout << "error handler: " << i_ << " - " << e_.what() << '\n';
    });

    _svc.set_not_found_handler([](const std::shared_ptr<restbed::Session> s_)
    {
        const auto&  req = s_->get_request();

        std::ostringstream  dump;
        dump << "BAD_REQUEST endpoint not found - " << req->get_method() << ' ' << req->get_path() << '\n';
        const std::string  err = std::move(dump.str());

        std::cout << err;
        s_->close(restbed::BAD_REQUEST, err, {{ "Content-Length", std::to_string(err.length()) }} );
    });
}

void  RESTsvr::start() noexcept
{
    std::cout << "staring svc " << std::hex << this << "\n";
    _svc.start(_settings);
}

void  RESTsvr::stop()  noexcept
{
    _svc.stop();
    std::cout << "stopped svc\n";
}

void  RESTsvr::_sighdlr(RESTsvr& svr_, const int sig_)
{
    std::cout << "recv'd signal (" << sig_ << ") stopping " << std::hex << &svr_ << " ,,,\n";
    svr_.stop();
}

void  RESTsvr::_geStream(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_)
{
    std::cout << "endpoint: stream\n";
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

