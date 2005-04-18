/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
    Author:
    Micha³ (NKG) Nied¼wiecki
    nkg@poczta.onet.pl
    http://nkg.republika.pl/

    ChangeLog:

    [v1.3 ]
	* Dostosowano do zmian w aktualnej wersji Kadu CVS

    [v1.2 ]
        * Zmieniono nazwê na ExtInfo (ext_info)
        * Naniesiono kilka drobnych poprawek

        [v1.1 ]
            * Ikona w menu i w oknie konfiguracji
            * Import i eksport danych do pliku

        [v1.0 ]
            * Nikt nie zg³asza³ ¿adnych problemów wiêc wydajê finaln± wersjê :)

        [v1.0 rc1]
            * Naprawiony b³±d przy skalowaniu zdjêcia
            * Przed za³adowaniem obrazka sprawdza czy plik istnieje, ¿eby nie dawa³ komunikatu o b³êdzie na konsole

        [v1.0 beta2]
            * Naprawiony powa¿ny b³±d przy dodawaniu i usuwaniu kontaktów
              (dziêki Smoczyca)

        [v1.0 beta1]
            * Pierwsza publiczna wersja :)
*/

#include "config_dialog.h"
#include "ext_info.h"
#include "message_box.h"
#include "debug.h"

#ifdef EXTERNAL
#include <modules/hint_manager.h>
#else
#include "../hints/hint_manager.h"
#endif
#define MODULE_EXTINFO_VERSION 1.2

ExtInfo* extinfo;

extern "C" int ext_info_init()
{
    extinfo = new ExtInfo;
    return 0;
}

extern "C" void ext_info_close()
{
    delete extinfo;
}

ExtInfo::ExtInfo()
    :extlist(ggPath("RExInfo.dat")),menuBirthday(false),menuNameDay(false)
{
    kdebugf();
    ConfigDialog::addTab("ExtInfo",dataPath("kadu/modules/data/ext_info/ext_info_tab.png"));
    ConfigDialog::addVGroupBox("ExtInfo", "ExtInfo", QT_TRANSLATE_NOOP("@default", "Remind"));
    ConfigDialog::addCheckBox("ExtInfo", "Remind", QT_TRANSLATE_NOOP("@default", "Enable to remind of name day"), "name_day", TRUE);
    ConfigDialog::addCheckBox("ExtInfo", "Remind", QT_TRANSLATE_NOOP("@default", "Enable to remind of birthday"), "birthday", TRUE);
    ConfigDialog::addSpinBox("ExtInfo", "Remind", QT_TRANSLATE_NOOP("@default", "Remind days before:"),"remind",0,100,1,1);
    ConfigDialog::addSpinBox("ExtInfo", "Remind", QT_TRANSLATE_NOOP("@default", "Reminds frequency (minutes):"),"remind_frequency",0,1440,1,10);
    ConfigDialog::addHBox("ExtInfo","ExtInfo","ieButtons");//"Import/export buttons"
    ConfigDialog::addPushButton("ExtInfo","ieButtons",QT_TRANSLATE_NOOP("@default","Import"));
    ConfigDialog::addPushButton("ExtInfo","ieButtons",QT_TRANSLATE_NOOP("@default","Export"));

    ConfigDialog::connectSlot("ExtInfo","Import",SIGNAL(clicked()),this,SLOT(onImport()));
    ConfigDialog::connectSlot("ExtInfo","Export",SIGNAL(clicked()),this,SLOT(onExport()));

    UserBox::userboxmenu->addItem(dataPath("kadu/modules/data/ext_info/ext_info_menu.png"), tr("Display extended information"), this, SLOT(showExtInfo()));

    QObject::connect(UserBox::userboxmenu, SIGNAL(popup()), this, SLOT(onPopupMenuCreate()));
    connect(&userlist, SIGNAL(userDataChanged(const UserListElement * const, const UserListElement * const,bool)),
        this, SLOT(userDataChanged(const UserListElement * const, const UserListElement * const,bool)));
    connect(&frmextinfo,SIGNAL(acceptChanges( const ExtList&)),this,SLOT(acceptChanges( const ExtList&)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkAnniversary()));
    ConfigDialog::registerSlotOnApply(this, SLOT(onApplyConfigDialog()));
    restartTimer();
    kdebugf2();
}

