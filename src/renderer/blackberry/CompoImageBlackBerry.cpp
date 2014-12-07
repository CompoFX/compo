#include "CompoImage.h"
#include "../CompoImage.inl"
#include "CompoBB10Util.h"

#include <QImage>
#include <cstdint>

using std::uint32_t;
using std::uint8_t;


CompoImage::CompoImage( const std::string &relativePath ) : d(new Private)
{
  const QString fullPath = assetPath() + "/Compo.bundle/" + relativePath.c_str();
  const QImage img = reorientedImage(fullPath).convertToFormat( QImage::Format_ARGB32 );

  d->width = img.width();
  d->height = img.height();
  d->bpp = 32;

  LOGI("%s --> Bitmap Info: %dx%d %d bit", fullPath.toAscii().constData(),
      d->width, d->height, d->bpp);
  d->pixelData = (unsigned char*) calloc(d->dataSize(), sizeof(unsigned char));
  uint32_t *src = (uint32_t*) img.bits();
  uint32_t *dest = (uint32_t*) d->pixelData;
  size_t sz = d->dataSize();

  LOGI("PIXELDATA %d", sz);

  for (;sz != 0; sz-=4, ++src, ++dest) {
    *dest = ARGB2RGBA(*src);
  }
}

CompoImage::CompoImage( void *imagePtr ) : d(new Private)
{
  const QImage img = ((QImage*)imagePtr)->convertToFormat( QImage::Format_ARGB32 );

  d->width = img.width();
  d->height = img.height();
  d->bpp = 32;

  LOGI("Bitmap Info: %dx%d %d bit", d->width, d->height, d->bpp);
  d->pixelData = (unsigned char*) calloc(d->dataSize(), sizeof(unsigned char));
  uint32_t *src = (uint32_t*) img.bits();
  uint32_t *dest = (uint32_t*) d->pixelData;
  size_t sz = d->dataSize();

  for (;sz != 0; sz-=4, ++src, ++dest) {
    *dest = ARGB2RGBA(*src);
  }

  LOGI("PIXELDATA %d", sz);
}

CompoImage CompoImage::fromPNGData( const unsigned char *data,
    const size_t dataSize)
{
  LOGI("Loading from data... [%X]", (unsigned) data);
  QImage pngImage;
  pngImage.loadFromData(data, dataSize);
  LOGI("Data [%X] Loaded!", (unsigned) data);

  return CompoImage(&pngImage);
}
