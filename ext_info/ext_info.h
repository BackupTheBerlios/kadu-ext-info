#ifndef EXTINFO_H
#define EXTINFO_H

#include <qobject.h>
#include <qtimer.h>
#include "frmextinfo.h"
#include "kadu.h"
#include "chat.h"

class ExtInfo : public QObject
{
    Q_OBJECT

    public:
        ExtInfo();
        ~ExtInfo();
    private:
        frmExtInfo *frmextinfo;
        ExtList extlist;
        QTimer timer;
        bool menuBirthday;
        bool menuNameDay;
        UserListElement user;   // User, ktorego popup menu sie wyswietli³o

        QPopupMenu *chatmenu;   // Menu pojawiajace sie po klikniecu w przycisk ext_info w oknie rozmowy
        int popups[2];
        QMap<Chat*,QPushButton*> chatButtons;

        QString formatBirthdayInfo(const QString& name, int days);
        QString formatNameDayInfo(const QString& name, int days);

        bool UpdateUser();

        void RegisterInConfigDialog();
        void UnregisterInConfigDialog();

        void RegisterSignals();
        void UnregisterSignals();

        void CreateChatButton();
        void DestroyChatButton();

        void handleCreatedChat(Chat* chat);
        void handleDestroyingChat(Chat* chat);

        Chat* getCurrentChat();

    private slots:
        void acceptChanges( const ExtList&);
        void closeWindow();
        void checkAnniversary();
        void chatCreated(const UserGroup*);
        void chatDestroying(const UserGroup*);
    public slots:
        void showExtInfo();
        void showExtInfo(const QString& section);
        void showChatUserInfo();
        void showChatExtInfo();
        void onPopupMenuCreate();
        void userDataChanged(UserListElement elem, QString name, QVariant oldValue,QVariant currentValue, bool massively, bool last);
        void knowNameDay();
        void knowBirthday();
        void restartTimer();
        void onApplyConfigDialog();
        void onExport();
        void onImport();
};

#endif
