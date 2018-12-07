#include "Document.h"

#include <iostream>
#include <iomanip>

namespace oflkr {

Document::JSON  Document::json() const
{
    JSON  json = { 
        { "id", key.id }, 
        {
            "ingest", {
                { "id", key.ingest.id },
                { "date", key.ingest.dt }
            }
        },
        { "vpath", obj.vpath },
        { "name", obj.title },
        { "type", obj.type },

        {
            "media", {
                { "size", obj.size },
                { "mime_type", obj.mimetype },
                { "dimensions", obj.xy }
            }
        }
    };

    json["media"]["meta"] = meta ? meta->_meta() : JSON();
    return json;
}

Document::JSON  Document::MetaImg::_meta() const
{
    JSON  json = {
        {
        "image", {
            { "camera", camera },
            { "camera_sn", sn },
            { "shutter_count", shuttercnt},
            { "lens", lens },
            { "focal_length", focallen },
            { 
                "exposure", {
                    { "shutter", shutter },
                    { "aperture", aperture },
                    { "iso", iso }
                }
            }
        }
        }
    };
    return json;
}

Document::JSON  Document::MetaVid::_meta() const
{
    JSON  json = {
        {
        "video", {
            { "model", model },
            { "codec", codec },
            { "container", container },
            { "duration", duration },
            { "framerate", framerate }
        }
        }
    };
    return json;
}

}
