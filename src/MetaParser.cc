#include "MetaParser.h"

#include <unistd.h>
#include <errno.h>
#include <strings.h>

#include <sstream>

#include <exiv2/exiv2.hpp>

namespace oflkr
{
    struct _Parser {
        const char*  id;
        oflkr::Logger&  log;

        _Parser(oflkr::Logger& log_, const char* id_) : log(log_), id(id_) { }
        virtual ~_Parser() = default;

        virtual Document::Meta*  parse(const Exiv2::Image&) = 0;
    };
    struct _ImgParser : public _Parser {
        _ImgParser(oflkr::Logger& log_) : _Parser(log_, "image") { }
        Document::Meta*  parse(const Exiv2::Image&) override;
    };
    struct _VidParser : public _Parser {
        _VidParser(oflkr::Logger& log_) : _Parser(log_, "video") { }
        Document::Meta*  parse(const Exiv2::Image&) override;
    };

MetaParser::MetaParser()
    : _log(LOG_INSTANCE("MetaParser"))
{
    _parsers = {
        { std::make_shared<oflkr::_ImgParser>(_log), 
          // yes, this is kinda dirty way to determine which underlying meta parser to use...
          [](const Exiv2::Image& img_) -> bool{
              return img_.mimeType().compare(0, 6, "image/") == 0;
          }
        },

        { std::make_shared<oflkr::_VidParser>(_log), 
          [](const Exiv2::Image& img_) -> bool{
              return img_.mimeType().compare(0, 6, "video/") == 0;
          }
        }
    };
}

Document::Meta*  MetaParser::parse(const char* filename_)
    throw (std::invalid_argument, std::range_error, std::underflow_error, std::overflow_error)
{
    Document::Meta*  m = nullptr;
    try
    {
        Exiv2::Image::AutoPtr  imgp = Exiv2::ImageFactory::open(filename_);
        imgp->readMetadata();
        const Exiv2::Image&  img = *imgp.get();

        for (const auto& p : _parsers) {
            if (p.handles(img)) {
                m = p.p->parse(img);
                break;
            }
        }
    }
    catch (const std::exception& ex)
    {
        LOG_ERROR(_log) << "failed to parse image - " << ex.what();
    }
    return m;
}


Document::Meta*  _ImgParser::parse(const Exiv2::Image& img_)
{
    Document::MetaImg*  meta = new Document::MetaImg();
    typedef Exiv2::ExifData::const_iterator (*EasyAccessFct)(const Exiv2::ExifData& ed);

    std::string   mftr;
    std::string   dtorg;
    std::string   dtorgsub;
    float    orientation = 1;

    static const struct _EasyAccess {
	EasyAccessFct  find;
	std::string*   target;
    } eatags[] = {
	{ Exiv2::isoSpeed,     &meta->iso      },
	{ Exiv2::whiteBalance, &meta->wb       },
	{ Exiv2::lensName,     &meta->lens     },
	{ Exiv2::model,        &meta->camera   },
	{ Exiv2::exposureTime, &meta->shutter  },
	{ Exiv2::fNumber,      &meta->aperture },
	{ Exiv2::serialNumber, &meta->sn       },
	{ Exiv2::focalLength,  &meta->focallen },

	{ NULL, NULL }
    };

    static const struct _MiscTags {
	const char*   tag;
	std::string*  target;
	long*         tgtlong;
	float*        tgtfloat;
    } misctags[] = {
	{ "Exif.Photo.DateTimeOriginal",   &dtorg,            NULL, NULL },   
	{ "Exif.Photo.SubSecTimeOriginal", &dtorgsub,         NULL, NULL },

	{ "Exif.Nikon3.ShutterCount",      &meta->shuttercnt, NULL, NULL },

	{ "Exif.Image.XResolution",        &meta->dpi,        NULL, NULL },
	{ "Exif.Image.Orientation",        NULL,              NULL, &orientation },

	{  NULL, NULL, NULL, NULL }
    };

    const Exiv2::ExifData&  ed = img_.exifData();
    Exiv2::ExifData::const_iterator  exif;
    const _EasyAccess*  ep = eatags;
    while (ep->target)
    {
        if ( (exif = ep->find(ed)) != ed.end())  {
            *ep->target = exif->print(&ed);
        }
        ++ep;
    }

    const _MiscTags*  mp = misctags;
    while (mp->tag)
    {
        if ( (exif = ed.findKey(Exiv2::ExifKey(mp->tag)) ) != ed.end()) {
            if (mp->target)    *mp->target   = exif->print(&ed);
            if (mp->tgtlong)   *mp->tgtlong  = exif->toLong();
            if (mp->tgtfloat)  *mp->tgtfloat = exif->toFloat();
        }
        ++mp;
    }

    switch ((int)orientation) {
        case 2:  orientation = 0;  break;   // flip horz
        case 3:  orientation = 180;  break;
        case 4:  orientation = 0;  break;   // flip vert
        case 5:  orientation = 0;  break;   // transpose
        case 6:  orientation = 90;  break;
        case 7:  orientation = 0;  break;   // traverse
        case 8:  orientation = -90;  break;

        case 1:
        default: orientation = 0;
    }
    meta->rotate = orientation;

    return meta;
}

Document::Meta*  _VidParser::parse(const Exiv2::Image& img_)
{
    Document::MetaVid*  meta = new Document::MetaVid();

    static const struct _Tags {
	const char* tag;
	std::string* target;
    } 
    xmptags[] = {
	{ "Xmp.video.Codec",         &meta->codec },
        { "Xmp.video.Duration",      &meta->duration },
        { "Xmp.video.FrameRate",     &meta->framerate },

        { "Xmp.audio.Compressor",    &meta->audioType },
        { "Xmp.audio.SampleRate",    &meta->audioSamplerate },
        { "Xmp.audio.BitsPerSample", &meta->audioBitsPerSample },
        { "Xmp.audio.ChannelType",   &meta->audioChannels },

	{ NULL, NULL }
    };

    const Exiv2::ExifData&  ed = img_.exifData();
    const Exiv2::XmpData&  xmp = img_.xmpData();
    Exiv2::XmpData::const_iterator  xp;
    const _Tags*  mp = xmptags;
    while (mp->tag)
    {
        if ( (xp = xmp.findKey(Exiv2::XmpKey(mp->tag)) ) != xmp.end()) {
            *mp->target = xp->print(&ed);
        }
        ++mp;
    }


    return meta;
}

}
