#include "frmavatar.h"
#include "ext_general.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qframe.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qfiledialog.h>
#include <qfile.h>
#include "frmsize.h"

frmAvatar::frmAvatar(const QString &filename,const QString &photo, QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ),
    image(NULL),
    locked(true),
    autoChange(false),
    lastModified('w'),
    fileName(filename),
    avWidth(100),
    avHeight(140)
{
    imageLock[0].load(extinfo->moduleDataPath("locked.png"));
    imageLock[1].load(extinfo->moduleDataPath("unlocked.png"));
    if ( !name )
    setName( "frmAvatar" );
    frmAvatarLayout = new QHBoxLayout( this, 11, 6, "frmAvatarLayout");

    siImage = new ScrollImage("", this, "siImage" );
    frmAvatarLayout->addWidget( siImage );

    frameSettings = new QFrame( this, "frameSettings" );
    frameSettings->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)5, 0, 0, frameSettings->sizePolicy().hasHeightForWidth() ) );
    frameSettings->setFrameShape( QFrame::NoFrame );
    frameSettings->setFrameShadow( QFrame::Plain );
    frameSettingsLayout = new QGridLayout( frameSettings, 1, 1, 0, 0, "frameSettingsLayout");

    layoutSettings = new QVBoxLayout( 0, 0, 6, "layoutSettings");

    framePreview = new QFrame( frameSettings, "framePreview" );
    framePreview->setFrameShape( QFrame::Box );
    framePreview->setFrameShadow( QFrame::Raised );
    framePreviewLayout = new QGridLayout( framePreview, 1, 1, 2, 0, "framePreviewLayout");

    pixmapPreview = new QLabel( framePreview, "pixmapPreview" );
    pixmapPreview->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pixmapPreview->sizePolicy().hasHeightForWidth() ) );
    pixmapPreview->setFrameShape( QLabel::NoFrame );
    pixmapPreview->setFrameShadow( QLabel::Plain );
    pixmapPreview->setLineWidth( 1 );

    framePreviewLayout->addWidget( pixmapPreview, 0, 0 );
    layoutSettings->addWidget( framePreview );

    pbAvatarSettings = new QPushButton( frameSettings, "pbAvatarSettings" );
    layoutSettings->addWidget( pbAvatarSettings );

    imageX = new QSpinBox(0, 256*256, 1, frameSettings, "imageX" );
    layoutSettings->addWidget( imageX );
    imageY = new QSpinBox(0, 256*256, 1, frameSettings, "imageY" );
    layoutSettings->addWidget( imageY );

    layoutSize = new QHBoxLayout( 0, 0, 6, "layoutSize");

    layoutImageSize = new QVBoxLayout( 0, 0, 6, "layoutImageSize");

    imageWidth = new QSpinBox(1, 256*256, 1, frameSettings, "imageWidth" );
    layoutImageSize->addWidget( imageWidth );

    imageHeight = new QSpinBox(1, 256*256, 1, frameSettings, "imageHeight" );
    imageHeight->setEnabled(false);
    layoutImageSize->addWidget( imageHeight );
    layoutSize->addLayout( layoutImageSize );

    lockSize = new QPushButton( frameSettings, "lockSize" );
    lockSize->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)4, (QSizePolicy::SizeType)4, 0, 0, lockSize->sizePolicy().hasHeightForWidth() ) );
    lockSize->setPixmap( imageLock[0] );
    lockSize->setFlat( TRUE );
    layoutSize->addWidget( lockSize );
    layoutSettings->addLayout( layoutSize );
    spacer = new QSpacerItem( 21, 60, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layoutSettings->addItem( spacer );

    pbLoadImage = new QPushButton( frameSettings, "pbLoadImage" );
    layoutSettings->addWidget( pbLoadImage );

    //pbSave = new QPushButton( frameSettings, "pbSave" );
    //layoutSettings->addWidget( pbSave );
    pbOk = new QPushButton(frameSettings, "pbOk");
    pbOk->setDefault(TRUE);
    layoutSettings->addWidget(pbOk);

    pbCancel = new QPushButton(frameSettings, "pbCancel");
    layoutSettings->addWidget(pbCancel);

    frameSettingsLayout->addLayout( layoutSettings, 0, 0 );
    frmAvatarLayout->addWidget( frameSettings );
    languageChange();
    resize( QSize(500, 300).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    if (QFile::exists(photo))
        loadImage(photo);
    applySettings();
    connect(lockSize, SIGNAL(clicked()), this, SLOT(onLockClicked()));
    connect(imageX, SIGNAL(valueChanged(int)), this, SLOT(onSelectedChanged(int)));
    connect(imageY, SIGNAL(valueChanged(int)), this, SLOT(onSelectedChanged(int)));
    connect(imageWidth, SIGNAL(valueChanged(int)), this, SLOT(onSelectedChanged(int)));
    connect(imageHeight, SIGNAL(valueChanged(int)), this, SLOT(onSelectedChanged(int)));
    connect(pbAvatarSettings, SIGNAL(clicked()), this, SLOT(onSettingsClicked()));
    connect(pbLoadImage, SIGNAL(clicked()), this, SLOT(onLoadImagaClicked()));
    connect(pbOk, SIGNAL(clicked()), this, SLOT(onOkClicked()));
    connect(pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(siImage, SIGNAL(onDragImageFile(const QString&)), this, SLOT(onDragImageFile(const QString&)));
    //connect(pbSave, SIGNAL(clicked()), this, SLOT(onSaveImageClicked()));
}

frmAvatar::~frmAvatar()
{
}

void frmAvatar::languageChange()
{
    setCaption(tr("Avatar"));
    pbAvatarSettings->setText( tr( "Settings" ) );
    lockSize->setText( QString::null );
    pbLoadImage->setText( tr( "Load image" ) );
    pbOk->setText(tr("OK"));
    pbCancel->setText(tr("Cancel"));
    //pbSave->setText( tr( "Save" ) );
}

void frmAvatar::applySettings()
{
    pixmapPreview->setFixedSize(avWidth,avHeight);
}

void frmAvatar::onLockClicked()
{
    locked = !locked;
    lockSize->setPixmap(imageLock[locked ? 0 : 1]);
    imageHeight->setEnabled(!locked);
    if (image)
    {
        pixmapImage->setScaled(locked);
        onEndChangedSelected(pixmapImage->getSelected());
    }
}

void frmAvatar::onSelectedChanged(int)
{
    if (autoChange || !image)
        return;
    pixmapImage->setSelected(QRect(imageX->value(), imageY->value(), imageWidth->value(), imageHeight->value()));
}

void frmAvatar::onLoadImagaClicked()
{
    QString image = QFileDialog::getOpenFileName(fileName,tr("All Pixmaps") +
        QString(" (*.bmp;*.jpg;*.jpeg;*.png;*.pgm;*.pbm;*.ppm;*.xbm;*.xpm);;") +
        tr("All files") + QString(" (*)"));
    if (image.length())
        loadImage(image);
}

/*void frmAvatar::onSaveImageClicked()
{
    QString image = QFileDialog::getSaveFileName("/home/michal/images",tr("All Pixmaps") +
        QString(" (*.bmp;*.jpg;*.jpeg;*.png;*.pgm;*.pbm;*.ppm;*.xbm;*.xpm);;") +
        tr("All files") + QString(" (*)"));
    if (image.length())
        pixmapPreview->pixmap()->save(image,"JPEG",90);
}*/

void frmAvatar::onOkClicked()
{
    if (pixmapPreview->pixmap()->save(fileName,"JPEG",90))
        accept();
    else
        reject();
}

bool frmAvatar::loadImage(const QString &fileName)
{
    QPixmap *buff = new QPixmap;
    if (!buff->load(fileName))
    {
        delete buff;
        return false;
    }
    if (image)
    {
        delete image;
        //svImage->removeChild(pixmapImage);
        disconnect (pixmapImage, SIGNAL(onEndChangedSelected(const QRect &)), this, SLOT(onEndChangedSelected(const QRect &)));
        disconnect (pixmapImage, SIGNAL(onChangeSelected(const QRect &)), this, SLOT(onChangeSelected(const QRect &)));
        siImage->clearImage();
        delete pixmapImage;
    }
    image = buff;
    pixmapImage = new Pixmap(*image, locked, avWidth, avHeight, siImage->viewport(), "pixmapImage");
    pixmapImage->show();
    //svImage->addChild(pixmapImage);
    siImage->setPixmapViewport(pixmapImage);
    connect (pixmapImage, SIGNAL(onChangeSelected(const QRect &)), this, SLOT(onChangeSelected(const QRect &)));
    connect (pixmapImage, SIGNAL(onEndChangedSelected(const QRect &)), this, SLOT(onEndChangedSelected(const QRect &)));
    onChangeSelected(pixmapImage->getSelected());
    onEndChangedSelected(pixmapImage->getSelected());
    return true;
}

void frmAvatar::onChangeSelected(const QRect &rect)
{
    autoChange = true;
    imageX->setValue(rect.x());
    imageY->setValue(rect.y());
    imageWidth->setValue(rect.width());
    imageHeight->setValue(rect.height());
    autoChange = false;
}

void frmAvatar::onEndChangedSelected(const QRect &rect)
{
    QImage img;
    img = *image;
    pixmapPreview->setPixmap(img.copy(rect).smoothScale(avWidth, avHeight));
}

void frmAvatar::onSettingsClicked()
{
    frmSize frmsize(avWidth, avHeight, this);
    if (frmsize.exec() == QDialog::Accepted)
    {
        avWidth = frmsize.getWidth();
        avHeight = frmsize.getHeight();
        applySettings();
        if (image)
        {
            pixmapImage->setAvatarSize(avWidth, avHeight);
        }
    }
}

void frmAvatar::onDragImageFile(const QString& filename)
{
    loadImage(filename);
}

Pixmap::Pixmap(const QPixmap &picture, bool scaled, int W, int H, QWidget* parent, const char* name)
    :QLabel(parent, name), avScaled(scaled), avRatio(double(H) / double(W))
{
    setPixmap(picture);
    setCursor(Qt::CrossCursor);
    setFixedSize(picture.width(), picture.height());
    moveXY(-1,-1);
    moveWH(width(), height());
    //setAcceptDrops(TRUE);
}

Pixmap::~Pixmap()
{
}

void Pixmap::paintEvent ( QPaintEvent * event)
{
    QLabel::paintEvent(event);
    QPainter paint( this );
    paint.setPen(QPen(Qt::cyan,1));
    paint.drawLine(0,y,width(),y);
    paint.drawLine(x,0,x,height());
    paint.setPen(QPen(Qt::green,1));
    paint.drawLine(0,h,width(),h);
    paint.drawLine(w,0,w,height());
}

void Cadr(int min, int max, int &v)
{
    if (v < min)
        v = min;
    else if (v > max)
        v = max;
}

void Pixmap::moveXY(int X, int Y, bool Emit)
{
    Cadr(-1, width() - 2, X);
    Cadr(-1, height() - 2, Y);
    moveLine(x, y, X, Y);
    int nw = (w - x <= 1) ? x + 2 : w;
    int nh = (h - y <= 1) ? y + 2 : h;
    if ((nw != w) || (nh != h) || avScaled)
        moveWH(nw, nh, false);
    if (Emit)
        emit onChangeSelected(getSelected());
}

void Pixmap::moveWH(int W, int H, bool Emit)
{
    Cadr(1, width(), W);
    Cadr(1, height(), H);
    moveLine(w, h, W, H);
    int nx = (w - x <= 1) ? w - 2 : x;
    int ny = (h - y <= 1) ? h - 2 : y;
    if ((nx != x) || (ny != y))
        moveXY(nx, ny, false);
    if (avScaled)
    {
        moveLine(w, h, W, int(double(W - x)*avRatio + y));
    }
    if (Emit)
        emit onChangeSelected(getSelected());
}

void Pixmap::moveLine(int &a, int &b, int na, int nb)
{
    int dx = a, dy = b;
    a = na;
    b = nb;
    update(a,0,1,height());
    update(0,b,width(),1);
    update(dx,0,1,height());
    update(0,dy,width(),1);
}

QRect Pixmap::getSelected()
{
    return QRect(x + 1, y + 1, w - x - 1, h - y - 1);
}

void Pixmap::setScaled(bool scaled)
{
    avScaled = scaled;
    if (scaled)
        moveXY(x, y);
}

void Pixmap::setSelected(const QRect &rect)
{
    moveXY(rect.x() - 1, rect.y() - 1, false);
    moveWH(rect.width() + x + 1, rect.height() + y + 1);
    emit onEndChangedSelected(getSelected());
}

void Pixmap::setAvatarSize(int W, int H)
{
    avRatio = double(H) / double(W);
    moveXY(x, y, true);
}

void Pixmap::mousePressEvent(QMouseEvent *e)
{
    mouseMoveEvent(e);
}

void Pixmap::mouseMoveEvent(QMouseEvent *e)
{
    if ((e->state() == Qt::LeftButton) || (e->button() == Qt::LeftButton))
        moveXY(e->x(),e->y());
    else if ((e->state() == Qt::RightButton) || (e->button() == Qt::RightButton))
        moveWH(e->x(),e->y());
}

void Pixmap::mouseReleaseEvent(QMouseEvent*)
{
    emit onEndChangedSelected(getSelected());
}

/*AvatarScrollImage::AvatarScrollImage(QWidget * parent, const char * name, WFlags f)
    :ScrollImage("",parent, name, f)
{
}

void AvatarScrollImage::setImage(const QPixmap &image)
{
    if (pixmapView == NULL)
    {
        pixmapView = new Pixmap(image, );
        addChild(pixmapView);
        pixmapView->show();
    }
    pixmapView->setPixmap(image);
    setScaled(scaled);
}*/
/*void Pixmap::dragEnterEvent (QDragEnterEvent *e)
{
    QString str;
    ParseDragEnterEvent(e,str);
    QMessageBox::information(this,"Jeje :)", str, QMessageBox::Ok);
}*/
