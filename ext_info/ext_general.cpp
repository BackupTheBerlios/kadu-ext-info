#include "ext_general.h"

#include "misc.h"
#include "config_dialog.h"
#include "userbox.h"
#if defined(KADU_0_5_0)
#include "chat_manager.h"
#include "icons_manager.h"
#include "http_client.h"
#endif
#include "userinfo.h"

#ifdef EXTERNAL
#include <modules/hint_manager.h>
#else
#include "../hints/hint_manager.h"
#endif

bool MigrateFromOldVersion()
{
    QDir dir(ggPath("ext_info"));
    if (dir.exists())
        return false;
    dir.mkdir(ggPath("ext_info"));
    return true;
}

void OpenWebBrowser(const QString &url)
{
    openWebBrowser(url);
}

KaduExtInfo::KaduExtInfo(bool migrate)
    :ExtInfo(extinfoPath("RExInfo.dat")), chatmenu(NULL), http(NULL), menuBirthday(false),menuNameDay(false)
{
    kdebugf();
    if (migrate)
    {
        extlist.loadFromFile(ggPath("RExInfo.dat"));
        extlist.saveToFile();
        MessageBox::msg(tr("ext_info v") + QString(EXTINFO_VERSION) + tr(" was imported settings from previous version."));
    }
    RegisterInConfigDialog(migrate);
    RegisterSignals();
    onApplyConfigDialog();
    if (checkUpdateStable || checkUpdateUnstable)
        checkForNewVersion();
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkAnniversary()));
    restartTimer();
    kdebugf2();
}

KaduExtInfo::~KaduExtInfo()
{
    kdebugf();
    if (http)
        delete http;
    DestroyChatButton();
    UnregisterSignals();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(checkAnniversary()));
    closeWindow();
    UnregisterInConfigDialog();
    kdebugf2();
}

QString KaduExtInfo::moduleDataPath(const QString &filename)
{
    return dataPath(QString("kadu/modules/data/ext_info/") + filename);
}

QString KaduExtInfo::extinfoPath(const QString &filename)
{
    return ggPath(QString("ext_info/") + filename);
}

void KaduExtInfo::RegisterInConfigDialog(bool migrate)
{
    kdebugf();
    config = new ConfigFile(extinfoPath(QString("ext-info.conf")));
    ConfigDialog::addTab("ExtInfo",dataPath("kadu/modules/data/ext_info/ext_info_tab.png"));
    ConfigDialog::addVGroupBox("ExtInfo", "ExtInfo", QT_TRANSLATE_NOOP("@default", "Remind"));
    ConfigDialog::addCheckBox(config, "ExtInfo", "Remind", QT_TRANSLATE_NOOP("@default", "Enable to remind of name day"), "name_day", migrate ? config_file.readBoolEntry("ExtInfo","name_day",true) : TRUE);
    ConfigDialog::addCheckBox(config, "ExtInfo", "Remind", QT_TRANSLATE_NOOP("@default", "Enable to remind of birthday"), "birthday", migrate ? config_file.readBoolEntry("ExtInfo","birthday",true) : TRUE);
    ConfigDialog::addSpinBox(config, "ExtInfo", "Remind", QT_TRANSLATE_NOOP("@default", "Remind days before:"),"remind",0,100,1, migrate ? config_file.readNumEntry("ExtInfo","remind",3) : 1);
    ConfigDialog::addSpinBox(config, "ExtInfo", "Remind", QT_TRANSLATE_NOOP("@default", "Reminds frequency (minutes):"),"remind_frequency",0,1440,1, migrate ? config_file.readNumEntry("ExtInfo","remind_frequency",10) : 10);
    ConfigDialog::addHBox("ExtInfo","ExtInfo","ieButtons");//"Import/export buttons"
    ConfigDialog::addPushButton("ExtInfo","ieButtons",QT_TRANSLATE_NOOP("@default","Import"));
    ConfigDialog::addPushButton("ExtInfo","ieButtons",QT_TRANSLATE_NOOP("@default","Export"));
    ConfigDialog::addCheckBox(config, "ExtInfo", "ExtInfo", QT_TRANSLATE_NOOP("@default", "Show ext_info button in chat windows"), "button", migrate ? config_file.readBoolEntry("ExtInfo","button",true) : TRUE);
    ConfigDialog::addCheckBox(config, "ExtInfo", "ExtInfo", QT_TRANSLATE_NOOP("@default", "Check for new stable version"), "stable", TRUE);
    ConfigDialog::addCheckBox(config, "ExtInfo", "ExtInfo", QT_TRANSLATE_NOOP("@default", "Check for new unstable version"), "unstable", FALSE);
    if (migrate)
        config->sync();
    ConfigDialog::connectSlot("ExtInfo","Import",SIGNAL(clicked()),this,SLOT(onImport()));
    ConfigDialog::connectSlot("ExtInfo","Export",SIGNAL(clicked()),this,SLOT(onExport()));
    kdebugf2();
}

