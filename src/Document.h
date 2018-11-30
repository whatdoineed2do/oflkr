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

    struct Key
    {
        const uint64_t  id;
        struct Ingest {
            uint64_t  id;
            uint64_t  dt;

            Ingest() : id(0), dt(0) { }
            Ingest(const uint64_t id_, const uint64_t dt_) : id(id_), dt(dt_)
            { }
        };
        const Ingest  ingest;
        const uint64_t  dttaken;

        Key() : id(-1), dttaken(0) { }

        Key(uint64_t id_, const Ingest& ingest_, uint64_t dttaken_)
            : id(id_), ingest(ingest_), dttaken(dttaken_)
        { }

        Key(const Key& rhs_)
            : id(rhs_.id), ingest(rhs_.ingest), dttaken(rhs_.dttaken)
        { }
    };

    struct Object
    {
        const std::string  vpath;
        const std::string  title;

        const Document::Type  type;
        const std::string  mimetype;
        const size_t  size;

        const std::string  xy;        // dimensions

        const std::string  moddate;  // may be empty or reset to empty
        const std::string  rating;

        Object( const std::string&  vpath_,
                const std::string&  title_,

                const Document::Type  type_,
                const std::string&  mimetype_,
                const size_t  size_,

                const std::string&  xy_,

                const std::string&  moddate_,
                const std::string&  rating_)
            : 
                vpath(vpath_), title(title_), type(type_), mimetype(mimetype_), size(size_),
                xy(xy_), moddate(moddate_), rating(rating_)
        { }

        Object(const Object& rhs_) = delete;

        Object(const Object&& rhs_) :
                vpath(std::move(rhs_.vpath)), title(std::move(rhs_.title)), type(std::move(rhs_.type)), mimetype(std::move(rhs_.mimetype)), size(std::move(rhs_.size)),
                xy(std::move(rhs_.xy)), moddate(std::move(rhs_.moddate)), rating(std::move(rhs_.rating))
        { }
    };

    JSON  json() const;

    struct Meta
    {
        Meta() = default;
        Meta(const Meta&) = default;
        Meta(Meta&&) = default;

        Meta& operator=(const Meta&)  = delete;
        Meta& operator=(const Meta&&) = delete;

        virtual ~Meta() = default;

        virtual JSON  _meta() const = 0;
    };

    struct MetaImg : public Meta
    {
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
	MetaVid() : duration(0), framerate(0) { }

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

    Document(const Document::Object&& obj_, const std::shared_ptr<Document::Meta>& meta_)
        : obj(std::move(obj_)), meta(meta_) 
    { }

    Document(const Document::Key&& key_, const Document::Object&& obj_, const std::shared_ptr<Document::Meta>& meta_)
        : key(std::move(key_)), obj(std::move(obj_)), meta(meta_) 
    { }


    Document(const Document&  rhs_) = delete;
    Document(const Document&& rhs_) : key(std::move(rhs_.key)), obj(std::move(rhs_.obj)), meta(std::move(rhs_.meta)) { }

    void operator=(const Document&)  = delete;
    void operator=(const Document&&) = delete;


    const Document::Key     key;
    const Document::Object  obj;
    const std::shared_ptr<Document::Meta>  meta;

  private:
#if 0
    void  _title()
    {
	std::string::size_type  p;
	title = ( (p = vpath.find_last_of('/')) == std::string::npos) ? 
	            vpath :
		    vpath.substr(p+1);
    }
#endif
};

#endif
