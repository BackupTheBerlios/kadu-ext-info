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
    Micha³ (D¼wiedziu) Nied¼wiecki
    nkg@poczta.onet.pl
    http://kadu-ext-info.berlios.de/

    ChangeLog:

        [v1.5.1 ]
            * Podczas gdy w oknie konfiguracji zosta³y zaakceptowane zmiany,
	      pojawia³o siê kilka ikon ext_info w oknie rozmowy. B³±d ten zosta³
	      w³a¶nie naprawiony.

        [v1.5 ]
            + Dodana ikona ext_info w oknie rozmowy
            * Zerwanie kompatybilno¶ci z Kadu 0.4.x

        [v1.4.2 ]
            * Dostosowano do zmian w Kadu 0.5.0-svn, jednocze¶nie zachowano
              zgodno¶æ z Kadu 0.4.x

        [v1.4.1 ]
            * Dostosowano do zmian w Kadu 0.5.0, jednocze¶nie zachowano zgodno¶æ
              z Kadu 0.4.x

        [v1.4 ]
            + Je¶li w oknie rozszerzonych informacji zostan± dokonane zmiany
              i je¶li u¿ytkownik kliknie na "Anuluj" lub spróbuje zamkn±æ okno
              klikaj±c na "X" zostanie zapytany, czy zachowaæ zmiany.zmodyfikowane
            * Spolszczone nazwy zak³adek

        [v1.3.1 ]
            * Poprawiony b³±d w t³umaczeniu (dziêki tomee)
            * Poprawiono b³±d, który wystêpowa³ gdy skasowano plik ze zdjêciem
            * Je¶li osoba ma jutro imieniny (odpowiednio urodziny) to
              wy¶wietlana jest informacja, ¿e "... ma jutro imieniny", a nie
              "... ma imieniny za 1 dni"

        [v1.3 ]
            * Dostosowano do zmian w aktualnej wersji Kadu CVS

        [v1.2 ]
            * Zmieniono nazwê na ExtInfo (ext_info)
            * Naniesiono kilka drobnych poprawek

        [v1.1 ]
            + Ikona w menu i w oknie konfiguracji
            + Import i eksport danych do pliku

        [v1.0 ]
            * Nikt nie zg³asza³ ¿adnych problemów wiêc wydajê finaln± wersjê :)

        [v1.0 rc1]
            * Naprawiony b³±d przy skalowaniu zdjêcia
            * Przed za³adowaniem obrazka sprawdza czy plik istnieje, ¿eby nie
              dawa³ komunikatu o b³êdzie na konsole

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
#include "userbox.h"
#include "chat_manager.h"
#include "icons_manager.h"
#include "userinfo.h"

#ifdef EXTERNAL
#include <modules/hint_manager.h>
#else
#include "../hints/hint_manager.h"
#endif
#define MODULE_EXTINFO_VERSION 1.5.1

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
    :frmextinfo(NULL),extlist(ggPath("RExInfo.dat")),menuBirthday(false),menuNameDay(false),chatmenu(NULL)
{
    kdebugf();
    RegisterInConfigDialog();
    RegisterSignals();
    if(config_file.readBoolEntry("ExtInfo","button",true))
        CreateChatButton();
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkAnniversary()));
    restartTimer();
    kdebugf2();
}

ExtInfo::~ExtInfo()
{
    kdebugf();
    DestroyChatButton();
    UnregisterSignals();
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
    ConfigDialog::addCheckBox("ExtInfo", "ExtInfo", QT_TRANSLATE_NOOP("@default", "Show ext_info button in chat windows"), "button", TRUE);

    ConfigDialog::connectSlot("ExtInfo","Import",SIGNAL(clicked()),this,SLOT(onImport()));
    ConfigDialog::connectSlot("ExtInfo","Export",SIGNAL(clicked()),this,SLOT(onExport()));
    kdebugf2();
}

void ExtInfo::UnregisterInConfigDialog()
{
    kdebugf();
    ConfigDialog::disconnectSlot("ExtInfo","Import",SIGNAL(clicked()),this,SLOT(onImport()));
    ConfigDialog::disconnectSlot("ExtInfo","Export",SIGNAL(clicked()),this,SLOT(onExport()));

    ConfigDialog::removeControl("ExtInfo","Show ext_info button in chat windows");
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

void ExtInfo::RegisterSignals()
{
    kdebugf();
    UserBox::userboxmenu->addItem(dataPath("kadu/modules/data/ext_info/ext_info_menu.png"), tr("Display extended information"), this, SLOT(showExtInfo()));

    QObject::connect(UserBox::userboxmenu, SIGNAL(popup()), this, SLOT(onPopupMenuCreate()));
    connect(userlist, SIGNAL(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)),
        this, SLOT(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)));
    ConfigDialog::registerSlotOnApply(this, SLOT(onApplyConfigDialog()));
    kdebugf2();
}

void ExtInfo::UnregisterSignals()
{
    kdebugf();
    ConfigDialog::unregisterSlotOnApply(this, SLOT(onApplyConfigDialog()));
    QObject::disconnect(UserBox::userboxmenu, SIGNAL(popup()), this, SLOT(onPopupMenuCreate()));
    disconnect(userlist, SIGNAL(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)),
        this, SLOT(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)));
    kdebugf2();
}