void KaduExtInfo::UnregisterInConfigDialog()
{
    kdebugf();
    ConfigDialog::disconnectSlot("ExtInfo","Import",SIGNAL(clicked()),this,SLOT(onImport()));
    ConfigDialog::disconnectSlot("ExtInfo","Export",SIGNAL(clicked()),this,SLOT(onExport()));

    ConfigDialog::removeControl("ExtInfo","Check for new unstable version");
    ConfigDialog::removeControl("ExtInfo","Check for new stable version");
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
    config->sync();
    delete config;
    kdebugf2();
}

void KaduExtInfo::onApplyConfigDialog()
{
    remindNameday = config->readBoolEntry("ExtInfo","name_day",true);
    remindBirthday = config->readBoolEntry("ExtInfo","birthday",true);
    beforeRemind = config->readNumEntry("ExtInfo","remind",3);
    remindFrequency = config->readNumEntry("ExtInfo","remind_frequency",10);
    showButton = config->readBoolEntry("ExtInfo","button",true);
    checkUpdateStable = config->readBoolEntry("ExtInfo","stable",true);
    checkUpdateUnstable = config->readBoolEntry("ExtInfo","unstable",false);

    if (showButton)
    {
        if (chatmenu == NULL)
        CreateChatButton();
    }
    else
        DestroyChatButton();
    restartTimer();
}

void KaduExtInfo::RegisterSignals()
{
    kdebugf();
    UserBox::userboxmenu->addItem(moduleDataPath("ext_info_menu.png"), tr("Display extended information"), this, SLOT(showExtInfo()));

    QObject::connect(UserBox::userboxmenu, SIGNAL(popup()), this, SLOT(onPopupMenuCreate()));
#if defined(KADU_0_4_x)
    connect(&userlist, SIGNAL(userDataChanged(const UserListElement * const, const UserListElement * const,bool)),
        this, SLOT(userDataChanged(const UserListElement * const, const UserListElement * const,bool)));
#elif defined(KADU_0_5_0)
    connect(userlist, SIGNAL(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)),
        this, SLOT(userDataChanged(UserListElement, QString, QVariant,QVariant, bool, bool)));
#endif
    ConfigDialog::registerSlotOnApply(this, SLOT(onApplyConfigDialog()));
    kdebugf2();
}

void KaduExtInfo::UnregisterSignals()
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

    int menuitem = UserBox::userboxmenu->getItem(tr("Display extended information"));
    UserBox::userboxmenu->removeItem(menuitem);
    kdebugf2();
}

void KaduExtInfo::checkForNewVersion()
{
    QTimer::singleShot(60000, this, SLOT(onStart()));
}

