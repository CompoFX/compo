#include "CompoTypes.h"
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/Container>
#include <bb/cascades/LayoutUpdateHandler>

#include "CompoViewControl.h"
#include "CompoViewControl_p.h"

#include "CompoBB10Util.h"
#include "CompoSlogger.h"
#include "CompoComposition.h"
#include "CompoImage.h"
#include "CompoRenderBufferOutput.h"
#include "CompoScreenOutput.h"

#include <QImage>
#include <QDeclarativeEngine>
#include <ctime>
#include <pthread.h>
#include <bps/bps.h>


struct _TemporaryAcquire
{
  const CompoGraphicsContext &gc;
  pthread_mutex_t *lock;

  _TemporaryAcquire( const CompoGraphicsContext &gc_,
      pthread_mutex_t *lock_) : gc(gc_), lock(lock_)
  {
    pthread_mutex_lock(lock);
    gc.grabContext();
  }

  ~_TemporaryAcquire()
  {
    gc.releaseContext();
    pthread_mutex_unlock(lock);
  }
};

struct _TemporaryLock
{
  pthread_mutex_t *lock;

  _TemporaryLock( pthread_mutex_t *lock_ ) : lock(lock_)
  {
    pthread_mutex_lock(lock);
  }

  ~_TemporaryLock()
  {
    pthread_mutex_unlock(lock);
  }
};

#define ACQUIRE(g,l) __attribute__((unused)) _TemporaryAcquire __acq__(g,l)
#define LOCK(l) __attribute__((unused)) _TemporaryLock __lck__(l)

using bb::cascades::ForeignWindowControl;
using bb::cascades::Container;
using bb::cascades::CustomControl;
using bb::cascades::LayoutUpdateHandler;

CompoViewControl::CompoViewControl(Container *parent) :
  CustomControl(parent),  
  d( new CompoViewControlPrivate )
{
  QObject::connect(this, SIGNAL(creationCompleted()),
      this, SLOT(onCreationCompleted()));

  Container *pContainer = Container::create();
  setRoot(pContainer);

  LayoutUpdateHandler::create(pContainer).onLayoutFrameChanged(this,
      SLOT(handleLayoutFrameUpdated(QRectF)));
}

static inline bool isZero(const float f) {
  return fabsf(f) < 0.0001;
}

void CompoViewControl::handleLayoutFrameUpdated(QRectF )
{
  // do Nothing for now
}

void CompoViewControl::onCreationCompleted()
{
  float w = preferredWidth();
  float h = preferredHeight();

  if (!isPreferredHeightSet() && !isPreferredWidthSet()) {
    w = h = 512.0f;
  }

  if (isZero(w) || !isPreferredWidthSet()) {
    w = h;
  }

  if (isZero(h) || !isPreferredHeightSet()) {
    h = w;
  }

  if (w < h) {
    h = w;
  }
  else if ( h < w) {
    w = h;
  }

  LOGI("CompoViewControl initialised with parent [0X%X] %.2fx%.2f",
      (unsigned) parent(), w, h);

  static QString wid("CompoViewControl");

  ForeignWindowControl *fw = ForeignWindowControl::create()
    .windowId(wid).preferredSize(w,
        h); 

  const QByteArray idArr = fw->windowId().toAscii();

  screen_create_context(&d->mScreenContext, SCREEN_APPLICATION_CONTEXT);
  screen_create_window_type(&d->mScreenWindow, d->mScreenContext,
      SCREEN_CHILD_WINDOW);

  screen_join_window_group(d->mScreenWindow, fw->windowGroup().toAscii().constData());

  screen_set_window_property_cv(d->mScreenWindow,
      SCREEN_PROPERTY_ID_STRING,
      idArr.length(),
      idArr.constData());

  int vis = 1;
  screen_set_window_property_iv(d->mScreenWindow,
      SCREEN_PROPERTY_VISIBLE,
      &vis);

  int format = SCREEN_FORMAT_RGBX8888;

  screen_set_window_property_iv(d->mScreenWindow, 
      SCREEN_PROPERTY_FORMAT, &format);

  int usage =  SCREEN_USAGE_OPENGL_ES2 | SCREEN_USAGE_ROTATION;
  
  screen_set_window_property_iv(d->mScreenWindow, 
      SCREEN_PROPERTY_USAGE, &usage);
  
  int s_size[2] = {w,h};
  screen_set_window_property_iv(d->mScreenWindow, 
      SCREEN_PROPERTY_BUFFER_SIZE, s_size);
  
  int size[2];
  screen_get_window_property_iv(d->mScreenWindow, 
    SCREEN_PROPERTY_BUFFER_SIZE, size);
 
  LOGI("Screen Size [%dx%d]", size[0], size[1]);

  int z = -5;
  screen_set_window_property_iv(d->mScreenWindow,
      SCREEN_PROPERTY_ZORDER, &z);

  screen_create_window_buffers(d->mScreenWindow, 2);
  
  ((Container*)root())->add(fw); 

  d->mGLContext.init((void*) d->mScreenWindow);  

  glViewport(0,0,w,h); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  
  d->mCompo = new CompoComposition(CompoImage("demo.jpg"));
  d->mGLContext.releaseContext();

  {
    LOCK(&d->mViewLock);
    pthread_t rendert;
    pthread_create( &rendert, 0, CompoViewControl::renderFunc,
     this);
  } 
}