ExtInfo::~ExtInfo()
{
    kdebugf();
    ConfigDialog::unregisterSlotOnApply(this, SLOT(onApplyConfigDialog()));
    QObject::disconnect(UserBox::userboxmenu, SIGNAL(popup()), this, SLOT(onPopupMenuCreate()));
    disconnect(&userlist, SIGNAL(userDataChanged(const UserListElement * const, const UserListElement * const,bool)),
        this, SLOT(userDataChanged(const UserListElement * const, const UserListElement * const,bool)));
    disconnect(&frmextinfo,SIGNAL(acceptChanges( const ExtList&)),this,SLOT(acceptChanges( const ExtList&)));
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(checkAnniversary()));

    ConfigDialog::disconnectSlot("ExtInfo","Import",SIGNAL(clicked()),this,SLOT(onImport()));
    ConfigDialog::disconnectSlot("ExtInfo","Export",SIGNAL(clicked()),this,SLOT(onExport()));

    ConfigDialog::removeControl("ExtInfo","Export");
    ConfigDialog::removeControl("ExtInfo","Import");
    ConfigDialog::removeControl("ExtInfo","ieButtons");
    ConfigDialog::removeControl("ExtInfo","Reminds frequency (minutes):");
    ConfigDialog::removeControl("ExtInfo","Remind days before:");
    ConfigDialog::removeControl("ExtInfo","Enable to remind of name day");
    ConfigDialog::removeControl("ExtInfo","Enable to remind of birthday");
    ConfigDialog::removeControl("ExtInfo","Remind");
    ConfigDialog::removeTab("ExtInfo");
    int menuitem = UserBox::userboxmenu->getItem(tr("Display extended information"));
    UserBox::userboxmenu->removeItem(menuitem);
    kdebugf2();
}

void ExtInfo::showExtInfo()
{
    kdebugf();
    UserList users;
    UserBox *activeUserBox=kadu->userbox()->getActiveUserBox();
    if (activeUserBox==NULL)
    {
        kdebugf2();
        return;
    }
    users = activeUserBox->getSelectedUsers();
    if (users.count() != 1)
    {
        kdebugf2();
        return;
    }
    frmextinfo.show(extlist,(*users.begin()).altNick());
    kdebugf2();
}

void ExtInfo::onPopupMenuCreate()
{
    kdebugf();
    UserList users;
    UserBox *activeUserBox=kadu->userbox()->getActiveUserBox();
    if (activeUserBox==NULL)//to siê zdarza...
    {
        kdebugf2();
        return;
    }
    users = activeUserBox->getSelectedUsers();
    user = (*users.begin());

    if (menuBirthday)
    {
        UserBox::userboxmenu->removeItem(UserBox::userboxmenu->getItem(tr("I know about birthday :)")));
        menuBirthday = false;
    }
    if (menuNameDay)
    {
        UserBox::userboxmenu->removeItem(UserBox::userboxmenu->getItem(tr("I know about name day :)")));
        menuNameDay = false;
    }

    if (users.count() != 1)
        UserBox::userboxmenu->setItemEnabled(UserBox::userboxmenu->getItem(tr("Display extended information")), false);
    else
    {
        if (extlist.contains(user.altNick()))
        {
            int name_day = extlist[user.altNick()].daysToNameDay();
            int birthday = extlist[user.altNick()].daysToBirthday();
            if (name_day <= config_file.readNumEntry("ExtInfo","remind",1) && name_day >= 0 && config_file.readBoolEntry("ExtInfo","name_day",true))
            {
                UserBox::userboxmenu->addItem("knowNameDay", tr("I know about name day :)"), this, SLOT(knowNameDay()));
                menuNameDay = true;
            }
            if (birthday <= config_file.readNumEntry("ExtInfo","remind",1) && birthday >= 0 && config_file.readBoolEntry("ExtInfo","birthday",true))
            {
                UserBox::userboxmenu->addItem("knowBirthday", tr("I know about birthday :)"), this, SLOT(knowBirthday()));
                menuBirthday = true;
            }
        }
    }
    kdebugf2();
}

