#ifndef COMPO_VIEW_CONTROL_H_BLACKBERRY
#define COMPO_VIEW_CONTROL_H_BLACKBERRY

#include <QScopedPointer>
#include <QImage>
#include <bb/cascades/CustomControl>
#include <screen/screen.h>

#if defined(COMPO_SHARED_LIB)
#define COMPO_EXPORT Q_DECL_EXPORT
#else
#define COMPO_EXPORT Q_DECL_IMPORT
#endif

namespace bb {
  namespace cascades {
    class Container;
  }
}

struct CompoViewControlPrivate;
class COMPO_EXPORT CompoViewControl : public bb::cascades::CustomControl 
{
  Q_OBJECT
  Q_ENUMS(CompoEffect)
  Q_PROPERTY(CompoEffect effect READ effect WRITE setEffect NOTIFY effectChanged)
public: 
  CompoViewControl(bb::cascades::Container *parent=0);
  
  enum CompoEffect {
    EffectNormal = 0,
    EffectGreyscale,
    EffectLighten,
    EffectSepia,
    EffectHitamPutih,
    EffectScribe,
    EffectHike,
    EffectCinta,
    EffectAnalog,
    EffectThermo,
    EffectDesaku,
    EffectKelud,
    EffectHero,
    EffectPencil,
    EffectStylish,
    EffectLordKevin,
    EffectHudson,
    EffectPixelate
  };

  virtual ~CompoViewControl();

  void setImage( const QImage& ); 
  Q_INVOKABLE void setImage( const QString& path );
  
  QImage resultImage() const;
  Q_INVOKABLE void saveResult( const QString &path );
   
  void setEffect( const CompoEffect effect );
  CompoEffect effect() const;

signals:
  void imageChanged( const QImage & );
  void effectChanged( CompoEffect effect );

private slots:
  void onCreationCompleted();
  void handleLayoutFrameUpdated(QRectF);
  void onWindowAttached( screen_window_t handle,
    const QString &group, const QString &id);

private:
  QScopedPointer<CompoViewControlPrivate> d;

  CompoViewControl( const CompoViewControl & );
  CompoViewControl &operator=( const CompoViewControl & );
  
  friend class CompoViewControlPrivate;

  static void * renderFunc( void * );
  
  CompoEffect mCurrentEffect;
};

#endif // COMPO_VIEW_H_BLACKBERRY
