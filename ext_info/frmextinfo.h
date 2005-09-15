#ifndef FRMExtInfo_H
#define FRMExtInfo_H

#include <qvariant.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qtextbrowser.h>
#include "extlist.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QButtonGroup;
class QLineEdit;
class QLabel;
class QComboBox;
class QPushButton;
class QTextEdit;
class QCheckBox;
class QFrame;
class QScrollView;
class GetDataFromKadu;
class ScrollImage;

class TextBrowser : public QTextBrowser
{
    Q_OBJECT
private slots:
    void onLinkClicked(const QString &link);
public:
    TextBrowser(QWidget * parent = 0, const char * name = 0);
    ~TextBrowser();
    void setSource(const QString &name); // a wszystko to zeby po kliknieciu w link nie otwieralo sie w tym samym :P
};

class frmExtInfo : public QDialog
{
    Q_OBJECT

public:
    frmExtInfo(QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    ~frmExtInfo();

    QTabWidget* tabWidget;

    QWidget* tabGeneral;
    TextBrowser* tbGeneral;

    QWidget* tabInfo;
    QButtonGroup* bgInfo;
    QLineEdit* leBirthDay;
    QLineEdit* lePhone;
    QLabel* tlPhone;
    QComboBox* cbSection;
    QLabel* tlSection;
    QLabel* tlBirthDay;
    QLineEdit* leLastName;
    QLineEdit* leNickname;
    QLabel* tlFirstName;
    QLabel* tlNameDay;
    QLabel* tlNickname;
    QLineEdit* leMobile;
    QLabel* tlMobile;
    QLineEdit* leFirstName;
    QLineEdit* leNameDay;
    QLabel* tlLastName;
    QLabel* tlInterests;
    QLineEdit* leInterests;
    QPushButton* pbRemoveSection;
    QPushButton* pbGetData;
    QButtonGroup* bgAdress;
    QLineEdit* leStreet;
    QLabel* tlStreet;
    QLabel* tlCity;
    QLineEdit* leCity;

    QWidget* tabNet;
    QButtonGroup* bgNet;
    QLineEdit* leSecondGG;
    QLabel* tlSecondGG;
    QLineEdit* leTlen;
    QLineEdit* leEmail2;
    QLineEdit* leEmail1;
    QLineEdit* leWWW;
    QLineEdit* leWP;
    QLabel* tlTlen;
    QLineEdit* leIRC;
    QLabel* tlICQ;
    QLabel* tlWP;
    QLabel* elEmail1;
    QLabel* tlIRC;
    QLabel* tlEmail2;
    QLabel* tlWWW;
    QLineEdit* leICQ;

    QWidget* tabMemo;
    QTextEdit* teMemo;
    QLabel* tlMemo;

    QWidget* tabPhoto;
    QLabel* tlPathImage;
    QButtonGroup* bgPhoto;
    QPushButton* pbLoadImage;
    QPushButton* pbRemoveImage;
    QPushButton* pbAvatar;
    QCheckBox* cbScaled;
    //QScrollView* fPhoto;
    ScrollImage* siPhoto;

    //QWidget* wPhoto;
    //QLabel* plPhoto;
    QPushButton* pbOk;
    QPushButton* pbCancel;
    QPushButton* pbAbout;

    ExtList extlist;

public slots:
    void show(const ExtList &src, const QString & sectionName );
    void renameSection(const QString& oldName, const QString& newName);
    void setCurrentSection(const QString& sectionName);

protected:
    QGridLayout* frmExtInfoLayout;

    QGridLayout* tabGeneralLayout;

    QGridLayout* tabInfoLayout;
    QGridLayout* bgInfoLayout;
    QGridLayout* lGeneral;
    QGridLayout* lInterests;
    QGridLayout* layout18;
    QGridLayout* lSectionButtons;
    QSpacerItem* sRemoveSection;
    QSpacerItem* sGetData;
    QGridLayout* bgAdressLayout;
    QGridLayout* tabNetLayout;
    QGridLayout* bgNetLayout;
    QGridLayout* lNet;
    QGridLayout* tabMemoLayout;
    QGridLayout* tabPhotoLayout;
    QVBoxLayout* bgPhotoLayout;
    QHBoxLayout* lBottomPanel;
    QSpacerItem* sOk;
    QSpacerItem* sCancelAbout;
    //void resizeEvent ( QResizeEvent * resize);
    void scaledPhoto();

protected slots:
    virtual void languageChange();

private:
    QString currentSection;
    GetDataFromKadu *kaduData;
    QString photo_path;
    QString getPhotoPath(const QString &photopath = "");

    QString infoTemplate;

    int clicked;

    bool loadInfoTemplate();
    int getCheckBoxItem(const QString& name);
    virtual void closeEvent (QCloseEvent * e);

private slots:
    void loadSection();
    void updateSection();
    void updateInfoTab();
    void loadImage(const QString & image, bool drop = false);

    void cbChangeSection( const QString & name );
    void memoCharsUpdate();
    void testFormatBirthday();
    void testFormatNameday();
    void showAbout();
    void clickedOk();
    void clockedCancel();
    void clickedRemoveSection();
    void clickedRemoveImage();
    void clickedLoadImage();
    void clickedAvatar();
    void checkScaledImage( int scaled );
    void getDataFromKadu();
    void tabCurrentChanged(QWidget *);
    void onDragImageFile(const QString& filename);

signals:
    void acceptChanges(const ExtList&);
    void closeWindow();
};

#endif // FRMExtInfo_H

