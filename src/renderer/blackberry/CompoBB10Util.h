#ifndef COMPO_BB10_UTIL_H
#define COMPO_BB10_UTIL_H

#include <QUrl>
#include <QString>
#include <QDir>
#include <libexif/exif-data.h>

static inline const QString &assetPath()
{
  static const QString assetPathDir = 
    QDir::currentPath() + QString("/app/native/assets");
  return assetPathDir;
}

static inline const QString urlToAbsolutePathQ( const QUrl &url )
{
  return assetPath() + url.path();
}

static inline const std::string urlToAbsolutePath(const QUrl &url)
{
  return urlToAbsolutePathQ(url).toUtf8().data();
}

static inline uint32_t ARGB2RGBA( const uint32_t pixel )
{ 
  return ( (pixel >> 16 ) & 0xFF) |
    ( (pixel & 0xFF00FF00) ) |
    ( (pixel & 0xFF) << 16 );

}

static inline uint32_t RGBA2ARGB( const uint32_t pixel )
{
  return ( ( pixel & 0xFF00FF00) ) |
    ( (pixel & 0x00FF0000) >> 16) |
    ( (pixel & 0xFF ) << 16 );
}

enum 
{
  kRot0       = 1, // top - left
  kRot0Flip   = 2,
  kRot180     = 3,
  kRot180Flip = 4,
  kRot270Flip = 5,
  kRot90      = 6, // right - top
  kRot90Flip  = 7,
  kRot270     = 8  // left - bottom
};

static inline int get_orientation (ExifData *exif_data)
{
  ExifEntry *entry;

  ExifByteOrder byte_order = exif_data_get_byte_order(exif_data);

  /* get orientation and rotate image accordingly if necessary */
  if ((entry = exif_content_get_entry (exif_data->ifd[EXIF_IFD_0],
          EXIF_TAG_ORIENTATION)))
  {
    return exif_get_short(entry->data, byte_order);
  }
  return 0;
}

static QImage reorientedImage(const QString &filePath)
{
  const QImage original(filePath);

  ExifData *ed = exif_data_new_from_file(filePath.toUtf8().constData());

  if (!ed) {
    return original;
  }

  ExifEntry *entry = exif_content_get_entry(ed->ifd[EXIF_IFD_0], 
      EXIF_TAG_ORIENTATION);

  if (!entry) {
    return original;
  }

  int v = get_orientation(ed);
  
  QMatrix mat;

  switch (v)
  {
    case kRot0:
    case kRot0Flip:
      return original;
    case kRot180:
    case kRot180Flip:
      mat.rotate(180.0);
      break;
    case kRot270:
    case kRot270Flip:
      mat.rotate(270.0);
      break;
    case kRot90:
    case kRot90Flip:
      mat.rotate(90.0);
      break;
  }

  switch (v) {
    case kRot0Flip:
    case kRot180Flip:
      mat.scale(-1, 1);
      break;
    case kRot90Flip:
    case kRot270Flip:
      mat.scale(1, -1);
  }

  return original.transformed(mat);
}

#endif
