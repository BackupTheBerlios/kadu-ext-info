#ifndef FRMAVATAR_H
#define FRMAVATAR_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include <qscrollview.h>
#include <qpainter.h>
#include <qlabel.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QPushButton;
class QSpinBox;

class Pixmap : public QLabel
{
    Q_OBJECT

public:
    Pixmap(const QPixmap &picture, bool scaled, int W, int H, QWidget* parent = 0, const char* name = 0);
    ~Pixmap();

    QRect getSelected();

    void setSelected(const QRect &rect);
    void setScaled(bool scaled);
    void setAvatarSize(int W, int H);

private:
    int x,y,w,h;
    bool avScaled;
    double avRatio;
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent * e);
    void moveXY(int X, int Y, bool Emit = true);
    void moveWH(int W, int H, bool Emit = true);
    void moveLine(int &a, int &b, int na, int nb);

signals:
    void onChangeSelected(const QRect &rect);
    void onEndChangedSelected(const QRect &rect);
};

class frmAvatar : public QDialog
{
    Q_OBJECT

public:
    frmAvatar(const QString &filename,const QString &photo = "", QWidget* parent = 0, const char* name = 0, bool modal = TRUE, WFlags fl = 0 );
    ~frmAvatar();

    QScrollView* svImage;
    QFrame* frameSettings;
    QFrame* framePreview;
    QLabel* pixmapPreview;
    QPushButton* pbAvatarSettings;
    QSpinBox* imageX;
    QSpinBox* imageY;
    QSpinBox* imageWidth;
    QSpinBox* imageHeight;
    QPushButton* lockSize;
    QPushButton* pbLoadImage;
    //QPushButton* pbSave;
    QPushButton* pbOk;
    QPushButton* pbCancel;

    Pixmap* pixmapImage;

protected:
    QHBoxLayout* frmAvatarLayout;
    QGridLayout* frameSettingsLayout;
    QVBoxLayout* layoutSettings;
    QSpacerItem* spacer;
    QGridLayout* framePreviewLayout;
    QHBoxLayout* layoutSize;
    QVBoxLayout* layoutImageSize;

protected slots:
    virtual void languageChange();

private:
    QPixmap imageLock[2];
    QPixmap *image;
    bool locked;
    bool autoChange;
    char lastModified;
    bool loadImage(const QString &fileName);

    QString fileName;

    //Avatar
    int avWidth;
    int avHeight;

private slots:
    void onLockClicked();
    void onSelectedChanged(int value);
    void onSettingsClicked();
    void onLoadImagaClicked();
    void onOkClicked();
    //void onSaveImageClicked();
    void onChangeSelected(const QRect &rect);
    void onEndChangedSelected(const QRect &rect);

    //Avatar
    void applySettings();

};

#endif // FRMAVATAR_H
