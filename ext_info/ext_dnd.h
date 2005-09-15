#ifndef EXTDRAGNDROPH
#define EXTDRAGNDROPH

#include <qscrollview.h>
#include <qlabel.h>

bool ParseDragEnterEvent (QDropEvent *e, QString &str);

class ScrollImage : public QScrollView
{
    Q_OBJECT
protected:
    QLabel *pixmapView;
    bool scaled;
    bool center;
    bool movedPixmap;
    bool externalPixmapViewport;
    void resizeEvent(QResizeEvent *);
    void onResize();
    void dragEnterEvent (QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

public:
    ScrollImage(const QString &imagepath = "", QWidget * parent = 0, const char * name = 0, WFlags f = 0);
    ScrollImage(const QPixmap &image, QWidget * parent = 0, const char * name = 0, WFlags f = 0);
    ~ScrollImage();

    bool setImage(const QString &imagepath);
    void setImage(const QPixmap &image);
    void clearImage();
    void setScaled(bool v);
    void setCenter(bool v);

    QLabel* pixmapViewport() {return pixmapView;}
    void setPixmapViewport(QLabel* v);

    bool hasScaled() {return scaled;}
    bool hasCenter() {return center;}
signals:
    void onDragImageFile(const QString& filename);
};

#endif /* EXTDRAGNDROPH */
