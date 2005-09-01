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
    Micha� (D�wiedziu) Nied�wiecki
    nkg@poczta.onet.pl
    http://kadu-ext-info.berlios.de/

    ChangeLog:

        [v1.4.2 ]
            * Dostosowano do zmian w Kadu 0.5.0-svn, jednocze�nie zachowano
              zgodno�� z Kadu 0.4.x

        [v1.4.1 ]
            * Dostosowano do zmian w Kadu 0.5.0, jednocze�nie zachowano zgodno��
              z Kadu 0.4.x

        [v1.4 ]
            + Je�li w oknie rozszerzonych informacji zostan� dokonane zmiany
              i je�li u�ytkownik kliknie na "Anuluj" lub spr�buje zamkn�� okno
              klikaj�c na "X" zostanie zapytany, czy zachowa� zmiany.zmodyfikowane
            * Spolszczone nazwy zak�adek

        [v1.3.1 ]
            * Poprawiony b��d w t�umaczeniu (dzi�ki tomee)
            * Poprawiono b��d, kt�ry wyst�powa� gdy skasowano plik ze zdj�ciem
            * Je�li osoba ma jutro imieniny (odpowiednio urodziny) to
              wy�wietlana jest informacja, �e "... ma jutro imieniny", a nie
              "... ma imieniny za 1 dni"

        [v1.3 ]
            * Dostosowano do zmian w aktualnej wersji Kadu CVS

        [v1.2 ]
            * Zmieniono nazw� na ExtInfo (ext_info)
            * Naniesiono kilka drobnych poprawek

        [v1.1 ]
            + Ikona w menu i w oknie konfiguracji
            + Import i eksport danych do pliku

        [v1.0 ]
            * Nikt nie zg�asza� �adnych problem�w wi�c wydaj� finaln� wersj� :)

        [v1.0 rc1]
            * Naprawiony b��d przy skalowaniu zdj�cia
            * Przed za�adowaniem obrazka sprawdza czy plik istnieje, �eby nie
              dawa� komunikatu o b��dzie na konsole

        [v1.0 beta2]
            * Naprawiony powa�ny b��d przy dodawaniu i usuwaniu kontakt�w
              (dzi�ki Smoczyca)

        [v1.0 beta1]
            * Pierwsza publiczna wersja :)
*/

#include "config_dialog.h"
#include "ext_info.h"
#include "message_box.h"
#include "debug.h"
#include "userbox.h"

#ifdef EXTERNAL
#include <modules/hint_manager.h>
#else
#include "../hints/hint_manager.h"
#endif
#define MODULE_EXTINFO_VERSION 1.4.2

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
    :frmextinfo(NULL),extlist(ggPath("RExInfo.dat")),menuBirthday(false),menuNameDay(false)
{
    kdebugf();
    RegisterInConfigDialog();
    UserBox::userboxmenu->addItem(dataPath("kadu/modules/data/ext_info/ext_info_menu.png"), tr("Display extended information"), this, SLOT(showExtInfo()));

    QObject::connect(UserBox::userboxmenu, SIGNAL(popup()), this, SLOT(onPopupMenuCreate()));
#if defined(KADU_0_4_x)
    connect(&userlist, SIGNAL(userDataChanged(const UserListElement * const, const UserListElement * const,bool)),
        this, SLOT(userDataChanged(const UserListElement * const, const UserListElement * const,bool)));
#elif defined(KADU_0_5_0)
    connect(userlist, SIGNAL(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)),
        this, SLOT(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)));
#endif
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
#if defined(KADU_0_4_x)
    disconnect(&userlist, SIGNAL(userDataChanged(const UserListElement * const, const UserListElement * const,bool)),
        this, SLOT(userDataChanged(const UserListElement * const, const UserListElement * const,bool)));
#elif defined(KADU_0_5_0)
    disconnect(userlist, SIGNAL(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)),
        this, SLOT(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)));
#endif

    disconnect(&timer, SIGNAL(timeout()), this, SLOT(checkAnniversary()));
    closeWindow();
    UnregisterInConfigDialog();
    int menuitem = UserBox::userboxmenu->getItem(tr("Display extended information"));
    UserBox::userboxmenu->removeItem(menuitem);
    kdebugf2();
}

void ExtInfo::RegisterInConfigDialog()
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
    kdebugf2();
}

void ExtInfo::UnregisterInConfigDialog()
{
    kdebugf();
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
    kdebugf2();
}

bool ExtInfo::UpdateUser()
{
    kdebugf();
#if defined(KADU_0_4_x)
    UserList users;
    UserBox *activeUserBox = kadu->userbox()->getActiveUserBox();
#elif defined(KADU_0_5_0)
    UserBox *activeUserBox = kadu->userbox()->activeUserBox();
#endif
    if (activeUserBox == NULL)//to si� zdarza...
    {
        kdebugf2();
        return false;
    }
#if defined(KADU_0_4_x)
    users = activeUserBox->getSelectedUsers();
#elif defined(KADU_0_5_0)
    UserListElements users = activeUserBox->selectedUsers();
#endif
    if (users.count() == 1)
    {
        user = (*users.begin());
        kdebugf2();
        return true;
    }
    return false;
//#else
//#endif
    kdebugf2();
}

void ExtInfo::showExtInfo()
{
    kdebugf();
    if (UpdateUser())
        showExtInfo(user.altNick());
    kdebugf2();
}

