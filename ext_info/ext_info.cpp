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
    http://www.kadu.net/~dzwiedziu/ext_info.php

    ChangeLog:

        [v2.0 ]
            + Dodana zak�adka z wypisanymi wszystkimi danymi o kontakcie, wraz
              z wy�wietleniem zdj�cia.
            + Dodany modu� tworzenia avatar�w (zak�adka "Zdj�cie"->przycisk
              "Avatar"). Avatary zapisywane s� w katalogu $HOME/.kadu/ext_info
            * Du�e zmiany w kodzie, reorganizacja ca�ej klasy g��wnej
            + Powr�t do zgodno�ci z Kadu 0.4.x
            + Powiadamianie o nowych wersjach
            * Przeniesienie ustawie� modu�u do katalogu $HOME/.kadu/ext_info

        [v1.5.1 ]
            * Podczas gdy w oknie konfiguracji zosta�y zaakceptowane zmiany,
              pojawia�o si� kilka ikon ext_info w oknie rozmowy. B��d ten zosta�
              w�a�nie naprawiony.

        [v1.5 ]
            + Dodana ikona ext_info w oknie rozmowy
            - Zerwanie kompatybilno�ci z Kadu 0.4.x

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

#include "ext_general.h"
#include <qhttp.h>

KaduExtInfo *extinfo;

extern "C" int ext_info_init()
{
    extinfo = new KaduExtInfo(MigrateFromOldVersion());
    return 0;
}

extern "C" void ext_info_close()
{
    delete extinfo;
}

ExtInfo::ExtInfo(const QString &datafile)
    :frmextinfo(NULL), http(NULL),extlist(datafile)
{
}

ExtInfo::~ExtInfo()
{
    if (http)
        delete http;
}

void ExtInfo::showExtInfo()
{
    kdebugf();
    if (getSelectedUser(currentUser))
        showExtInfo(currentUser);
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

void ExtInfo::acceptChanges( const ExtList & data )
{
    kdebugf();
    extlist = data;
    extlist.saveToFile();
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

void ExtInfo::restartTimer()
{
    timer.start(remindFrequency * 60000);
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
        QString filename = QFileDialog::getSaveFileName(extinfoPath("RExInfo.dat"),
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
    QString filename = QFileDialog::getOpenFileName(extinfoPath("RExInfo.dat"),
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

// Slots    -----------------------------------------------------------------------------------------------

void ExtInfo::changeCurrentUser(const QString &newUser)
{
    currentUser = newUser;
}

void ExtInfo::changeUserName(const QString& oldName, const QString& newName)
{
    extlist.renameItem(oldName, newName);
    if (frmextinfo)
        frmextinfo->renameSection(oldName, newName);
}

void ExtInfo::checkAnniversary()
{
    kdebugf();
    if (remindNameday)
    {
        ExtList name_day = extlist.getCommingNameDay(beforeRemind);
        for (ExtList::iterator i = name_day.begin(); i != name_day.end(); i++)
        {
            showRemindAnniversary(formatNameDayInfo(i.key(),(*i).daysToNameDay()),i.key());
        }
    }
    if (remindBirthday)
    {
        ExtList birthday = extlist.getCommingBirthday(beforeRemind);
        for (ExtList::iterator i = birthday.begin(); i != birthday.end(); i++)
        {
            showRemindAnniversary(formatBirthdayInfo(i.key(),(*i).daysToBirthday()),i.key());
        }
    }
    kdebugf2();
}