void ExtInfo::userDataChanged(const UserListElement* const oldData, const UserListElement* const newData,bool massivielty)
{
    kdebugf();
    if (!oldData || !newData)
        return;                 //To powodowa³o b³±d przy usuwaniu i dodawaniu kontaktów
    if (oldData->altNick() != newData->altNick())
    {

        extlist.renameItem(oldData->altNick(),newData->altNick());
        frmextinfo.renameSection(oldData->altNick(),newData->altNick());
    }
    kdebugf2();
}

void ExtInfo::acceptChanges( const ExtList & data )
{
    kdebugf();
    extlist = data;
    extlist.saveToFile();
    kdebugf2();
}

void ExtInfo::checkAnniversary()
{
    kdebugf();
    if (config_file.readBoolEntry("ExtInfo","name_day",true))
    {
        ExtList name_day = extlist.getCommingNameDay(config_file.readNumEntry("ExtInfo","remind",1));
        for (ExtList::iterator i = name_day.begin(); i != name_day.end(); i++)
            hint_manager->message("ExtInfo",
                formatNameDayInfo(i.key(),(*i).daysToNameDay()),
                NULL,
                userlist.containsAltNick(i.key()) ? &userlist.byAltNick(i.key()) : NULL);
    }
    if (config_file.readBoolEntry("ExtInfo","birthday",true))
    {
        ExtList birthday = extlist.getCommingBirthday(config_file.readNumEntry("ExtInfo","remind",1));
        for (ExtList::iterator i = birthday.begin(); i != birthday.end(); i++)
            hint_manager->message("ExtInfo",
                formatBirthdayInfo(i.key(),(*i).daysToBirthday()),
                NULL,
                userlist.containsAltNick(i.key()) ? &userlist.byAltNick(i.key()) : NULL);
    }
    kdebugf2();
}

QString ExtInfo::formatNameDayInfo(const QString& name, int days)
{
    QString str;
    if (days)
        str = name + tr(" have name day for ") + QString::number(days) + tr(" days");
    else
        str = name + tr(" have name day");
    return str;
}

QString ExtInfo::formatBirthdayInfo(const QString& name, int days)
{
    QString str;
    if (days)
        str = name + tr(" have birthday for ") + QString::number(days) + tr(" days");
    else
        str = name + tr(" have birthday");
    return str;
}

void ExtInfo::knowNameDay()
{
    kdebugf();
    extlist[user.altNick()].setKnowNameDay();
    kdebugf2();
}

void ExtInfo::knowBirthday()
{
    kdebugf();
    extlist[user.altNick()].setKnowBirthday();
    kdebugf2();
}

void ExtInfo::restartTimer()
{
    timer.start(config_file.readNumEntry("ExtInfo","remind_frequency",10) * 60000);
}

void ExtInfo::onApplyConfigDialog()
{
    restartTimer();
}

void ExtInfo::onExport()
{
    kdebugf();
    if (frmextinfo.isShown())
    {
        frmextinfo.setFocus();
        MessageBox::wrn(tr("First you must close ExtInfo window"));
        kdebugf2();
        return;
    }
    for(;;)
    {
        QString filename = QFileDialog::getSaveFileName(ggPath("RExInfo.dat"),
            tr("RExInfo/ExtInfo 1.x files") + QString(" (RExInfo.dat; rexinfo.dat);;") +
            tr("All files") + QString(" (*)"));
        if (filename.length())
        {
            if (QFile::exists(filename))
            {
                if (!MessageBox::ask(tr("This file are exists. Overwrite this file?")))
                    continue;
            }
            if (!extlist.saveToFile(filename))
                MessageBox::wrn(tr("Error write file"));
        }
        break;
    }
    kdebugf2();
}

void ExtInfo::onImport()
{
    kdebugf();
    if (frmextinfo.isShown())
    {
        frmextinfo.setFocus();
        MessageBox::wrn(tr("First you must close ExtInfo window"));
        kdebugf2();
        return;
    }
    QString filename = QFileDialog::getOpenFileName(ggPath("RExInfo.dat"),
        tr("RExInfo/ExtInfo 1.x files") + QString(" (RExInfo.dat; rexinfo.dat);;") +
        tr("All files") + QString(" (*)"));
    if (filename.length())
    {
        if (!extlist.loadFromFile(filename))
        {
            MessageBox::wrn(tr("Error read file"));
            kdebugf2();
            return;
        }
    }
    extlist.saveToFile();
    kdebugf2();
}
