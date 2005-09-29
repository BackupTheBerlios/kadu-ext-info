#include "frmextinfo.h"
#include "ext_general.h"
#include "ext_dnd.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qbuttongroup.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qscrollview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qtextcodec.h>
#include <qdir.h>
#include <qregexp.h>

#include "frmavatar.h"

#include <iostream>
using namespace std;

TextBrowser::TextBrowser (QWidget * parent, const char * name)
    :QTextBrowser(parent, name)
{
    connect(this, SIGNAL(linkClicked(const QString &)), this, SLOT(onLinkClicked(const QString &)));
}

TextBrowser::~TextBrowser()
{
    disconnect(this, SIGNAL(linkClicked(const QString &)), this, SLOT(onLinkClicked(const QString &)));
}

void TextBrowser::onLinkClicked(const QString &link)
{
    OpenWebBrowser(link);
}

void TextBrowser::setSource(const QString &)
{   // bleee :P
}

frmExtInfo::frmExtInfo(QWidget* parent, const char* name, bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl), kaduData(new GetDataFromKadu), clicked(QMessageBox::NoButton)
{
    kdebugf();
    setName( "frmExtInfo" );

    if (loadInfoTemplate() == false)
        infoTemplate = QString("Nie znaleziono pliku ") + tr(EXTINFO_INFOTEMPLATE);

    frmExtInfoLayout = new QGridLayout( this, 1, 1, 11, 6, "frmExtInfoLayout");

    tabWidget = new QTabWidget( this, "tabWidget" );

    // Pierwszy tab (General)
    tabGeneral = new QWidget(tabWidget, "tabGeneral");
    tabGeneralLayout = new QGridLayout(tabGeneral, 1, 1, 11, 6, "tabGeneralLayout");
    tbGeneral = new TextBrowser(tabGeneral, "tbGeneral");
    tabGeneralLayout->addWidget(tbGeneral, 0, 0);
    tabWidget->insertTab(tabGeneral, QString(""));

    // Tab Info
    tabInfo = new QWidget(tabWidget, "tabInfo" );
    tabInfoLayout = new QGridLayout(tabInfo, 2, 2, 11, 6, "tabInfoLayout");
    tabInfoLeftLayout = new QGridLayout(0, 8, 2, 0, 6, "tabInfoLeftLayout");
    tabInfoLayout->addLayout(tabInfoLeftLayout, 0, 0);

    leNickname = new QLineEdit(tabInfo, "leNickname");
    tabInfoLeftLayout->addWidget(leNickname, 1, 1);
    tlNickname = new QLabel(tabInfo, "tlNickname");
    tabInfoLeftLayout->addWidget(tlNickname, 1, 0);

    leFirstName = new QLineEdit(tabInfo, "leFirstName");
    tabInfoLeftLayout->addWidget(leFirstName, 2, 1);
    tlFirstName = new QLabel(tabInfo, "tlFirstName");
    tabInfoLeftLayout->addWidget(tlFirstName, 2, 0);

    leLastName = new QLineEdit(tabInfo, "leLastName");
    tabInfoLeftLayout->addWidget(leLastName, 3, 1);
    tlLastName = new QLabel(tabInfo, "tlLastName");
    tabInfoLeftLayout->addWidget(tlLastName, 3, 0);

    leNameDay = new QLineEdit(tabInfo, "leNameDay");
    tabInfoLeftLayout->addWidget(leNameDay, 4, 1);
    tlNameDay = new QLabel(tabInfo, "tlNameDay");
    tabInfoLeftLayout->addWidget(tlNameDay, 4, 0);

    leBirthDay = new QLineEdit(tabInfo, "leBirthDay");
    tabInfoLeftLayout->addWidget(leBirthDay, 5, 1);
    tlBirthDay = new QLabel(tabInfo, "tlBirthDay");
    tabInfoLeftLayout->addWidget(tlBirthDay, 5, 0);

    lePhone = new QLineEdit(tabInfo, "lePhone");
    tabInfoLeftLayout->addWidget(lePhone, 6, 1);
    tlPhone = new QLabel(tabInfo, "tlPhone");
    tabInfoLeftLayout->addWidget(tlPhone, 6, 0);

    leMobile = new QLineEdit(tabInfo, "leMobile");
    tabInfoLeftLayout->addWidget(leMobile, 7, 1);
    tlMobile = new QLabel(tabInfo, "tlMobile");
    tabInfoLeftLayout->addWidget(tlMobile, 7, 0);




    tabInfoRightLayout = new QGridLayout(NULL, 5, 2, 0, 6, "tabInfoRightLayout");
    tabInfoLayout->addLayout(tabInfoRightLayout, 0, 1);

    pbRemoveSection = new QPushButton(tabInfo, "pbRemoveSection");
    tabInfoRightLayout->addWidget(pbRemoveSection, 0, 0);
    pbGetData = new QPushButton(tabInfo, "pbGetData");
    tabInfoRightLayout->addWidget(pbGetData, 1, 0);
    sTabInfoButtons = new QSpacerItem( 31, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabInfoRightLayout->addMultiCell(sTabInfoButtons, 0, 1, 1, 1);

    bgAdress = new QButtonGroup(tabInfo, "bgAdress");
    tabInfoRightLayout->addMultiCellWidget(bgAdress, 3, 3, 0, 1);
    bgAdress->setColumnLayout(0, Qt::Vertical);
    bgAdress->layout()->setSpacing(6);
    bgAdress->layout()->setMargin(11);
    bgAdressLayout = new QGridLayout( bgAdress->layout() );

    leStreet = new QLineEdit(bgAdress, "leStreet");
    bgAdressLayout->addWidget(leStreet, 0, 1);
    tlStreet = new QLabel(bgAdress, "tlStreet");
    bgAdressLayout->addWidget(tlStreet, 0, 0);

    leCity = new QLineEdit(bgAdress, "leCity");
    bgAdressLayout->addWidget(leCity, 1, 1);
    tlCity = new QLabel(bgAdress, "tlCity");
    bgAdressLayout->addWidget(tlCity, 1, 0);



    lInterests = new QGridLayout(NULL, 2, 1, 0, 0, "lInterests");
    tabInfoRightLayout->addMultiCell(lInterests, 4, 4, 0, 1);
    leInterests = new QLineEdit(tabInfo, "leInterests");
    lInterests->addWidget(leInterests, 1, 0);
    tlInterests = new QLabel(tabInfo, "tlInterests");
    tlInterests->setAlignment(int(QLabel::AlignBottom));
    lInterests->addWidget(tlInterests, 0, 0);
    tabWidget->insertTab(tabInfo, QString(""));



/*
    bgInfo = new QButtonGroup( tabInfo, "bgInfo" );
    bgInfo->setFrameShape( QButtonGroup::NoFrame );
    bgInfo->setColumnLayout(0, Qt::Vertical );
    bgInfo->layout()->setSpacing( 6 );
    bgInfo->layout()->setMargin( 11 );
    bgInfoLayout = new QGridLayout( bgInfo->layout() );
    bgInfoLayout->setAlignment( Qt::AlignTop );

    lGeneral = new QGridLayout( 0, 1, 1, 0, 6, "lGeneral");


    bgInfoLayout->addMultiCellLayout( lGeneral, 0, 1, 0, 0 );




    bgInfoLayout->addLayout( lInterests, 1, 1 );

    layout18 = new QGridLayout( 0, 1, 1, 0, 6, "layout18");

    lSectionButtons = new QGridLayout( 0, 1, 1, 0, 6, "lSectionButtons");
    sRemoveSection = new QSpacerItem( 31, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    lSectionButtons->addItem( sRemoveSection, 1, 1 );

    sGetData = new QSpacerItem( 31, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    lSectionButtons->addItem( sGetData, 0, 1 );


    layout18->addLayout( lSectionButtons, 0, 0 );


    bgInfoLayout->addLayout( layout18, 0, 1 );

    tabInfoLayout->addMultiCellWidget(bgInfo, 1, 1, 0, 1);*/


 /*
    tabInfo = new QWidget( tabWidget, "tabInfo" );
    tabInfoLayout = new QGridLayout( tabInfo, 1, 1, 0, 6, "tabInfoLayout");

    bgInfo = new QButtonGroup( tabInfo, "bgInfo" );
    bgInfo->setFrameShape( QButtonGroup::NoFrame );
    bgInfo->setColumnLayout(0, Qt::Vertical );
    bgInfo->layout()->setSpacing( 6 );
    bgInfo->layout()->setMargin( 11 );
    bgInfoLayout = new QGridLayout( bgInfo->layout() );
    bgInfoLayout->setAlignment( Qt::AlignTop );

    lGeneral = new QGridLayout( 0, 1, 1, 0, 6, "lGeneral");

    leBirthDay = new QLineEdit( bgInfo, "leBirthDay" );

    lGeneral->addWidget( leBirthDay, 5, 1 );

    lePhone = new QLineEdit( bgInfo, "lePhone" );

    lGeneral->addWidget( lePhone, 6, 1 );

    tlPhone = new QLabel( bgInfo, "tlPhone" );

    lGeneral->addWidget( tlPhone, 6, 0 );

    tlSection = new QLabel( bgInfo, "tlSection" );

    lGeneral->addWidget( tlSection, 0, 0 );

    tlBirthDay = new QLabel( bgInfo, "tlBirthDay" );

    lGeneral->addWidget( tlBirthDay, 5, 0 );

    leLastName = new QLineEdit( bgInfo, "leLastName" );

    lGeneral->addWidget( leLastName, 3, 1 );

    leNickname = new QLineEdit( bgInfo, "leNickname" );
    leNickname->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 0, 0, leNickname->sizePolicy().hasHeightForWidth() ) );

    lGeneral->addWidget( leNickname, 1, 1 );

    tlFirstName = new QLabel( bgInfo, "tlFirstName" );

    lGeneral->addWidget( tlFirstName, 2, 0 );

    tlNameDay = new QLabel( bgInfo, "tlNameDay" );

    lGeneral->addWidget( tlNameDay, 4, 0 );

    tlNickname = new QLabel( bgInfo, "tlNickname" );

    lGeneral->addWidget( tlNickname, 1, 0 );

    leMobile = new QLineEdit( bgInfo, "leMobile" );

    lGeneral->addWidget( leMobile, 7, 1 );

    tlMobile = new QLabel( bgInfo, "tlMobile" );

    lGeneral->addWidget( tlMobile, 7, 0 );

    leFirstName = new QLineEdit( bgInfo, "leFirstName" );

    lGeneral->addWidget( leFirstName, 2, 1 );

    leNameDay = new QLineEdit( bgInfo, "leNameDay" );

    lGeneral->addWidget( leNameDay, 4, 1 );

    tlLastName = new QLabel( bgInfo, "tlLastName" );

    lGeneral->addWidget( tlLastName, 3, 0 );

    bgInfoLayout->addMultiCellLayout( lGeneral, 0, 1, 0, 0 );

    lInterests = new QGridLayout( 0, 1, 1, 0, 0, "lInterests");

    tlInterests = new QLabel( bgInfo, "tlInterests" );
    tlInterests->setAlignment( int( QLabel::AlignBottom ) );

    lInterests->addWidget( tlInterests, 0, 0 );

    leInterests = new QLineEdit( bgInfo, "leInterests" );

    lInterests->addWidget( leInterests, 1, 0 );

    bgInfoLayout->addLayout( lInterests, 1, 1 );

    layout18 = new QGridLayout( 0, 1, 1, 0, 6, "layout18");

    lSectionButtons = new QGridLayout( 0, 1, 1, 0, 6, "lSectionButtons");
    sRemoveSection = new QSpacerItem( 31, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    lSectionButtons->addItem( sRemoveSection, 1, 1 );

    pbRemoveSection = new QPushButton( bgInfo, "pbRemoveSection" );

    lSectionButtons->addWidget( pbRemoveSection, 1, 0 );
    sGetData = new QSpacerItem( 31, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    lSectionButtons->addItem( sGetData, 0, 1 );

    pbGetData = new QPushButton( bgInfo, "pbGetData" );

    lSectionButtons->addWidget( pbGetData, 0, 0 );

    layout18->addLayout( lSectionButtons, 0, 0 );

    bgAdress = new QButtonGroup( bgInfo, "bgAdress" );
    bgAdress->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)4, 0, 0, bgAdress->sizePolicy().hasHeightForWidth() ) );
    bgAdress->setColumnLayout(0, Qt::Vertical );
    bgAdress->layout()->setSpacing( 6 );
    bgAdress->layout()->setMargin( 11 );
    bgAdressLayout = new QGridLayout( bgAdress->layout() );
    bgAdressLayout->setAlignment( Qt::AlignTop );

    leStreet = new QLineEdit( bgAdress, "leStreet" );

    bgAdressLayout->addWidget( leStreet, 1, 1 );

    tlStreet = new QLabel( bgAdress, "tlStreet" );

    bgAdressLayout->addWidget( tlStreet, 1, 0 );

    tlCity = new QLabel( bgAdress, "tlCity" );

    bgAdressLayout->addWidget( tlCity, 0, 0 );

    leCity = new QLineEdit( bgAdress, "leCity" );

    bgAdressLayout->addWidget( leCity, 0, 1 );

    layout18->addWidget( bgAdress, 1, 0 );

    bgInfoLayout->addLayout( layout18, 0, 1 );

    tabInfoLayout->addWidget( bgInfo, 0, 0 );
    tabWidget->insertTab( tabInfo, QString("") );
// */

// Tab net!!!
    tabNet = new QWidget( tabWidget, "tabNet" );
    tabNetLayout = new QGridLayout( tabNet, 1, 1, 0, 6, "tabNetLayout");

    bgNet = new QButtonGroup( tabNet, "bgNet" );
    bgNet->setFrameShape( QButtonGroup::NoFrame );
    bgNet->setColumnLayout(0, Qt::Vertical );
    bgNet->layout()->setSpacing( 6 );
    bgNet->layout()->setMargin( 11 );
    bgNetLayout = new QGridLayout( bgNet->layout() );
    bgNetLayout->setAlignment( Qt::AlignTop );

    lNet = new QGridLayout( 0, 1, 1, 0, 6, "lNet");

    leSecondGG = new QLineEdit( bgNet, "leSecondGG" );

    lNet->addWidget( leSecondGG, 7, 1 );

    tlSecondGG = new QLabel( bgNet, "tlSecondGG" );

    lNet->addWidget( tlSecondGG, 7, 0 );

    leTlen = new QLineEdit( bgNet, "leTlen" );

    lNet->addWidget( leTlen, 6, 1 );

    leEmail2 = new QLineEdit( bgNet, "leEmail2" );

    lNet->addWidget( leEmail2, 1, 1 );

    leEmail1 = new QLineEdit( bgNet, "leEmail1" );

    lNet->addWidget( leEmail1, 0, 1 );

    leWWW = new QLineEdit( bgNet, "leWWW" );

    lNet->addWidget( leWWW, 2, 1 );

    leWP = new QLineEdit( bgNet, "leWP" );

    lNet->addWidget( leWP, 4, 1 );

    tlTlen = new QLabel( bgNet, "tlTlen" );

    lNet->addWidget( tlTlen, 6, 0 );

    leIRC = new QLineEdit( bgNet, "leIRC" );

    lNet->addWidget( leIRC, 3, 1 );

    tlICQ = new QLabel( bgNet, "tlICQ" );

    lNet->addWidget( tlICQ, 5, 0 );

    tlWP = new QLabel( bgNet, "tlWP" );

    lNet->addWidget( tlWP, 4, 0 );

    elEmail1 = new QLabel( bgNet, "elEmail1" );

    lNet->addWidget( elEmail1, 0, 0 );

    tlIRC = new QLabel( bgNet, "tlIRC" );

    lNet->addWidget( tlIRC, 3, 0 );

    tlEmail2 = new QLabel( bgNet, "tlEmail2" );

    lNet->addWidget( tlEmail2, 1, 0 );

    tlWWW = new QLabel( bgNet, "tlWWW" );

    lNet->addWidget( tlWWW, 2, 0 );

    leICQ = new QLineEdit( bgNet, "leICQ" );

    lNet->addWidget( leICQ, 5, 1 );

    bgNetLayout->addLayout( lNet, 0, 0 );

    tabNetLayout->addWidget( bgNet, 0, 0 );
    tabWidget->insertTab( tabNet, QString("") );

    tabMemo = new QWidget( tabWidget, "tabMemo" );
    tabMemoLayout = new QGridLayout( tabMemo, 1, 1, 11, 6, "tabMemoLayout");

    teMemo = new QTextEdit( tabMemo, "teMemo" );

    tabMemoLayout->addWidget( teMemo, 0, 0 );

    tlMemo = new QLabel( tabMemo, "tlMemo" );
    tlMemo->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    tabMemoLayout->addWidget( tlMemo, 1, 0 );
    tabWidget->insertTab( tabMemo, QString("") );

// Tab photo
    tabPhoto = new QWidget( tabWidget, "tabPhoto" );
    tabPhotoLayout = new QGridLayout( tabPhoto, 1, 1, 11, 6, "tabPhotoLayout");

    tlPathImage = new QLabel( tabPhoto, "tlPathImage" );
    tlPathImage->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Maximum, true));
    tabPhotoLayout->addMultiCellWidget( tlPathImage, 1, 1, 0, 1 );

    bgPhoto = new QButtonGroup( tabPhoto, "bgPhoto" );
    bgPhoto->setFrameShape( QButtonGroup::NoFrame );

    bgPhotoLayout = new QVBoxLayout(bgPhoto, 0, 6, "lPhoto");
    bgPhotoLayout->setAlignment( Qt::AlignTop );
    pbLoadImage = new QPushButton( bgPhoto, "pbLoadImage" );
    bgPhotoLayout->addWidget(pbLoadImage);
    pbRemoveImage = new QPushButton( bgPhoto, "pbRemoveImage" );
    bgPhotoLayout->addWidget(pbRemoveImage);
    pbAvatar = new QPushButton(bgPhoto, "pbAvatar");
    bgPhotoLayout->addWidget(pbAvatar);
    cbScaled = new QCheckBox( bgPhoto, "cbScaled" );
    bgPhotoLayout->addWidget(cbScaled);

    tabPhotoLayout->addWidget( bgPhoto, 0, 1 );
