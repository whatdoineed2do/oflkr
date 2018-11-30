#include "Document.h"

#include <iostream>
#include <iomanip>


Document::JSON  Document::json() const
{
    JSON  json = { 
        { "id", id }, 
        {
            "ingest", {
                { "id", ingest.id },
                { "date", ingest.dt }
            }
        },
        { "vpath", vpath },
        { "name", title },
        { "type", type },

        {
            "media", {
                { "size", size },
                { "mime_type", mimetype },
                { "dimensions", xy }
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