void ExtInfo::showExtInfo(const QString& section)
{
    kdebugf();
    if (frmextinfo == NULL)
    {
        frmextinfo = new frmExtInfo();
        connect(frmextinfo,SIGNAL(acceptChanges( const ExtList&)),this,SLOT(acceptChanges( const ExtList&)));
        connect(frmextinfo,SIGNAL(closeWindow()),this,SLOT(closeWindow()));
    }
    frmextinfo->show(extlist,section);
    kdebugf2();
}

void ExtInfo::closeWindow()
{
    kdebugf();
    if (frmextinfo)
    {
        disconnect(frmextinfo,SIGNAL(closeWindow()),this,SLOT(closeWindow()));
        disconnect(frmextinfo,SIGNAL(acceptChanges( const ExtList&)),this,SLOT(acceptChanges( const ExtList&)));
        delete frmextinfo;
        frmextinfo = NULL;
    }
    kdebugf2();
}

void ExtInfo::onPopupMenuCreate()
{
    kdebugf();

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

    if (UpdateUser())
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
    else
    {
        UserBox::userboxmenu->setItemEnabled(UserBox::userboxmenu->getItem(tr("Display extended information")), false);
    }
    kdebugf2();
}

void ExtInfo::userDataChanged(const UserListElement* const oldData, const UserListElement* const newData,bool massivielty)
{
    kdebugf();
#ifdef KADU_0_4_x
    if (!oldData || !newData)
        return;                 //To powodowa�o b��d przy usuwaniu i dodawaniu kontakt�w
    if (oldData->altNick() != newData->altNick())
    {

        extlist.renameItem(oldData->altNick(),newData->altNick());
        if (frmextinfo)
        {
            frmextinfo->renameSection(oldData->altNick(),newData->altNick());
        }
    }
#endif
    kdebugf2();
}

void ExtInfo::userDataChanged(UserListElement elem, QString name, QVariant oldValue,QVariant currentValue, bool massively, bool last)
{
    kdebugf();
#ifdef KADU_0_5_0
    if (name != QString("AltNick"))
        return;                 //To powodowa�o b��d przy usuwaniu i dodawaniu kontakt�w
    extlist.renameItem(oldValue.toString(), currentValue.toString());
    if (frmextinfo)
    {
        frmextinfo->renameSection(oldValue.toString(), currentValue.toString());
    }
#endif
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
        {
#if defined(KADU_0_4_x)
            hint_manager->message("ExtInfo",
                formatNameDayInfo(i.key(),(*i).daysToNameDay()),
                NULL,
                userlist.containsAltNick(i.key()) ? &userlist.byAltNick(i.key()) : NULL);
#elif defined(KADU_0_5_0)
            UserListElement buff;
            hint_manager->message("ExtInfo",
                formatNameDayInfo(i.key(),(*i).daysToNameDay()),
                NULL,
                userlist->containsAltNick(i.key()) ? (buff = userlist->byAltNick(i.key()),&buff) : NULL);
#endif
        }
    }
    if (config_file.readBoolEntry("ExtInfo","birthday",true))
    {
        ExtList birthday = extlist.getCommingBirthday(config_file.readNumEntry("ExtInfo","remind",1));
        for (ExtList::iterator i = birthday.begin(); i != birthday.end(); i++)
        {
#if defined(KADU_0_4_x)
            hint_manager->message("ExtInfo",
                formatBirthdayInfo(i.key(),(*i).daysToBirthday()),
                NULL,
                userlist.containsAltNick(i.key()) ? &userlist.byAltNick(i.key()) : NULL);
#elif defined(KADU_0_5_0)
            UserListElement buff;
            hint_manager->message("ExtInfo",
                formatBirthdayInfo(i.key(),(*i).daysToBirthday()),
                NULL,
                userlist->containsAltNick(i.key()) ? (buff = userlist->byAltNick(i.key()),&buff) : NULL);
#endif
        }
    }
    kdebugf2();
}

QString ExtInfo::formatNameDayInfo(const QString& name, int days)
{
    QString str;
    if (days > 1)
    {
        str = name + tr(" has name-day in ") + QString::number(days) + tr(" days");
    }
    else if (days == 1)
    {
        str = name + tr(" has name-day tomorrow");
    }
    else
    {
        str = name + tr(" has name-day");
    }
    return str;
}

QString ExtInfo::formatBirthdayInfo(const QString& name, int days)
{
    QString str;
    if (days > 1)
    {
        str = name + tr(" has birthday in ") + QString::number(days) + tr(" days");
    }
    else if (days == 1)
    {
        str = name + tr(" has birthday tomorrow");
    }
    else
    {
        str = name + tr(" has birthday");
    }
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
    if (frmextinfo)
    {
        frmextinfo->setFocus();
        MessageBox::wrn(tr("First you must close ext_info window"));
        kdebugf2();
        return;
    }
    for(;;)
    {
        QString filename = QFileDialog::getSaveFileName(ggPath("RExInfo.dat"),
            tr("RExInfo/ext_info 1.x files") + QString(" (RExInfo.dat; rexinfo.dat);;") +
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
    if (frmextinfo)
    {
        frmextinfo->setFocus();
        MessageBox::wrn(tr("First you must close ext_info window"));
        kdebugf2();
        return;
    }
    QString filename = QFileDialog::getOpenFileName(ggPath("RExInfo.dat"),
        tr("RExInfo/ext_info 1.x files") + QString(" (RExInfo.dat; rexinfo.dat);;") +
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
