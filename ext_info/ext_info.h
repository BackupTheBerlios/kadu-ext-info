#ifndef EXTINFO_H
#define EXTINFO_H

#include <qobject.h>
#include <qtimer.h>
#include "frmextinfo.h"
#include "kadu.h"
#include "chat.h"

class QHttp;

class ExtInfo : public QObject
{
    Q_OBJECT

public:
    virtual ~ExtInfo();
                    // pobierz ¶cie¿kê do pliku znajduj±cego siê w katalogu:
    virtual QString moduleDataPath(const QString &filename) = 0;// danych modu³u
    virtual QString extinfoPath(const QString &filename) = 0;   // ustawieñ modu³u w profilu u¿ytkownika

protected:

    //Ustawienia (z okna konfiguracji)
    bool remindNameday;         // przypominaj o imieninach
    bool remindBirthday;        //     ,,      o urodzinach
    int beforeRemind;           // ilo¶æ dni przed
    int remindFrequency;        // co ile minut wy¶wietla dymek z przypomnieniem
    bool showButton;            // poka¿ przycisk ext_info w oknie rozmowy
    bool checkUpdateStable;     // sprawdzaj czy jest nowa wersja stablina ext_info
    bool checkUpdateUnstable;   //      ,,           ,,           rozwojowa    ,,


    frmExtInfo *frmextinfo;
    QHttp *http;
    ExtList extlist;
    QTimer timer;
    QString currentUser;

    QString formatBirthdayInfo(const QString& name, int days);
    QString formatNameDayInfo(const QString& name, int days);

    ExtInfo(const QString &datafile);

    virtual bool getSelectedUser(QString &user) = 0;
    virtual void showRemindAnniversary(const QString &str, const QString &user) = 0;
    void restartTimer();

protected slots:
    void acceptChanges( const ExtList&);
    void closeWindow();
    void checkAnniversary();

public slots:
    void changeCurrentUser(const QString &newUser); // chyba bedzie niepotrzebne
    void changeUserName(const QString& oldName, const QString& newName);    // Gdy zmieni³ siê altNick kogo¶

    void showExtInfo();
    void showExtInfo(const QString& section);
    void onExport();
    void onImport();
};

#endif
