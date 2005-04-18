#include "frmextinfo.h"
#include "debug.h"

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

frmExtInfo::frmExtInfo( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    kdebugf();
    if ( !name )
    setName( "frmExtInfo" );
    frmExtInfoLayout = new QGridLayout( this, 1, 1, 11, 6, "frmExtInfoLayout");

    tabWidget = new QTabWidget( this, "tabWidget" );
    tabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, tabWidget->sizePolicy().hasHeightForWidth() ) );

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

    cbSection = new QComboBox( FALSE, bgInfo, "cbSection" );

    lGeneral->addWidget( cbSection, 0, 1 );

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

    tabPhoto = new QWidget( tabWidget, "tabPhoto" );
    tabPhotoLayout = new QGridLayout( tabPhoto, 1, 1, 11, 6, "tabPhotoLayout");

    tlPathImage = new QLabel( tabPhoto, "tlPathImage" );
    tlPathImage->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, tlPathImage->sizePolicy().hasHeightForWidth() ) );

    tabPhotoLayout->addMultiCellWidget( tlPathImage, 1, 1, 0, 1 );

    bgPhoto = new QButtonGroup( tabPhoto, "bgPhoto" );
    bgPhoto->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)5, 0, 0, bgPhoto->sizePolicy().hasHeightForWidth() ) );
    bgPhoto->setFrameShape( QButtonGroup::NoFrame );
    bgPhoto->setColumnLayout(0, Qt::Vertical );
    bgPhoto->layout()->setSpacing( 6 );
    bgPhoto->layout()->setMargin( 0 );
    bgPhotoLayout = new QGridLayout( bgPhoto->layout() );
    bgPhotoLayout->setAlignment( Qt::AlignTop );

    lPhoto = new QVBoxLayout( 0, 0, 6, "lPhoto");

    pbLoadImage = new QPushButton( bgPhoto, "pbLoadImage" );
    lPhoto->addWidget( pbLoadImage );

    pbRemoveImage = new QPushButton( bgPhoto, "pbRemoveImage" );
    lPhoto->addWidget( pbRemoveImage );

    cbScaled = new QCheckBox( bgPhoto, "cbScaled" );
    cbScaled->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, cbScaled->sizePolicy().hasHeightForWidth() ) );
    lPhoto->addWidget( cbScaled );

    bgPhotoLayout->addLayout( lPhoto, 0, 0 );

    tabPhotoLayout->addWidget( bgPhoto, 0, 1 );

    fPhoto = new QScrollView( tabPhoto, "fPhoto" );
    fPhoto->setFrameShape( QScrollView::WinPanel );
    fPhoto->setFrameShadow( QScrollView::Sunken );

    wPhoto = new QWidget(fPhoto->viewport());
    fPhoto->addChild(wPhoto);

    plPhoto = new QLabel(wPhoto, "plPhoto" );

    tabPhotoLayout->addWidget( fPhoto, 0, 0 );
    tabWidget->insertTab( tabPhoto, QString("") );

    frmExtInfoLayout->addWidget( tabWidget, 0, 0 );

    lBottomPanel = new QHBoxLayout( 0, 0, 6, "lBottomPanel");
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
    connect( pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
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
    connect( pbGetData,SIGNAL( clicked() ),this, SLOT( getDataFromKadu()));
    connect( tabWidget,(SIGNAL(currentChanged ( QWidget * ))),this, SLOT(tabCurrentChanged ( QWidget * )));

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
    tlSection->setBuddy( cbSection );
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
    kdebugf2();
}

void frmExtInfo::languageChange()
{
    kdebugf();
    setCaption( tr( "Extension info" ) );
    bgInfo->setTitle( QString::null );
    QToolTip::add( leBirthDay, tr( "Format: dd.mm.yyyy" ) );
    QToolTip::add( lePhone, tr( "Phone number" ) );
    tlPhone->setText( tr( "Phone:" ) );
    tlSection->setText( tr( "Section:" ) );
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
    tabWidget->changeTab( tabInfo, tr( "Info" ) );
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
    cbScaled->setText( tr( "Scaled" ) );
    tabWidget->changeTab( tabPhoto, tr( "Photo" ) );
    pbOk->setText( tr( "OK" ) );
    pbOk->setAccel( QKeySequence( QString::null ) );
    pbCancel->setText( tr( "Cancel" ) );
    pbAbout->setText( tr( "About" ) );
    kdebugf2();
}

void frmExtInfo::loadSection()
{
    kdebugf();
    leNickname->setText(extlist[currentSection].nickname);
    leLastName->setText(extlist[currentSection].last_name);
    leFirstName->setText(extlist[currentSection].first_name);
    leBirthDay->setText(extlist[currentSection].birthday);
    leNameDay->setText(extlist[currentSection].name_day);
    lePhone->setText(extlist[currentSection].phone);
    leMobile->setText(extlist[currentSection].mobile);
    leCity->setText(extlist[currentSection].city);
    leStreet->setText(extlist[currentSection].street);
    leInterests->setText(extlist[currentSection].interests);
    leEmail1->setText(extlist[currentSection].email[0]);
    leEmail2->setText(extlist[currentSection].email[1]);
    leWWW->setText(extlist[currentSection].www);
    leIRC->setText(extlist[currentSection].irc);
    leWP->setText(extlist[currentSection].wp);
    leICQ->setText(extlist[currentSection].icq);
    leTlen->setText(extlist[currentSection].tlen);
    leSecondGG->setText(extlist[currentSection].alt_gg);
    teMemo->setText(extlist[currentSection].memo);
    cbScaled->setChecked(extlist[currentSection].photo_scaled);
    loadImage(extlist[currentSection].photo_path);
    kdebugf2();
}

