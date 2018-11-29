#include "RESTsvr.h"

#include <sys/types.h>
#include <signal.h>
#include <string>
#include <map>
#include <list>
#include <iomanip>
#include <functional>

#include <nlohmann/json.hpp>
using json = nlohmann::json;


    const std::string  GET = "GET";
    const std::string  PUT = "PUT";
    const std::string  POST = "POST";

    const std::string  APPL_JSON = "application/json";
    const std::string  APPL_HTML = "text/html";


RESTsvr::RESTsvr(const int port_, const int workers_)  noexcept
    : _log(log4cpp::Category::getInstance("RESTsvr"))
{
    LOG_DEBUG(_log) << "port=" << port_ << " workers=" << workers_;

    struct _Resources {
        const std::string&  gop;
	const char*         path;
	const std::function<void (const std::shared_ptr<restbed::Session>)>  trgt;

	_Resources(const std::string& gop_,
	           const char* path_,
		   const std::function<void (const std::shared_ptr<restbed::Session>)>  trgt_)
	    : gop(gop_), path(path_), trgt(trgt_)
	{ }
    };
    const std::list<_Resources>  resources = {
      // RETREIVE
        { GET, "/api/stream", 
	  std::bind(RESTsvr::_geStream, std::ref(*this), std::placeholders::_1)
	},
        { GET, "/api/stream/recent", 
	  std::bind(RESTsvr::_geStreamRecent, std::ref(*this), std::placeholders::_1)
	},
        { GET, "/api/albums", 
	  std::bind(RESTsvr::_geAlbums, std::ref(*this), std::placeholders::_1)
	},
        { GET, "/api/album/{id: [0-9]*}", 
	  std::bind(RESTsvr::_geAlbum, std::ref(*this), std::placeholders::_1)
	},
        { GET, "/api/media/{id: [0-9]*}", 
	  std::bind(RESTsvr::_geMedia, std::ref(*this), std::placeholders::_1)
	},
        { GET, "/api/media/{id: [0-9]*}/meta", 
	  std::bind(RESTsvr::_geMediaMeta, std::ref(*this), std::placeholders::_1)
	},

      // CREATE
        { PUT, "/api/album/create", 
	  std::bind(RESTsvr::_peAlbum, std::ref(*this), std::placeholders::_1)
	},
        { PUT, "/api/media/create", 
	  std::bind(RESTsvr::_peMedia, std::ref(*this), std::placeholders::_1)
	},

      // DELETE
        { POST, "/api/album/delete", 
	  std::bind(RESTsvr::_deAlbum, std::ref(*this), std::placeholders::_1)
	},
        { POST, "/api/media/delete", 
	  std::bind(RESTsvr::_deMedia, std::ref(*this), std::placeholders::_1)
	}
    };

    for (auto& r : resources) {
	auto resource = std::make_shared<restbed::Resource>();
	resource->set_path(r.path);
	resource->set_method_handler(r.gop, r.trgt);

	_svc.publish(resource);
    }
    
    _settings = std::make_shared<restbed::Settings>();
    _settings->set_port(port_);
    _settings->set_worker_limit(workers_);
    _settings->set_default_header("Connection", "close");
    _settings->set_default_header("Server", "whatdoineed2do/oflkr" );
    _settings->set_default_header("Pragma", "no-cache" );
    _settings->set_default_header("Cache-Control", "private,max-age=0,no-cache,no-store" );
    _settings->set_case_insensitive_uris(true);

// doesnt work... deadlock
#if 0
    _svc.set_signal_handler(SIGINT, [this](const int sig_) {
        this->_svc.stop();
    });
    _svc.set_signal_handler(SIGTERM, std::bind(RESTsvr::_sighdlr, std::ref(*this), std::placeholders::_1) );
    _svc.set_signal_handler(SIGHUP,  std::bind(RESTsvr::_sighdlr, std::ref(*this), std::placeholders::_1) );
#endif

    _svc.set_error_handler([this](const int i_, const std::exception& e_, const std::shared_ptr<restbed::Session> s_) {
        LOG_ERROR(_log) << "error handler: " << i_ << " - " << e_.what();
    });
}

void  RESTsvr::start()
{
    LOG_INFO(_log) << "staring svc " << std::hex << this;
    _svc.set_ready_handler([this](restbed::Service& svc_) {
	LOG_INFO(_log) << "svc ready " << std::hex << &svc_;
    });
    _svc.start(_settings);
}

void  RESTsvr::stop()
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
        { "Content-Type", APPL_HTML },
        { "Content-Length", std::to_string(body.length()) }
    };

    s_->close(restbed::OK, body, hdrs);
}

void  RESTsvr::_geStreamRecent(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_)
{
}

void  RESTsvr::_geAlbums(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }

void  RESTsvr::_geAlbum (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_)
{
    const auto&  req = s_->get_request();
    const auto  param = req->get_path_parameter("id");

    json  j = {
        { "id", param },
	{ "count", 100 },
	{
	    "items", {
	      {
	         { "id", 555001 },
		 { "media_type", "image"}
	      },
	      {
	         { "id", 655009, },
		 { "media_type", "image" }
	      }
	    }
	}
    };
    const std::string  body = std::move(j.dump());

    s_->close(restbed::OK, body, 
                {
		  { "Content-Type", APPL_JSON },
		  { "Content-Length", std::to_string(body.length()) }
		} );
}


void  RESTsvr::_geMedia(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }
void  RESTsvr::_geMediaMeta(RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }

void  RESTsvr::_peAlbum        (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }
void  RESTsvr::_peMedia        (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }

void  RESTsvr::_deAlbum        (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }
void  RESTsvr::_deMedia        (RESTsvr& svr_, const std::shared_ptr<restbed::Session> s_) { }


