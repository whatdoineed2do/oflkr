#include <iostream>
#include <exception>

#include "MetaParser.h"
#include "Document.h"
#include "Logger.h"

int main(int argc, char* argv[])
{
    oflkr::MetaParser  p;
    oflkr::Document::Meta*  m;

    try
    {
        oflkr::Logger&  log = LOG_ROOT();

        char**  fp = argv+1;
        while (*fp)
        {
            const char*  f = *fp++;
            if ( (m = p.parse(f)) == nullptr) {
                continue;
            }

            LOG_DEBUG(log) << f;
            LOG_INFO(log) << m->_meta();

            oflkr::Document  doc(oflkr::Document::Object(f, f, oflkr::Document::UNKNOWN, "mime/???", 123456, "h x w", "Jan 1 1970 midnight", "3/5"), std::shared_ptr<oflkr::Document::Meta>(m));

            LOG_NOTICE(log) << doc.json();
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "failed to initialise logger - " << ex.what() << '\n';
    }
    return 0;
}
