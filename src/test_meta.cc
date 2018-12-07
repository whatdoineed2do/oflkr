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

            LOG_NOTICE(log) << f;
            LOG_NOTICE(log) << m->_meta();
            delete m;
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "failed to initialise logger - " << ex.what() << '\n';
    }
    return 0;
}