void CompoViewControl::onWindowAttached(screen_window_t, QString const&,
    QString const &) {}

QImage CompoViewControl::resultImage() const
{
  {
    ACQUIRE(d->mGLContext, &d->mFrameLock);
    const int imageWidth  = 512;
    const int imageHeight = 512;
    const int imageStride = imageWidth * 4;
    const int imageByteSize = imageHeight * imageStride;
    uint8_t *imageBuffer = (uint8_t*) malloc(imageByteSize);
    
    if (d->mCompo != 0) {
      CompoRenderBufferOutput offScreen(imageWidth, imageHeight);
      offScreen.render(*(d->mCompo));
      LOGI(" ---->> Grabbing Frame...");
      glReadPixels(0, 0, imageWidth, imageHeight, GL_RGBA, 
        GL_UNSIGNED_BYTE, imageBuffer);
    
      uint32_t *p = (uint32_t*) imageBuffer; 
      size_t sz = imageByteSize;

      for(; sz != 0; sz -= 4, ++p) {
        *p = RGBA2ARGB(*p);
      }
      QMatrix flipVert;
      flipVert.scale(1, -1);

      return QImage(imageBuffer, imageWidth, imageHeight, QImage::Format_ARGB32)
        .transformed(flipVert);
    }
    free(imageBuffer);
  }
  return QImage();
}

void CompoViewControl::saveResult( const QString &filePath )
{
  resultImage().save(filePath + (filePath.endsWith(".jpg") ? "" : ".jpg"));
}

CompoViewControl::~CompoViewControl()
{
  d->bRenderDone = true;
  LOGI("Waiting for render thread to stop...");
  {
    LOCK(&d->mViewLock);
    delete d->mCompo; d->mCompo = 0;
  }

  LOGI("Window and Context Destroyed");
  screen_destroy_window(d->mScreenWindow);
  screen_destroy_context(d->mScreenContext);
}

void CompoViewControl::setImage( const QImage &image )
{
  {
    ACQUIRE(d->mGLContext, &d->mFrameLock);
    if (d->mCompo !=0) {
      d->mCompo->setImage(CompoImage((void*)&image));
    }
  } 
  emit imageChanged(image);
}

void CompoViewControl::setImage( const QString &path )
{
  setImage(reorientedImage(path));
}

void CompoViewControl::setEffect( const CompoViewControl::CompoEffect effect)
{
  LOGI("Setting effect %d", effect);

  {
    ACQUIRE(d->mGLContext, &d->mFrameLock);
    mCurrentEffect = effect;
  
    if (d->mCompo != 0) {
      d->mCompo->setActiveEffect((CompoComposition::CompoCompositionEffect) effect);
    }
  }

  emit effectChanged(effect);
}

CompoViewControl::CompoEffect CompoViewControl::effect() const
{
  return mCurrentEffect;
}


void *CompoViewControl::renderFunc( void *view )
{
  CompoViewControl *thisView = (CompoViewControl*) view;
  {
    LOCK(&thisView->d->mViewLock); 
    static const struct timespec spec = {0, 66666666};

    bps_initialize();
  
    CompoGraphicsContext &ctx = thisView->d->mGLContext;
    CompoComposition *compo = thisView->d->mCompo;
    ctx.grabContext();
    CompoScreenOutput screenOutput(ctx);
    ctx.releaseContext();
    while(!thisView->d->bRenderDone)
    {
      nanosleep(&spec, 0); 
      {
        ACQUIRE(ctx, &thisView->d->mFrameLock);
        screenOutput.render(*compo); 
        ctx.swap();
      }
    } 
    bps_shutdown();
  } 
  return 0;
}

  __attribute__((constructor))
static void initializeCompoView()
{
  LOGI("com.compofx registers CompoViewControl to CompoView");
  qmlRegisterType<CompoViewControl>("com.compofx", 1, 0, "CompoView");
}

  __attribute__((destructor))
static void endCompoView()
{
  LOGI("CompoView library unloaded from memory");
}
