#ifndef OFLKR_DOCUMENT_H
#define OFLKR_DOCUMENT_H

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>


class Document
{
  public:
    using JSON = nlohmann::json;

    enum Type { UNKNOWN, IMAGE, VIDEO };

    Document(const char* vpath_, const size_t sz_) : vpath(vpath_), size(sz_), type(Document::UNKNOWN), meta(nullptr)
    { _title(); }

    Document(const Document&&  rhs_)
	: vpath(std::move(rhs_.vpath)), title(std::move(rhs_.title)), type(rhs_.type), mimetype(std::move(rhs_.mimetype)), size(rhs_.size), uri(std::move(rhs_.uri)), 
	  xy(std::move(rhs_.xy)), moddate(std::move(rhs_.moddate)), rating(std::move(rhs_.rating)),
	  meta(std::move(rhs_.meta))
    { }

    Document(const Document&  rhs_) = delete;
    void operator=(const Document&)  = delete;
    void operator=(const Document&&) = delete;



    uint64_t  id;
    struct {
        uint64_t  id;
        uint64_t  dt;
    } ingest;
    uint64_t  dttaken;

    std::string  vpath;
    std::string  title;

    Document::Type  type;
    std::string  mimetype;
    size_t  size;
    std::string  uri;

    std::string  xy;        // dimensions

    std::string  moddate;  // may be empty or reset to empty
    std::string  rating;

    JSON  json() const;

    struct Meta
    {
        const Document&  document;

        Meta(const Document& d_) : document(d_) { }
        Meta(const Meta&  rhs_) : document(rhs_.document) { }
        Meta(const Meta&& rhs_) : document(rhs_.document) { }

        Meta& operator=(const Meta&)  = delete;
        Meta& operator=(const Meta&&) = delete;

        virtual ~Meta() = default;

        virtual JSON  _meta() const = 0;
    };

    struct MetaImg : public Meta
    {
        MetaImg(const Document& d_) : Meta(d_), rotate(0) { }

	MetaImg(const MetaImg& rhs_) :
            Meta(rhs_),
	    dpi(rhs_.dpi),
	    rotate(rhs_.rotate),
	    camera(rhs_.camera),
	    sn(rhs_.sn),
	    lens(rhs_.lens),
	    focallen(rhs_.focallen),
	    shuttercnt(rhs_.shuttercnt),
	    shutter(rhs_.shutter),
	    aperture(rhs_.aperture),
	    iso(rhs_.iso),
	    wb(rhs_.wb)
	{ }

	MetaImg(const MetaImg&& rhs_) :
            Meta(rhs_),
	    dpi(std::move(rhs_.dpi)),
	    rotate(rhs_.rotate),
	    camera(std::move(rhs_.camera)),
	    sn(std::move(rhs_.sn)),
	    lens(std::move(rhs_.lens)),
	    focallen(std::move(rhs_.focallen)),
	    shuttercnt(std::move(rhs_.shuttercnt)),
	    shutter(std::move(rhs_.shutter)),
	    aperture(std::move(rhs_.aperture)),
	    iso(std::move(rhs_.iso)),
	    wb(std::move(rhs_.wb))
	{ }

	MetaImg& operator=(const MetaImg&)  = delete;
	MetaImg& operator=(const MetaImg&&) = delete;


	std::string  dpi;
	double  rotate;

	std::string  camera;
	std::string  sn;
	std::string  lens;
	std::string  focallen;
	std::string  shuttercnt;

	std::string  shutter;
	std::string  aperture;
	std::string  iso;
	std::string  wb;

        JSON  _meta() const override;
    };

    struct MetaVid : public Meta 
    {
	MetaVid(const Document& d_) : Meta(d_), duration(0), framerate(0) { }

	MetaVid(const MetaVid& rhs_) :
            Meta(rhs_), 
            container(rhs_.container),
	    model(rhs_.model),
	    duration(rhs_.duration),
	    framerate(rhs_.framerate)
	{}
	MetaVid(const MetaVid&& rhs_) :
            Meta(rhs_),
            container(std::move(rhs_.container)),
	    model(std::move(rhs_.model)),
	    duration(rhs_.duration),
	    framerate(rhs_.framerate)
	{}
	MetaVid& operator=(const MetaVid&)  = delete;
	MetaVid& operator=(const MetaVid&&) = delete;

	std::string  container;
	std::string  model;
	time_t  duration;
	float   framerate;

        JSON  _meta() const override
        {
            return JSON();
        }
    };
    Document::Meta*  meta;


  private:
    void  _title()
    {
	std::string::size_type  p;
	title = ( (p = vpath.find_last_of('/')) == std::string::npos) ? 
	            vpath :
		    vpath.substr(p+1);
    }
};

#endif