void KaduExtInfo::CreateChatButton()
{
    kdebugf();
    chatmenu = new QPopupMenu;

#if defined(KADU_0_4_x)
    popups[0] = chatmenu->insertItem(icons_manager.loadIcon("EditUserInfo"),tr("Display standard information"),this,SLOT(showChatUserInfo()));
    popups[1] = chatmenu->insertItem(icons_manager.loadIcon(this->moduleDataPath("ext_info_menu.png")),tr("Display extended information"),this,SLOT(showChatExtInfo()));
    connect(chat_manager, SIGNAL(chatCreated(const UinsList&)), this, SLOT(chatCreated(const UinsList&)));
    connect(chat_manager, SIGNAL(chatDestroying(const UinsList&)), this, SLOT(chatDestroying(const UinsList&)));
#elif defined(KADU_0_5_0)
    popups[0] = chatmenu->insertItem(icons_manager->loadIcon("EditUserInfo"),tr("Display standard information"),this,SLOT(showChatUserInfo()));
    popups[1] = chatmenu->insertItem(icons_manager->loadIcon(this->moduleDataPath("ext_info_menu.png")),tr("Display extended information"),this,SLOT(showChatExtInfo()));
    connect(chat_manager, SIGNAL(chatCreated(const UserGroup*)), this, SLOT(chatCreated(const UserGroup*)));
    connect(chat_manager, SIGNAL(chatDestroying(const UserGroup*)), this, SLOT(chatDestroying(const UserGroup*)));
#endif
    ChatList::ConstIterator it;
    for ( it = chat_manager->chats().begin(); it != chat_manager->chats().end(); it++ )
        handleCreatedChat(*it);
    kdebugf2();
}

void KaduExtInfo::DestroyChatButton()
{
    if (chatmenu == NULL)
        return;
    kdebugf();
#if defined(KADU_0_4_x)
    disconnect(chat_manager, SIGNAL(chatCreated(const UinsList&)), this, SLOT(chatCreated(const UinsList&)));
    disconnect(chat_manager, SIGNAL(chatDestroying(const UinsList&)), this, SLOT(chatDestroying(const UinsList&)));
#elif defined(KADU_0_5_0)
    disconnect(chat_manager, SIGNAL(chatCreated(const UserGroup*)), this, SLOT(chatCreated(const UserGroup*)));
    disconnect(chat_manager, SIGNAL(chatDestroying(const UserGroup*)), this, SLOT(chatDestroying(const UserGroup*)));
#endif
    ChatList::ConstIterator it;
    for ( it = chat_manager->chats().begin(); it != chat_manager->chats().end(); it++ )
        handleDestroyingChat(*it);
    delete chatmenu;
    chatmenu = NULL;
    kdebugf2();
}

void KaduExtInfo::handleCreatedChat(Chat* chat)
{
    kdebugf();
#if defined(KADU_0_4_x)
    if (chat->uins().count() != 1)
#elif defined(KADU_0_5_0)
    if (chat->users()->count() != 1)
#endif
        return;
    QPushButton* chatbutton = new QPushButton(chat->buttontray);
#if defined(KADU_0_4_x)
    chatbutton->setPixmap(icons_manager.loadIcon("PersonalInfo"));
#elif defined(KADU_0_5_0)
    chatbutton->setPixmap(icons_manager->loadIcon("PersonalInfo"));
#endif
    chatbutton->setPopup(chatmenu);
    chatbutton->show();
    QToolTip::add(chatbutton, "User Info");
    chatButtons[chat] = chatbutton;
    kdebugf2();
}

void KaduExtInfo::handleDestroyingChat(Chat* chat)
{
    kdebugf();
//#if defined(KADU_0_5_0)

    if (!chatButtons.contains(chat))
            return;

    delete chatButtons[chat];
    chatButtons.remove(chat);
//#endif
    kdebugf2();
}

Chat* KaduExtInfo::getCurrentChat()
{
    kdebugf();
//#if defined(KADU_0_5_0)

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
//#endif
    return NULL;
}

