#include "Document.h"

#include <iostream>
#include <iomanip>


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
        { "camera", camera },
        { "camera_sn", sn }
    };
    return json;
}