/*
    fPhoto = new QScrollView( tabPhoto, "fPhoto" );
    fPhoto->setFrameShape( QScrollView::WinPanel );
    fPhoto->setFrameShadow( QScrollView::Sunken );

    wPhoto = new QWidget(fPhoto->viewport());
    fPhoto->addChild(wPhoto);

    plPhoto = new QLabel(wPhoto, "plPhoto" );

    tabPhotoLayout->addWidget( fPhoto, 0, 0 );*/
    siPhoto = new ScrollImage("", tabPhoto, "siPhoto");
    tabPhotoLayout->addWidget(siPhoto, 0, 0);
    tabWidget->insertTab( tabPhoto, QString("") );

    frmExtInfoLayout->addWidget( tabWidget, 0, 0 );

// Koniec tabow
    lBottomPanel = new QHBoxLayout( 0, 0, 6, "lBottomPanel");
    cbSection = new QComboBox(FALSE, this, "cbSection");
    lBottomPanel->addWidget(cbSection);

    sOk = new QSpacerItem( 140, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    lBottomPanel->addItem( sOk );

    pbOk = new QPushButton( this, "pbOk" );
    pbOk->setDefault( TRUE );
    lBottomPanel->addWidget( pbOk );

    pbCancel = new QPushButton( this, "pbCancel" );
    lBottomPanel->addWidget( pbCancel );
    sCancelAbout = new QSpacerItem( 31, 21, QSizePolicy::Fixed, QSizePolicy::Minimum );
    lBottomPanel->addItem( sCancelAbout );

    pbAbout = new QPushButton( this, "pbAbout" );
    lBottomPanel->addWidget( pbAbout );

    frmExtInfoLayout->addLayout( lBottomPanel, 1, 0 );
    languageChange();
    resize( QSize(500, 200).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pbCancel, SIGNAL( clicked() ), this, SLOT(clockedCancel()) );
    connect( cbSection, SIGNAL( activated(const QString&) ), this, SLOT( cbChangeSection(const QString&) ) );
    connect( teMemo, SIGNAL( textChanged() ), this, SLOT( memoCharsUpdate() ) );
    connect( leNameDay, SIGNAL( lostFocus() ), this, SLOT( testFormatNameday() ) );
    connect( leBirthDay, SIGNAL( lostFocus() ), this, SLOT( testFormatBirthday() ) );
    connect( pbAbout, SIGNAL( clicked() ), this, SLOT( showAbout() ) );
    connect( pbOk, SIGNAL( clicked() ), this, SLOT( clickedOk() ) );
    connect( pbRemoveSection, SIGNAL( clicked() ), this, SLOT( clickedRemoveSection() ) );
    connect( cbScaled, SIGNAL( stateChanged(int) ), this, SLOT( checkScaledImage(int) ) );
    connect( pbRemoveImage, SIGNAL( clicked() ), this, SLOT( clickedRemoveImage() ) );
    connect( pbLoadImage, SIGNAL( clicked() ), this, SLOT( clickedLoadImage() ) );
    connect(pbAvatar, SIGNAL(clicked()), this, SLOT(clickedAvatar()));
    connect( pbGetData,SIGNAL( clicked() ),this, SLOT( getDataFromKadu()));
    connect( tabWidget,(SIGNAL(currentChanged ( QWidget * ))),this, SLOT(tabCurrentChanged ( QWidget * )));
    connect(siPhoto, SIGNAL(onDragImageFile(const QString&)), this, SLOT(onDragImageFile(const QString&)));

    // tab order
    setTabOrder( tabWidget, cbSection );
    setTabOrder( cbSection, leNickname );
    setTabOrder( leNickname, leFirstName );
    setTabOrder( leFirstName, leLastName );
    setTabOrder( leLastName, leNameDay );
    setTabOrder( leNameDay, leBirthDay );
    setTabOrder( leBirthDay, lePhone );
    setTabOrder( lePhone, leMobile );
    setTabOrder( leMobile, pbGetData );
    setTabOrder( pbGetData, pbRemoveSection );
    setTabOrder( pbRemoveSection, leCity );
    setTabOrder( leCity, leStreet );
    setTabOrder( leStreet, leInterests );
    setTabOrder( leInterests, leEmail1 );
    setTabOrder( leEmail1, leEmail2 );
    setTabOrder( leEmail2, leWWW );
    setTabOrder( leWWW, leIRC );
    setTabOrder( leIRC, leWP );
    setTabOrder( leWP, leICQ );
    setTabOrder( leICQ, leTlen );
    setTabOrder( leTlen, leSecondGG );
    setTabOrder( leSecondGG, teMemo );
    setTabOrder( teMemo, pbLoadImage );
    setTabOrder( pbLoadImage, pbRemoveImage );
    setTabOrder( pbRemoveImage, cbScaled );
    setTabOrder( cbScaled, pbOk );
    setTabOrder( pbOk, pbCancel );
    setTabOrder( pbCancel, pbAbout );

    // buddies
    tlPhone->setBuddy( lePhone );
    //tlSection->setBuddy( cbSection );
    tlBirthDay->setBuddy( leBirthDay );
    tlFirstName->setBuddy( leFirstName );
    tlNameDay->setBuddy( leNameDay );
    tlNickname->setBuddy( leNickname );
    tlMobile->setBuddy( leMobile );
    tlLastName->setBuddy( leLastName );
    tlInterests->setBuddy( leInterests );
    tlStreet->setBuddy( leStreet );
    tlCity->setBuddy( leCity );
    tlSecondGG->setBuddy( leSecondGG );
    tlTlen->setBuddy( leTlen );
    tlICQ->setBuddy( leICQ );
    tlWP->setBuddy( leWP );
    elEmail1->setBuddy( leEmail1 );
    tlIRC->setBuddy( leIRC );
    tlEmail2->setBuddy( leEmail2 );
    tlWWW->setBuddy( leWWW );
    kdebugf2();
}

