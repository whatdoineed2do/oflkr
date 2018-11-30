#include "DataLayer.h"

#include "Document.h"

FileDataLayer::FileDataLayer()
{
}

Document::JSON  FileDataLayer::album(uint64_t id_)
{
    Document  d(Document::Object("/tmp/foo.jpg", "foo", Document::IMAGE, "image/jpeg", 123456, "4288x2848", "today", "1/5"), nullptr);
    return d.json();
}


Document::JSON  FileDataLayer::stream() { return Document::JSON(); }

Document::JSON  FileDataLayer::albums() { return Document::JSON(); }

Document::JSON  FileDataLayer::albumCr(const Document::JSON&)  { return Document::JSON(); }
void  FileDataLayer::albumUpd(uint64_t) { }
void  FileDataLayer::albumDel(uint64_t) { }

Document::JSON  FileDataLayer::media(uint64_t)  { return Document::JSON(); }
Document::JSON  FileDataLayer::mediaCr(const Document::JSON&)  { return Document::JSON(); }
Document::JSON  FileDataLayer::mediaRpl(uint64_t)  { return Document::JSON(); }
Document::JSON  FileDataLayer::mediaUpd(uint64_t) { return Document::JSON(); }
Document::JSON  FileDataLayer::mediaDel(uint64_t) { return Document::JSON(); }