void frmExtInfo::cbChangeSection( const QString &name )
{
    kdebugf();
    updateSection();
    currentSection = name;
    loadSection();
    kdebugf2();
}

void frmExtInfo::updateSection()
{
    kdebugf();
    if (currentSection == "")
        return;
    extlist[currentSection].nickname = leNickname->text();
    extlist[currentSection].last_name = leLastName->text();
    extlist[currentSection].first_name = leFirstName->text();
    extlist[currentSection].birthday = leBirthDay->text();
    extlist[currentSection].name_day = leNameDay->text();
    extlist[currentSection].phone = lePhone->text();
    extlist[currentSection].mobile = leMobile->text();
    extlist[currentSection].city = leCity->text();
    extlist[currentSection].street = leStreet->text();
    extlist[currentSection].interests = leInterests->text();
    extlist[currentSection].email[0] = leEmail1->text();
    extlist[currentSection].email[1] = leEmail2->text();
    extlist[currentSection].www = leWWW->text();
    extlist[currentSection].irc = leIRC->text();
    extlist[currentSection].wp = leWP->text();
    extlist[currentSection].icq = leICQ->text();
    extlist[currentSection].tlen = leTlen->text();
    extlist[currentSection].alt_gg = leSecondGG->text();
    extlist[currentSection].memo = teMemo->text();
    extlist[currentSection].photo_path = photo_path;
    extlist[currentSection].photo_scaled = cbScaled->isChecked();
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
    QMessageBox::about (this, "ExtInfo", QString("ExtInfo v.") + QString(EXTINFO_VERSION) + QString(" GPL\nCopyright (C) by NKG 2004\nnkg@poczta.onet.pl\nhttp://nkg.republika.pl/\n\n") + tr("This module is compatible with RExInfo plugin from PowerGG.") + "\nCopyright (C) by RaV\nhttp://www.rexinfo.prv.pl/");
}


void frmExtInfo::clickedOk()
{
    kdebugf();
    updateSection();
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
            cbSection->insertItem(i.key());
    }
    else
        updateSection();
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
        updateSection();
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


void frmExtInfo::loadImage( const QString & image )
{
    kdebugf();
    photo_path = image;
    if (image.length() && QFile::exists(image))
    {
        wPhoto->show();
        plPhoto->setPixmap(QPixmap(image));
        scaledPhoto();
    }
    else
    {
        wPhoto->hide();
        plPhoto->clear();
    }
    tlPathImage->setText(tr ("Path image: ") + image);
    kdebugf2();
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


void frmExtInfo::checkScaledImage(int scaled)
{
    kdebugf();
    plPhoto->setScaledContents(QButton::On == scaled);
    scaledPhoto();
    kdebugf2();
}

void frmExtInfo::resizeEvent ( QResizeEvent * resize)
{
    kdebugf();
    QDialog::resizeEvent(resize);
    scaledPhoto();
    kdebugf2();
}

void frmExtInfo::scaledPhoto()
{
    kdebugf();
    if (photo_path.length() == 0)
        return;
    plPhoto->setScaledContents(cbScaled->isChecked());
    if (plPhoto->hasScaledContents())
    {
        fPhoto->setContentsPos(0,0);
        fPhoto->setVScrollBarMode(QScrollView::AlwaysOff);
        fPhoto->setHScrollBarMode(QScrollView::AlwaysOff);
        float scaleW = float(fPhoto->visibleWidth()) / float(plPhoto->pixmap()->width());
        float scaleH = float(fPhoto->visibleHeight()) / float(plPhoto->pixmap()->height());
        float scale = scaleW > scaleH ? scaleH : scaleW;
        QRect rect(0,0,int(float(plPhoto->pixmap()->width()) * scale), int(float(plPhoto->pixmap()->height()) * scale));
        wPhoto->setGeometry(rect);
        plPhoto->setGeometry(rect);
    }
    else
    {
        fPhoto->setVScrollBarMode(QScrollView::Auto);
        fPhoto->setHScrollBarMode(QScrollView::Auto);
        QRect rect(0,0,plPhoto->pixmap()->width(),plPhoto->pixmap()->height());
        wPhoto->setGeometry(rect);
        plPhoto->setGeometry(rect);
    }
    kdebugf2();
}

void frmExtInfo::getDataFromKadu()
{
    kdebugf();
    if (!userlist.containsAltNick(currentSection))
    {
        kdebugf2();
        return;
    }
    UserListElement user = userlist.byAltNick(currentSection);
    leNickname->setText(user.nickName());
    leFirstName->setText(user.firstName());
    leLastName->setText(user.lastName());
    leMobile->setText(user.mobile());
    leEmail1->setText(user.email());
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
    kdebugf2();
}