bool KaduExtInfo::getSelectedUser(QString &user)
{
    kdebugf();
#if defined(KADU_0_4_x)
    UserBox *activeUserBox = kadu->userbox()->getActiveUserBox();
#elif defined(KADU_0_5_0)
    UserBox *activeUserBox = kadu->userbox()->activeUserBox();
#endif
    if (activeUserBox == NULL)//to siê zdarza...
    {
        kdebugf2();
        return false;
    }
#if defined(KADU_0_4_x)
    UserList users = activeUserBox->getSelectedUsers();
#elif defined(KADU_0_5_0)
    UserListElements users = activeUserBox->selectedUsers();
#endif
    if (users.count() == 1)
    {
        user = (*users.begin()).altNick();
        kdebugf2();
        return true;
    }
    return false;
    kdebugf2();
}

void KaduExtInfo::showRemindAnniversary(const QString &str, const QString &user)
{
#if defined(KADU_0_4_x)
    hint_manager->message("ExtInfo", str, NULL, userlist.containsAltNick(user) ? &userlist.byAltNick(user) : NULL);
#elif defined(KADU_0_5_0)
    UserListElement buff;
    hint_manager->message("ExtInfo", str, NULL, userlist->containsAltNick(user) ? (buff = userlist->byAltNick(user), &buff) : NULL);
#endif
}

// Slots-----------------------------------------------------------------------------------------------------------

void KaduExtInfo::chatCreated(const UserGroup* ul)
{
    kdebugf();
#if defined(KADU_0_5_0)
    Chat* chat = chat_manager->findChat(ul);
    handleCreatedChat(chat);
#endif
    kdebugf2();
}

void KaduExtInfo::chatDestroying(const UserGroup* ul)
{
    kdebugf();
#if defined(KADU_0_5_0)
    Chat* chat = chat_manager->findChat(ul);
    handleDestroyingChat(chat);
#endif
    kdebugf2();
}

void KaduExtInfo::chatCreated(const UinsList& ul)
{
    kdebugf();
#if defined(KADU_0_4_x)
    Chat* chat = chat_manager->findChatByUins(ul);
    handleCreatedChat(chat);
#endif
    kdebugf2();
}

void KaduExtInfo::chatDestroying(const UinsList& ul)
{
    kdebugf();
#if defined(KADU_0_4_x)
    Chat* chat = chat_manager->findChatByUins(ul);
    handleDestroyingChat(chat);
#endif
    kdebugf2();
}

void KaduExtInfo::showChatUserInfo()
{
    kdebugf();
    Chat *chat = getCurrentChat();
#if defined(KADU_0_4_x)
    (new UserInfo(userlist.byUinValue(*(chat->uins().begin())).altNick()))->show();
#elif defined(KADU_0_5_0)
    UserListElements s = chat->users()->toUserListElements();
    (new UserInfo(s[0], 0, "user info"))->show();
#endif
    kdebugf2();
}

void KaduExtInfo::showChatExtInfo()
{
    kdebugf();
    Chat *chat = getCurrentChat();
#if defined(KADU_0_4_x)
    showExtInfo(userlist.byUinValue(*(chat->uins().begin())).altNick());
#elif defined(KADU_0_5_0)
    UserListElements s = chat->users()->toUserListElements();
    showExtInfo(s[0].altNick());
#endif
    kdebugf2();
}

void KaduExtInfo::userDataChanged(UserListElement elem, QString name, QVariant oldValue,QVariant currentValue, bool massively, bool last)
{
    kdebugf();
#ifdef KADU_0_5_0
    if (name != QString("AltNick"))
        return;
    changeUserName(oldValue.toString(), currentValue.toString());
#endif
    kdebugf2();
}

void KaduExtInfo::userDataChanged(const UserListElement* const oldData, const UserListElement* const newData,bool)
{
    kdebugf();
#ifdef KADU_0_4_x
    if (!oldData || !newData)
        return;
    changeUserName(oldData->altNick(),newData->altNick());
#endif
    kdebugf2();
}

