#ifdef COMPO_TARGET_IOS
#include "CompoUtil.h"
#endif

struct CompoImage::Private
{
  unsigned char *pixelData;
  int width, height, bpp; 
  private:
    int pstride;
  public:  
  
  inline Private(): pixelData(0), width(0), height(0), bpp(32), pstride(0) {}
  
  inline Private(const int width, const int height, void *imageData, 
      bool isCopied);
  
  inline size_t stride() const {
    if (pstride == 0) {
      return (width * bytePerPixel());
    }
    else {
      return pstride;
    }
  }
  
  inline size_t dataSize() const {
    return (height * stride());
  }
  
  inline size_t bytePerPixel() const {
    return (bpp >> 3);
  }
  
  inline ~Private() {
    if (pixelData) {
      free(pixelData);
      pixelData = 0;
    }
  }
};

inline CompoImage::Private::Private( const int width_, const int height_,
                                    void *imageData, bool isCopied):
pixelData(0), width(width_), height(height_), bpp(32), pstride(0)
{
  if (isCopied) {
    pixelData = reinterpret_cast<unsigned char*>(malloc(dataSize()));
    memcpy( pixelData, imageData, dataSize());
  }
  else {
    pixelData = reinterpret_cast<unsigned char*>(imageData);
  }
}

// CompoImage Class Implementation
int CompoImage::width() const { return d->width; }
int CompoImage::height() const { return d->height; }
int CompoImage::bpp() const { return d->bpp; }
const unsigned char *CompoImage::pixels() const { return d->pixelData; }
size_t CompoImage::dataSize() const { return d->dataSize(); }

CompoImage::CompoImage( const CompoImage &other ) : d(other.d)
{

}

CompoImage::CompoImage( const int width,
                        const int height,
                        void *imageData,
                        bool isCopied ) :
d(new Private(width, height, imageData, isCopied))
{
  
}

CompoImage::~CompoImage()
{
}

CompoImage &CompoImage::operator=(const CompoImage &other)
{
  d = other.d;
  return *this;
}