frmExtInfo::~frmExtInfo()
{
    kdebugf();
    destroy();
    delete kaduData;
    kdebugf2();
}

bool frmExtInfo::loadInfoTemplate()
{
    QFile file;
    QString fn;
    if (QFile::exists(fn = extinfo->extinfoPath(tr(EXTINFO_INFOTEMPLATE))) == false)
        if (QFile::exists(fn = extinfo->moduleDataPath(tr(EXTINFO_INFOTEMPLATE))) == false)
            return false;
    file.setName(fn);
    if (!file.open(IO_ReadOnly))
        return false;
    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF8"));
    infoTemplate = "";
    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        infoTemplate += line;
    }
    file.close();
    return true;
}

void frmExtInfo::clockedCancel()
{
    clicked = QMessageBox::Cancel;
    close();
}

void frmExtInfo::closeEvent ( QCloseEvent * e )
{
    QString modifieds;
    if (clicked == QMessageBox::NoButton)
        saveSection(currentSection);
    if (clicked != QMessageBox::Ok)
    {
        for (ExtList::iterator i = extlist.begin(); i != extlist.end(); i++)
        {
            if ((*i).modified)
            {
                modifieds += i.key();
                modifieds += "\n";
            }
        }
        if (modifieds.length())
        {
            int ret = QMessageBox::question(this, tr("Extension info"), tr("This section have modified:") + QString("\n\n") + modifieds + QString("\n") + tr("Do you want to save changes?"),QMessageBox::Yes ,QMessageBox::No,QMessageBox::Cancel);
            if (ret == QMessageBox::Yes)
            {
                clickedOk();
            }
            else if (ret == QMessageBox::Cancel)
            {
                e->ignore();
                clicked = QMessageBox::NoButton;
                return;
            }
        }
    }
    e->accept();
    emit(closeWindow());
}

