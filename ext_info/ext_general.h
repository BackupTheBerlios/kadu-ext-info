#ifndef ext_generalH
#define ext_generalH

#define EXTINFO_VERSION "2.0 beta 5"
#define EXTINFO_PACKED_VERSION "020000b05"
#define EXTINFO_INFOTEMPLATE "info_template_en.html"
#define MODULE_EXTINFO_VERSION 2.0

#include "config.h"

#if defined(KADU_VERSION_0_4_0_cvs) || defined(KADU_VERSION_0_4_0) || defined(KADU_VERSION_0_4_1)
#define KADU_0_4_x
#elif defined(KADU_VERSION_0_5_0_svn) || defined(KADU_VERSION_0_5_0)
#define KADU_0_5_0
#endif

#include <qobject.h>
#include <qstring.h>
#include "message_box.h"
#include "debug.h"
#include "ext_info.h"

class Chat;
class QPushButton;
class UserGroup;
class ConfigFile;
class UinsList;
class HttpClient;
class ToolButton;
class UserListElements;

class GetDataFromKadu
{
public:
    uint UIN;
    QString nickName;
    QString firstName;
    QString lastName;
    QString mobile;
    QString email;
    bool found;

    GetDataFromKadu(const QString& altNick = "");
    bool load(const QString& altNick);
};

class KaduExtInfo : public ExtInfo
{
    Q_OBJECT

private:
    ConfigFile* config;
    // Chat Button
    QPopupMenu *chatmenu;   // Menu pojawiajace sie po klikniecu w przycisk ext_info w oknie rozmowy
    int popups[2];
    // Check for new version
    HttpClient *http;

    // Popup menu
    bool menuBirthday;
    bool menuNameDay;

    // Podpinanie sygna³ów od Kadu oraz okno konfiguracji Kadu
    void RegisterInConfigDialog(bool migrate);
    void UnregisterInConfigDialog();
    void RegisterSignals();
    void UnregisterSignals();

    // Chat button w 0.4.x
    QMap<Chat*,QPushButton*> chatButtons;
    void CreateChatButton();
    void DestroyChatButton();
    void handleCreatedChat(Chat* chat);
    void handleDestroyingChat(Chat* chat);
    Chat* getCurrentChat();

    // Chat button w 0.5.0
    void Create5ChatButton();
    void Destroy5ChatButton();
    void setShowChatButton(bool v);

    //
    void checkForNewVersion();

    bool getSelectedUser(QString &user);
    void showRemindAnniversary(const QString &str, const QString &user);

public:
    KaduExtInfo(bool migrate = false);  // migrate = true: migracja ze starej wersji
    ~KaduExtInfo();
    virtual QString moduleDataPath(const QString &filename = "");
    virtual QString extinfoPath(const QString &filename = "");

public slots:
    void openMailComposer(const QString &link);
    void openChat(const QString &link);
    void openSMS(const QString &link);

private slots:
    void onApplyConfigDialog();
    void userDataChanged(const UserListElement* const oldData, const UserListElement* const newData,bool);  // Kadu 0.4.x
    void userDataChanged(UserListElement elem, QString name, QVariant oldValue,QVariant currentValue, bool massively, bool last); // Kadu 0.5.0

    void onPopupMenuCreate();
    void knowNameDay();
    void knowBirthday();

    // Chat button kod zapo¿yczony z modu³u XMMS
    //void chatCreated(const UserGroup*);     // Kadu 0.5.0
    //void chatDestroying(const UserGroup*);
    void chatCreated(const UinsList&);      // Kadu 0.4.x
    void chatDestroying(const UinsList&);
    // 0.5.0
    void onButtonAction(const UserGroup* users, bool is_on);
    void onAddedButton(ToolButton *button, ToolBar *toolbar, const UserListElements& users);

    void showChatUserInfo();
    void showChatExtInfo();

    // Check for new version
    void onStart();
    void onDownloaded();
};

extern KaduExtInfo *extinfo;
bool MigrateFromOldVersion();
void OpenWebBrowser(const QString &url);

#endif //ext_generalH