void ExtInfo::CreateChatButton()
{
    kdebugf();
    chatmenu = new QPopupMenu;
    popups[0] = chatmenu->insertItem(icons_manager->loadIcon("EditUserInfo"),tr("Display standard information"),this,SLOT(showChatUserInfo()));
    popups[1] = chatmenu->insertItem(icons_manager->loadIcon(dataPath("kadu/modules/data/ext_info/ext_info_menu.png")),tr("Display extended information"),this,SLOT(showChatExtInfo()));

    connect(chat_manager, SIGNAL(chatCreated(const UserGroup*)), this, SLOT(chatCreated(const UserGroup*)));
    connect(chat_manager, SIGNAL(chatDestroying(const UserGroup*)), this, SLOT(chatDestroying(const UserGroup*)));
    ChatList::ConstIterator it;
    for ( it = chat_manager->chats().begin(); it != chat_manager->chats().end(); it++ )
        handleCreatedChat(*it);
    kdebugf2();
}

void ExtInfo::DestroyChatButton()
{
    if (chatmenu == NULL)
        return;
    kdebugf();
    disconnect(chat_manager, SIGNAL(chatCreated(const UserGroup*)), this, SLOT(chatCreated(const UserGroup*)));
    disconnect(chat_manager, SIGNAL(chatDestroying(const UserGroup*)), this, SLOT(chatDestroying(const UserGroup*)));
    ChatList::ConstIterator it;
    for ( it = chat_manager->chats().begin(); it != chat_manager->chats().end(); it++ )
        handleDestroyingChat(*it);
    delete chatmenu;
    chatmenu = NULL;
    kdebugf2();
}

void ExtInfo::chatCreated(const UserGroup* ul)
{
    kdebugf();
    Chat* chat = chat_manager->findChat(ul);
    handleCreatedChat(chat);
}

void ExtInfo::chatDestroying(const UserGroup* ul)
{
    kdebugf();
    Chat* chat = chat_manager->findChat(ul);
    handleDestroyingChat(chat);
}

void ExtInfo::handleCreatedChat(Chat* chat)
{
    kdebugf();
    if (chat->users()->count() != 1)
        return;
    QPushButton* chatbutton = new QPushButton(chat->buttontray);
    chatbutton->setPixmap(icons_manager->loadIcon("PersonalInfo"));
    chatbutton->setPopup(chatmenu);
    chatbutton->show();
    QToolTip::add(chatbutton, "User Info");
    chatButtons[chat] = chatbutton;
    kdebugf2();
}

void ExtInfo::handleDestroyingChat(Chat* chat)
{
    kdebugf();

    if (!chatButtons.contains(chat))
            return;

    delete chatButtons[chat];
    chatButtons.remove(chat);
    kdebugf2();
}

bool ExtInfo::UpdateUser()
{
    kdebugf();
    UserBox *activeUserBox = kadu->userbox()->activeUserBox();
    if (activeUserBox == NULL)//to siê zdarza...
    {
        kdebugf2();
        return false;
    }
    UserListElements users = activeUserBox->selectedUsers();
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

void ExtInfo::showChatUserInfo()
{
    kdebugf();
    Chat *chat = getCurrentChat();
    UserListElements s = chat->users()->toUserListElements();
    (new UserInfo(s[0], 0, "user info"))->show();
    kdebugf2();
}

void ExtInfo::showChatExtInfo()
{
    kdebugf();
    Chat *chat = getCurrentChat();
    UserListElements s = chat->users()->toUserListElements();
    showExtInfo(s[0].altNick());
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

void ExtInfo::userDataChanged(UserListElement elem, QString name, QVariant oldValue,QVariant currentValue, bool massively, bool last)
{
    kdebugf();
    if (name != QString("AltNick"))
        return;                 //To powodowa³o b³±d przy usuwaniu i dodawaniu kontaktów
    extlist.renameItem(oldValue.toString(), currentValue.toString());
    if (frmextinfo)
    {
        frmextinfo->renameSection(oldValue.toString(), currentValue.toString());
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
        {
            UserListElement buff;
            hint_manager->message("ExtInfo",
                formatNameDayInfo(i.key(),(*i).daysToNameDay()),
                NULL,
                userlist->containsAltNick(i.key()) ? (buff = userlist->byAltNick(i.key()),&buff) : NULL);
        }
    }
    if (config_file.readBoolEntry("ExtInfo","birthday",true))
    {
        ExtList birthday = extlist.getCommingBirthday(config_file.readNumEntry("ExtInfo","remind",1));
        for (ExtList::iterator i = birthday.begin(); i != birthday.end(); i++)
        {
            UserListElement buff;
            hint_manager->message("ExtInfo",
                formatBirthdayInfo(i.key(),(*i).daysToBirthday()),
                NULL,
                userlist->containsAltNick(i.key()) ? (buff = userlist->byAltNick(i.key()),&buff) : NULL);
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
    if (config_file.readBoolEntry("ExtInfo","button",true))
    {
        if (chatmenu == NULL)
	    CreateChatButton();
    }
    else
        DestroyChatButton();
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

Chat* ExtInfo::getCurrentChat()
{
    kdebugf();

    // Getting all chat windows
    ChatList cs = chat_manager->chats();

    // Now for each chat window we check,
    // if it's an active one.
    uint i;
    for ( i = 0; i < cs.count(); i++ )
    {
        if (cs[i]->isActiveWindow())
        {
            break;
        }
    }
    if (i == cs.count())
        return 0;

    return cs[i];
}