void KaduExtInfo::onPopupMenuCreate()
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

    if (getSelectedUser(currentUser))
    {

        if (extlist.contains(currentUser))
        {
            int name_day = extlist[currentUser].daysToNameDay();
            int birthday = extlist[currentUser].daysToBirthday();
            if (name_day <= beforeRemind && name_day >= 0 && remindNameday)
            {
                UserBox::userboxmenu->addItem("knowNameDay", tr("I know about name day :)"), this, SLOT(knowNameDay()));
                menuNameDay = true;
            }
            if (birthday <= beforeRemind && birthday >= 0 && birthday)
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

void KaduExtInfo::knowNameDay()
{
    kdebugf();
    extlist[currentUser].setKnowNameDay();
    kdebugf2();
}

void KaduExtInfo::knowBirthday()
{
    kdebugf();
    extlist[currentUser].setKnowBirthday();
    kdebugf2();
}

void KaduExtInfo::onStart()
{
    http = new HttpClient;
    http->setHost("www.kadu.net");
    connect(http, SIGNAL(finished()), this, SLOT(onDownloaded()));
    if (checkUpdateUnstable)
        http->get("/~dzwiedziu/ext_info/latest-unstable.php?version=" VERSION "&extinfo=" EXTINFO_PACKED_VERSION);
    else if (checkUpdateStable)
        http->get("/~dzwiedziu/ext_info/latest-stable.php?version=" VERSION "&extinfo=" EXTINFO_PACKED_VERSION);
}

uint extractVersion(const char *version)
{
    int major = (version[0] - '0')*10 + version[1] - '0';
    int minor = (version[2] - '0')*10 + version[3] - '0';
    int release = (version[4] - '0')*10 + version[5] - '0';
    bool beta = version[6] == 'b';
    int beta_release = (version[7] - '0')*10 + version[8] - '0';
    return (major << 24) + (minor << 16) + ((beta ? release : release + 1) << 8) + beta_release;
}

QString formatVersion(const uchar *version)
{
    QString str = QString("%1.%2").arg(int(version[3])).arg(int(version[2]));
    if ((version[0] == 0) && (version[1] > 1))
        str += QString(".%1").arg(int(version[1]) - 1);
    else if (version[0])
        str += QString(" beta %1").arg(int(version[0]));
    return str;
}

void KaduExtInfo::onDownloaded()
{
    union {
        uint packed;
        uchar unpacked[4];
    };
    packed = extractVersion(http->data().data());
    if (packed > extractVersion(EXTINFO_PACKED_VERSION))
    {
        QString str = tr("New %1 version ext_info is available: %2").arg(unpacked[0] ? tr("experimental") : tr("stable")).arg(formatVersion(unpacked));
        MessageBox::msg(str);
    }
    disconnect(http, SIGNAL(finished()), this, SLOT(onDownloaded()));
}

// GetDataFromKadu +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

GetDataFromKadu::GetDataFromKadu(const QString& altNick)
{
    if (altNick.isEmpty())
        found = false;
    else
        load(altNick);
}

bool GetDataFromKadu::load(const QString& altNick)
{
    kdebugf();
#if defined(KADU_0_4_x)
    if (!(found = userlist.containsAltNick(altNick)))
#elif defined(KADU_0_5_0)
    if (!(found = userlist->containsAltNick(altNick)))
#endif
    {
        kdebugf2();
        return false;
    }
#if defined(KADU_0_4_x)
    UserListElement user = userlist.byAltNick(altNick);
    UIN = user.uin();
#elif defined(KADU_0_5_0)
    UserListElement user = userlist->byAltNick(altNick);
    UIN = user.ID("Gadu").toUInt();
#endif
    nickName = user.nickName();
    firstName = user.firstName();
    lastName = user.lastName();
    mobile = user.mobile();
    email = user.email();
    return true;
    kdebugf2();
}
