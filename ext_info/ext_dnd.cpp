#include "ext_dnd.h"
#include <qmessagebox.h>
#include <qdragobject.h>
#include <qlabel.h>
#include <qfile.h>

bool ParseDragEnterEvent (QDropEvent *e, QString &str)
{
    if (QTextDrag::canDecode(e) || QImageDrag::canDecode(e) || QUriDrag::canDecode(e))
        e->accept();
    const char *f;
    str = "";
    for( int i=0; (f=e->format( i )); i++ )
    {
        if ( *(f) ) {
            if ( !str.isEmpty() )
            str += "\n";
            str += f;
        }
    }
    return true;
}

ScrollImage::ScrollImage(const QString &imagepath, QWidget * parent, const char * name, WFlags f)
    :QScrollView(parent, name, f), pixmapView(NULL), scaled(false), center(true), movedPixmap(true), externalPixmapViewport(false)
{
    setImage(imagepath);
    setAcceptDrops(TRUE);
}

ScrollImage::ScrollImage(const QPixmap &image, QWidget * parent, const char * name, WFlags f)
    :QScrollView(parent, name, f), pixmapView(NULL), scaled(false), center(true), movedPixmap(true), externalPixmapViewport(false)
{
    setImage(image);
    setAcceptDrops(TRUE);
}

ScrollImage::~ScrollImage()
{
}

bool ScrollImage::setImage(const QString &imagepath)
{
    if (imagepath.length() && QFile::exists(imagepath))
    {
        QPixmap buff;
        if (buff.load(imagepath))
        {
            setImage(buff);
            return true;
        }
    }
    return false;
}

void ScrollImage::setImage(const QPixmap &image)
{
    setContentsPos(0,0);
    if (pixmapView == NULL)
    {
        pixmapView = new QLabel(viewport(), "plPhoto");
        addChild(pixmapView);
        externalPixmapViewport = false;
        pixmapView->show();
    }
    pixmapView->setPixmap(image);
    setScaled(scaled);
}

void ScrollImage::setPixmapViewport(QLabel* v)
{
    setContentsPos(0,0);
    if (pixmapView)
        clearImage();
    pixmapView = v;
    addChild(pixmapView);
    externalPixmapViewport = true;
    setScaled(pixmapView->hasScaledContents());
}

void ScrollImage::clearImage()
{
    if (pixmapView == NULL)
        return;
    removeChild(pixmapView);
    if (externalPixmapViewport == false)
        delete pixmapView;
    pixmapView = NULL;
}

void ScrollImage::setScaled(bool v)
{
    scaled = v;
    if (pixmapView)
    {
        if (v)
        {
            pixmapView->setScaledContents(v);
            setVScrollBarMode(QScrollView::AlwaysOff);
            setHScrollBarMode(QScrollView::AlwaysOff);
        }
        else
        {
            setVScrollBarMode(QScrollView::Auto);
            setHScrollBarMode(QScrollView::Auto);
            pixmapView->setFixedSize(pixmapView->pixmap()->width(),pixmapView->pixmap()->height());
        }
    }
    onResize();
}

void ScrollImage::setCenter(bool v)
{
    center = v;
    onResize();
}

void ScrollImage::resizeEvent(QResizeEvent *e)
{
    QScrollView::resizeEvent(e);
    onResize();
}

void ScrollImage::onResize()
{
    if (pixmapView == NULL)
        return;
    if (scaled)
    {
        setContentsPos(0,0);
        float scaleW = float(visibleWidth()) / float(pixmapView->pixmap()->width());
        float scaleH = float(visibleHeight()) / float(pixmapView->pixmap()->height());
        float scale = scaleW > scaleH ? scaleH : scaleW;
        int w = int(float(pixmapView->pixmap()->width()) * scale);
        int h = int(float(pixmapView->pixmap()->height()) * scale);
        pixmapView->setFixedSize(w, h);
        if (center)
        {
            int x = (visibleWidth() > w) ? (visibleWidth() - w)/2 : 0;
            int y = (visibleHeight() > h) ? (visibleHeight() - h)/2 : 0;
            pixmapView->move(x,y);
        }
        else if (pixmapView->pos().x() || pixmapView->pos().y())
            pixmapView->move(0,0);
    }
    else
    {
        if (center)
        {
            int x = (visibleWidth() > pixmapView->pixmap()->width()) ? (visibleWidth() - pixmapView->pixmap()->width())/2 : 0;
            int y = (visibleHeight() > pixmapView->pixmap()->height()) ? (visibleHeight() - pixmapView->pixmap()->height())/2 : 0;
            if (x || y)
            {
                pixmapView->move(x,y);
                movedPixmap = true;
            }
            else if (movedPixmap)
            {
                pixmapView->move(0,0);
                movedPixmap = false;
            }
        }
        else if (pixmapView->pos().x() || pixmapView->pos().y())
            pixmapView->move(0,0);
    }
}

void ScrollImage::dragEnterEvent (QDragEnterEvent *e)
{
    if (QUriDrag::canDecode(e))
    {
        e->accept();
    }
}

void ScrollImage::dragMoveEvent (QDragMoveEvent *e)
{
    e->acceptAction(e->action() == QDropEvent::Copy);
}

void ScrollImage::dropEvent(QDropEvent *e)
{
    QStrList strings;
    if ( QUriDrag::decode( e, strings ) )
    {
        QStringList files;
        if ( QUriDrag::decodeLocalFiles( e, files ) )
        {
            if (files.count() == 1)
                emit(onDragImageFile(files[0]));
        }
    }
}
