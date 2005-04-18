#ifndef EXTINFO_H
#define EXTINFO_H

#include <qobject.h>
#include "frmextinfo.h"
#include "kadu.h"

class ExtInfo : public QObject
{
    Q_OBJECT

    public:
        ExtInfo();
        ~ExtInfo();
    private:
        frmExtInfo frmextinfo;
        ExtList extlist;
        QTimer timer;
        bool menuBirthday;
        bool menuNameDay;
        UserListElement user;

        QString formatBirthdayInfo(const QString& name, int days);
        QString formatNameDayInfo(const QString& name, int days);

    private slots:
        void acceptChanges( const ExtList&);
        void checkAnniversary();
    public slots:
        void showExtInfo();
        void onPopupMenuCreate();
        void userDataChanged(const UserListElement* const oldData, const UserListElement* const newData,bool);
        void knowNameDay();
        void knowBirthday();
        void restartTimer();
        void onApplyConfigDialog();
        void onExport();
        void onImport();
};

#endif