void frmExtInfo::languageChange()
{
    kdebugf();
    setCaption(tr("Extension info"));

    tabWidget->changeTab(tabGeneral, tr("Info"));
    tabWidget->changeTab(tabInfo, tr("General"));

    //bgInfo->setTitle( QString::null );
    QToolTip::add( leBirthDay, tr( "Format: dd.mm.yyyy" ) );
    QToolTip::add( lePhone, tr( "Phone number" ) );
    tlPhone->setText( tr( "Phone:" ) );
    //tlSection->setText( tr( "Section:" ) );
    tlBirthDay->setText( tr( "Birthday:" ) );
    tlFirstName->setText( tr( "First name:" ) );
    tlNameDay->setText( tr( "Name day:" ) );
    tlNickname->setText( tr( "Nickname:" ) );
    QToolTip::add( leMobile, tr( "Cellular phone number" ) );
    tlMobile->setText( tr( "Mobile:" ) );
    QToolTip::add( leNameDay, tr( "Format: dd.mm" ) );
    tlLastName->setText( tr( "Last name:" ) );
    tlInterests->setText( tr( "Interests:" ) );
    pbRemoveSection->setText( tr( "Remove description" ) );
    pbGetData->setText( tr( "Get data from Kadu" ) );
    bgAdress->setTitle( tr( "Home adress" ) );
    QToolTip::add( leStreet, tr( "Street and home number" ) );
    tlStreet->setText( tr( "Street:" ) );
    tlCity->setText( tr( "City:" ) );
    QToolTip::add( leCity, tr( "Zip and City" ) );
    bgNet->setTitle( QString::null );
    tlSecondGG->setText( tr( "Second GG:" ) );
    tlTlen->setText( tr( "Tlen Nick:" ) );
    tlICQ->setText( tr( "ICQ UIN:" ) );
    tlWP->setText( tr( "WP Kontakt:" ) );
    elEmail1->setText( tr( "E-mail:" ) );
    tlIRC->setText( tr( "IRC Nick:" ) );
    tlEmail2->setText( tr( "E-mail(2):" ) );
    tlWWW->setText( tr( "WWW:" ) );
    tabWidget->changeTab( tabNet, tr( "Net" ) );
    tlMemo->setText( tr( "0 chars" ) );
    tabWidget->changeTab( tabMemo, tr( "Memo" ) );
    tlPathImage->setText( tr( "Path image:" ) );
    bgPhoto->setTitle( QString::null );
    pbLoadImage->setText( tr( "Load image" ) );
    pbRemoveImage->setText( tr( "Remove image" ) );
    pbAvatar->setText(tr("Avatar"));
    cbScaled->setText( tr( "Scaled" ) );
    tabWidget->changeTab( tabPhoto, tr( "Photo" ) );
    pbOk->setText( tr( "OK" ) );
    pbOk->setAccel( QKeySequence( QString::null ) );
    pbCancel->setText( tr( "Cancel" ) );
    pbAbout->setText( tr( "About" ) );
    kdebugf2();
}

