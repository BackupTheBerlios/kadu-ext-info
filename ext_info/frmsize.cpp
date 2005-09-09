#include "frmsize.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

frmSize::frmSize(int w, int h, QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "frmSize" );
    frmSizeLayout = new QVBoxLayout( this, 11, 6, "frmSizeLayout");

    groupSize = new QButtonGroup( this, "groupSize" );
    groupSize->setColumnLayout(0, Qt::Vertical );
    groupSize->layout()->setSpacing( 6 );
    groupSize->layout()->setMargin( 11 );
    groupSizeLayout = new QVBoxLayout( groupSize->layout() );
    groupSizeLayout->setAlignment( Qt::AlignTop );

    rbgG = new QRadioButton( groupSize, "rbgG" );
    groupSizeLayout->addWidget( rbgG );

    rbSkype = new QRadioButton( groupSize, "rbSkype" );
    groupSizeLayout->addWidget( rbSkype );

    rbEpuls = new QRadioButton( groupSize, "rbEpuls" );
    groupSizeLayout->addWidget( rbEpuls );

    rbUser = new QRadioButton( groupSize, "rbUser" );
    groupSizeLayout->addWidget( rbUser );

    layoutUser = new QHBoxLayout( 0, 0, 6, "layoutUser");

    width = new QSpinBox(1, 256*256, 1, groupSize, "width" );
    width->setEnabled(false);
    width->setValue(w);
    layoutUser->addWidget( width );

    separator = new QLabel( groupSize, "separator" );
    layoutUser->addWidget( separator );

    height = new QSpinBox(1, 256*256, 1, groupSize, "height" );
    height->setEnabled(false);
    height->setValue(h);
    layoutUser->addWidget( height );
    spacerSize = new QSpacerItem( 21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layoutUser->addItem( spacerSize );
    groupSizeLayout->addLayout( layoutUser );
    frmSizeLayout->addWidget( groupSize );

    if ((w == 48) && (h == 48))
        groupSize->setButton(0);
    else if ((w == 100) && (h == 100))
        groupSize->setButton(1);
    else if ((w == 100) && (h == 140))
        groupSize->setButton(2);
    else
    {
        groupSize->setButton(3);
        width->setValue(w);
        width->setEnabled(true);
        height->setValue(h);
        height->setEnabled(true);
    }

    layoutButtons = new QHBoxLayout( 0, 0, 6, "layoutButtons");
    spacerButtons = new QSpacerItem( 51, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layoutButtons->addItem( spacerButtons );

    pbOk = new QPushButton( this, "pbOk" );
    pbOk->setDefault( TRUE );
    layoutButtons->addWidget( pbOk );

    pbCancel = new QPushButton( this, "pbCancel" );
    layoutButtons->addWidget( pbCancel );
    frmSizeLayout->addLayout( layoutButtons );
    languageChange();
    resize( QSize().expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    connect(groupSize, SIGNAL(clicked(int)), this, SLOT(onButtonGroupClicked(int)));
    connect(pbOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

frmSize::~frmSize()
{
}

void frmSize::languageChange()
{
    setCaption( tr( "Avatar Size" ) );
    groupSize->setTitle( tr( "Avatar Size" ) );
    rbgG->setText( tr( "48x48 (GaduGadu 7)" ) );
    rbSkype->setText( tr( "100x100 (Skype)" ) );
    rbEpuls->setText( tr( "100x140 (epuls.pl)" ) );
    rbUser->setText( tr( "User" ) );
    separator->setText( tr( "x" ) );
    pbOk->setText( tr( "OK" ) );
    pbCancel->setText( tr( "Cancel" ) );
}

int frmSize::getWidth()
{
    int w,h;
    getSize(w,h);
    return w;
}

int frmSize::getHeight()
{
    int w,h;
    getSize(w,h);
    return h;
}

void frmSize::getSize(int &w, int &h)
{
    switch(groupSize->selectedId())
    {
        case 0:
            w = h = 48;
            break;
        case 1:
            w = h = 100;
            break;
        case 2:
            w = 100;
            h = 140;
            break;
        default:
            w = width->value();
            h = height->value();
    }
}

void frmSize::onButtonGroupClicked(int id)
{
    width->setEnabled(id == 3);
    height->setEnabled(id == 3);
}