void frmExtInfo::loadSection(const QString &name)
{
    kdebugf();
    leNickname->setText(extlist[name].nickname);
    leLastName->setText(extlist[name].last_name);
    leFirstName->setText(extlist[name].first_name);
    leBirthDay->setText(extlist[name].birthday);
    leNameDay->setText(extlist[name].name_day);
    lePhone->setText(extlist[name].phone);
    leMobile->setText(extlist[name].mobile);
    leCity->setText(extlist[name].city);
    leStreet->setText(extlist[name].street);
    leInterests->setText(extlist[name].interests);
    leEmail1->setText(extlist[name].email[0]);
    leEmail2->setText(extlist[name].email[1]);
    leWWW->setText(extlist[name].www);
    leIRC->setText(extlist[name].irc);
    leWP->setText(extlist[name].wp);
    leICQ->setText(extlist[name].icq);
    leTlen->setText(extlist[name].tlen);
    leSecondGG->setText(extlist[name].alt_gg);
    teMemo->setText(extlist[name].memo);
    cbScaled->setChecked(extlist[name].photo_scaled);
    loadImage(extlist[name].photo_path);
    //currentUin = getGGUin();

    updateInfoTab();

    kdebugf2();
}

void frmExtInfo::saveSection(const QString &name)
{
    kdebugf();
    if (name.isEmpty())
        return;
    ExtListElement old = extlist[name];
    extlist[name].nickname = leNickname->text();
    extlist[name].last_name = leLastName->text();
    extlist[name].first_name = leFirstName->text();
    extlist[name].birthday = leBirthDay->text();
    extlist[name].name_day = leNameDay->text();
    extlist[name].phone = lePhone->text();
    extlist[name].mobile = leMobile->text();
    extlist[name].city = leCity->text();
    extlist[name].street = leStreet->text();
    extlist[name].interests = leInterests->text();
    extlist[name].email[0] = leEmail1->text();
    extlist[name].email[1] = leEmail2->text();
    extlist[name].www = leWWW->text();
    extlist[name].irc = leIRC->text();
    extlist[name].wp = leWP->text();
    extlist[name].icq = leICQ->text();
    extlist[name].tlen = leTlen->text();
    extlist[name].alt_gg = leSecondGG->text();
    extlist[name].memo = teMemo->text();
    extlist[name].photo_path = photo_path;
    extlist[name].photo_scaled = cbScaled->isChecked();
    if (old != extlist[name])
    {
        extlist[name].modified = true;
    }
    kdebugf2();
}

void frmExtInfo::updateInfoTab()
{
    QString info = infoTemplate;
#define REPLACE(tag, txt) info.replace("{T"tag"}",txt.isEmpty() ? "<!--" : "");\
                          info.replace("{/T"tag"}",txt.isEmpty() ? "-->" : "");\
                          info.replace("{"tag"}", txt)
    info.replace("{extinfo_path}",extinfo->extinfoPath());
    info.replace("{module_data_path}",extinfo->moduleDataPath());
    REPLACE("gg", (kaduData->found ? QString("%1").arg(kaduData->UIN) : QString("")));
    REPLACE("ggnick", currentSection);
    REPLACE("nickorggnick", (leNickname->text().length() ? leNickname->text() : currentSection));
    REPLACE("nick", leNickname->text());
    REPLACE("avatar", getPhotoPath());
    info.replace("{avatar_width}", (photo_path[0] == '/') ? "width" : "none");
    REPLACE("first_name", leFirstName->text());
    REPLACE("last_name", leLastName->text());
    REPLACE("street", leStreet->text());
    REPLACE("city", leCity->text());
    REPLACE("homeadress", QString(leStreet->text() + leCity->text()));
    REPLACE("birthday", leBirthDay->text());
    REPLACE("name_day", leNameDay->text());
    REPLACE("phone", lePhone->text());
    REPLACE("mobile", leMobile->text());
    REPLACE("interests", leInterests->text());
    REPLACE("email1", leEmail1->text());
    REPLACE("email2", leEmail2->text());
    REPLACE("www", leWWW->text());
    REPLACE("irc", leIRC->text());
    REPLACE("icq", leICQ->text());
    REPLACE("wp", leWP->text());
    REPLACE("tlen", leTlen->text());
    REPLACE("alt_gg", leSecondGG->text());
    REPLACE("memo", QString(teMemo->text()).replace("\n","<br>"));
    tbGeneral->setText(info);
}

void frmExtInfo::cbChangeSection( const QString &name )
{
    kdebugf();
    saveSection(currentSection);
    currentSection = name;
    kaduData->load(name);
    loadSection(name);
    kdebugf2();
}

void frmExtInfo::memoCharsUpdate()
{
    kdebugf();
    int chars = teMemo->text().length() + teMemo->lines() - 1;
    tlMemo->setText(chars > 1024 ? (QString::number(chars) + tr(" chars, Warning!!! This memo have over 1024 chars. It isn't compatible with RExInfo!")) : (QString::number(chars) + tr(" chars")));
    kdebugf2();
}


void frmExtInfo::testFormatBirthday()
{
    kdebugf();
    int day, month, year;
    sscanf(leBirthDay->text(),"%2d.%2d.%4d",&day, &month, &year);
    if (!QDate::isValid(year,month,day) && leBirthDay->text().length())
    {
        if (QMessageBox::critical (this,"ExtInfo",tr("Error: Bad format birthday date, pleas write in format: dd.mm.yyyy"),QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Ok)
            leBirthDay->setFocus();
        else
            leBirthDay->setText("");
    }
    kdebugf2();
}

void frmExtInfo::testFormatNameday()
{
    kdebugf();
    int day, month;
    sscanf(leNameDay->text(),"%2d.%2d",&day, &month);
    if (!QDate::isValid(2004,month,day) && leNameDay->text().length()) //2004 bo rok przestêpny, by prawid³owe by³o 29.02
    {
        if (QMessageBox::critical (this,"ExtInfo",tr("Error: Bad format nameday date, pleas write in format: dd.mm"),QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Ok)
            leNameDay->setFocus();
        else
            leNameDay->setText("");
    }
    kdebugf2();
}


void frmExtInfo::showAbout()
{
    QMessageBox::about (this, "ExtInfo", QString("ExtInfo v.") + QString(EXTINFO_VERSION) + QString(" GPL\nCopyright (C) by ") + tr("Dzwiedziu")  + QString(" 2005\nnkg@poczta.onet.pl\nhttp://nkg.republika.pl/\n\n") + tr("This module is compatible with RExInfo plugin from PowerGG.") + "\nCopyright (C) by RaV\nhttp://www.rexinfo.prv.pl/");
}


void frmExtInfo::clickedOk()
{
    kdebugf();
    clicked = QMessageBox::Ok;
    saveSection(currentSection);
    emit(acceptChanges(extlist));
    close();
    kdebugf2();
}


void frmExtInfo::show(const ExtList &src, const QString & sectionName )
{
    kdebugf();
    if (isHidden())
    {
        cbSection->clear();
        currentSection = "";
        extlist = src;
        for (ExtList::iterator i = extlist.begin(); i != extlist.end(); i++)
        {
            cbSection->insertItem(i.key());
            (*i).modified = false;
        }
    }
    else
        saveSection(currentSection);
    setCurrentSection(sectionName);
    if (isHidden())
        QDialog::show();
    kdebugf2();
}

void frmExtInfo::renameSection(const QString& oldName, const QString& newName)
{
    kdebugf();
    if (isHidden())
    {
        kdebugf2();
        return;
    }
    extlist.renameItem(oldName,newName);
    if (currentSection == oldName)
    {
        currentSection = newName;
        saveSection(newName);
    }
    cbSection->removeItem(getCheckBoxItem(oldName));
    cbSection->insertItem(newName);
    if (currentSection == newName)
        cbSection->setCurrentItem(cbSection->count() - 1);
    kdebugf2();
}

int frmExtInfo::getCheckBoxItem(const QString& name)
{
    kdebugf();
    for (int i = 0; i < cbSection->count(); i++)
    {
        if (cbSection->text(i) == name)
        {
            kdebugf2();
            return i;
        }
    }
    kdebugf2();
    return -1;
}

void frmExtInfo::clickedRemoveSection()
{
    kdebugf();
    extlist.remove(currentSection);
    currentSection = "";
    cbSection->removeItem(cbSection->currentItem());
    if (extlist.count())
        cbChangeSection(cbSection->text(cbSection->currentItem()));
    kdebugf2();
}

QString frmExtInfo::getPhotoPath()
{
    return getPhotoPath(photo_path);
}

QString frmExtInfo::getPhotoPath(const QString &photopath)
{
    if (photopath.length() && (photopath[0] != '/'))
        return extinfo->extinfoPath(photopath);
    else
        return photopath;
}

void frmExtInfo::loadImage(const QString & image, bool drop)
{
    kdebugf();
    QString img = getPhotoPath(image);

    if (siPhoto->setImage(img))
    {
        photo_path = image;
        tlPathImage->setText(tr ("Path image: ") + photo_path);
    }
    else
    {
        if (drop == false)
        {
            siPhoto->clearImage();
            photo_path = "";
            tlPathImage->setText(tr("Path image: "));
        }
    }
    tbGeneral->reload();
    kdebugf2();
}

void frmExtInfo::onDragImageFile(const QString& filename)
{
    loadImage(filename, true);
}

void frmExtInfo::clickedRemoveImage()
{
    loadImage("");
}

void frmExtInfo::clickedLoadImage()
{
    kdebugf();
    QString image = QFileDialog::getOpenFileName(photo_path,tr("All Pixmaps") +
        QString(" (*.bmp;*.jpg;*.jpeg;*.png;*.pgm;*.pbm;*.ppm;*.xbm;*.xpm);;") +
        tr("All files") + QString(" (*)"));
    if (image.length())
        loadImage(image);
    kdebugf2();
}

QString encodeName(const QString &name)
{
    QString out;
    for (uint i = 0; i < name.length(); i++)
    {
        if (((name[i] >= 'a') && (name[i] <= 'z')) || ((name[i] >= 'A') && (name[i] <= 'Z')) || ((name[i] >= '0') && (name[i] <= '9')))
            out += name[i];
        else
            out += QString("").sprintf("_%02x",int(uchar((name.local8Bit())[i])));
    }
    return out;
}

void frmExtInfo::clickedAvatar()
{
    QString n = encodeName(currentSection) + QString(".jpg");
    frmAvatar frmavatar(extinfo->extinfoPath(n), getPhotoPath(), this);
    if (frmavatar.exec() == QDialog::Accepted)
    {
        cbScaled->setChecked(false);
        loadImage(n);
    }
}

void frmExtInfo::checkScaledImage(int scaled)
{
    kdebugf();
    scaledPhoto();
    kdebugf2();
}

void frmExtInfo::scaledPhoto()
{
    kdebugf();
    siPhoto->setScaled(cbScaled->isChecked());
    kdebugf2();
}

void frmExtInfo::getDataFromKadu()
{
    kdebugf();
    if (!kaduData->found)
    {
        kdebugf2();
        return;
    }
    leNickname->setText(kaduData->nickName);
    leFirstName->setText(kaduData->firstName);
    leLastName->setText(kaduData->lastName);
    leMobile->setText(kaduData->mobile);
    leEmail1->setText(kaduData->email);
    kdebugf2();
}

void frmExtInfo::setCurrentSection(const QString& sectionName)
{
    kdebugf();
    if (extlist.contains(sectionName))
    {
        cbSection->setCurrentItem(getCheckBoxItem(sectionName));
        cbChangeSection(sectionName);
    }
    else
    {
        extlist[sectionName];
        cbSection->insertItem(sectionName);
        cbSection->setCurrentItem(cbSection->count() - 1);
        cbChangeSection(sectionName);
        getDataFromKadu();
    }
    kdebugf2();
}

void frmExtInfo::tabCurrentChanged ( QWidget * tab)
{
    kdebugf();
    if (tab == tabPhoto)
        scaledPhoto();
    else if (tab == tabGeneral)
        updateInfoTab();
    kdebugf2();
}
