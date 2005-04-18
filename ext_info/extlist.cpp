#include "extlist.h"
#include <qfile.h>
#include <qtextcodec.h>
#include <qdatetime.h>
#include <stdio.h>
#include "debug.h"

ExtListElement::ExtListElement()
    :know_birthday(false), know_name_day(false), photo_scaled(false)
{
}

ExtListElement::ExtListElement(const ExtListElement& src)
    :know_birthday(src.know_birthday), know_name_day(src.know_name_day),
    nickname(src.nickname),
    last_name(src.last_name),
    first_name(src.first_name),
    birthday(src.birthday),
    name_day(src.name_day),
    phone(src.phone),
    mobile(src.mobile),
    city(src.city),
    street(src.street),
    interests(src.interests),
    www(src.www),
    irc(src.irc),
    wp(src.wp),
    icq(src.icq),
    tlen(src.tlen),
    alt_gg(src.alt_gg),
    memo(src.memo),
    photo_path(src.photo_path),
    photo_scaled(src.photo_scaled)
{
    kdebugf();
    email[0] = src.email[0];
    email[1] = src.email[0];
    kdebugf2();
}

int ExtListElement::daysToAnniversary(const QString& date)
{
    kdebugf();
    int month, day;
    sscanf(date,"%2d.%2d",&day,&month);
    QDate current = QDate::currentDate();
    QDate anniversary(current.year(),month,day);
    if (current > anniversary)
        anniversary.setYMD(current.year() + 1,month,day);
    kdebugf2();
    return current.daysTo(anniversary);
}

int ExtListElement::daysToBirthday()
{
    if (know_birthday || birthday.length() == 0)
        return -1;
    return daysToAnniversary(birthday);
}

int ExtListElement::daysToNameDay()
{
    if (know_name_day || name_day.length() == 0)
        return -1;
    return daysToAnniversary(name_day);
}

ExtList::ExtList(const QString& file)
:QMap<QString,ExtListElement>(),FileName(file)
{
    if (file.length())
        loadFromFile();
}

ExtList::ExtList(const ExtList &src)
    :QMap<QString,ExtListElement>(src),FileName(src.FileName)
{
    //(*this) = src;
}

ExtList& ExtList::operator=(const ExtList& src)
{
    QMap<QString,ExtListElement>::operator=(src);
    FileName = src.FileName;
    return *this;
}

// Idziemy na pê³n± kompatybilno¶æ z RExInfo
bool ExtList::saveToFile(const QString fileName)
{
    kdebugf();
    QString filename = fileName.length() ? fileName : FileName;
    if (!FileName.length())
        return false;
    QFile file(filename);
    QString line;

    bool fileOpened=file.open(IO_WriteOnly | IO_Truncate);
    if (fileOpened) {
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("CP 1250"));
        for(const_iterator i = begin(); i != end(); ++i)
        {
            stream << '[' << i.key() << "]\r\n";
            stream << "Pseudo" << '=' << (*i).nickname << "\r\n";
            stream << "Nazwisko" << '=' << (*i).last_name << "\r\n";
            stream << "Imie" << '=' << (*i).first_name << "\r\n";
            stream << "DataUr" << '=' << (*i).birthday << "\r\n";
            stream << "DataImien" << '=' << (*i).name_day << "\r\n";
            stream << "TelDom" << '=' << (*i).phone << "\r\n";
            stream << "TelKom" << '=' << (*i).mobile << "\r\n";
            stream << "AdrMiasto" << '=' << (*i).city << "\r\n";
            stream << "AdrUl" << '=' << (*i).street << "\r\n";
            stream << "EdtZainter" << '=' << (*i).interests << "\r\n";
            stream << "Email1" << '=' << (*i).email[0] << "\r\n";
            stream << "Email2" << '=' << (*i).email[1] << "\r\n";
            stream << "WWW" << '=' << (*i).www << "\r\n";
            stream << "IrcNick" << '=' << (*i).irc << "\r\n";
            stream << "WP" << '=' << (*i).wp << "\r\n";
            stream << "ICQ" << '=' << (*i).icq << "\r\n";
            stream << "TlenN" << '=' << (*i).tlen << "\r\n";
            stream << "DrugGG" << '=' << (*i).alt_gg << "\r\n";
            stream << "MemoEx" << '=' << QString((*i).memo).replace("\n", "~~") << "\r\n";
            stream << "Zdjecie" << '=' << (*i).photo_path << "\r\n";
            stream << "ImgRozciag" << '=' << ((*i).photo_scaled ? "1" : "0") << "\r\n";
        }
        file.close();
        //FileName = filename;
    }
    kdebugf2();
    return fileOpened;
}

bool ExtList::loadFromFile(const QString fileName)
{
    kdebugf();
    QString filename = fileName.length() ? fileName : FileName;
    if (!FileName.length())
    {
        kdebugf2();
        return false;
    }
    QFile file(filename);
    QString line;
    QString activeGroupName;

    bool fileOpened=file.open(IO_ReadOnly);
    if (fileOpened) {
        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("CP 1250"));
        while (!stream.atEnd())
        {
            line = stream.readLine();
            line.stripWhiteSpace();
            if (line.startsWith("[") && line.endsWith("]"))
            {
                QString name = line.mid(1, line.length() - 2);//.stripWhiteSpace();
                if (name.lower() == "rexinfo_opcje_wtyczki")
                    activeGroupName = "";
                else
                    activeGroupName = name;
            }
            else if (activeGroupName.length())
            {
                QString name = line.section('=', 0, 0);
                QString value = line.right(line.length()-name.length()-1);
                name = name.stripWhiteSpace();
                if (line.contains('=') >= 1 && name.length() && value.length() && activeGroupName.length())
                {
                    if (name == "Pseudo") (*this)[activeGroupName].nickname = value;
                    if (name == "Nazwisko") (*this)[activeGroupName].last_name = value;
                    if (name == "Imie") (*this)[activeGroupName].first_name = value;
                    if (name == "DataUr") (*this)[activeGroupName].birthday = value;
                    if (name == "DataImien") (*this)[activeGroupName].name_day = value;
                    if (name == "TelDom") (*this)[activeGroupName].phone = value;
                    if (name == "TelKom") (*this)[activeGroupName].mobile = value;
                    if (name == "AdrMiasto") (*this)[activeGroupName].city = value;
                    if (name == "AdrUl") (*this)[activeGroupName].street = value;
                    if (name == "EdtZainter") (*this)[activeGroupName].interests = value;
                    if (name == "Email1") (*this)[activeGroupName].email[0] = value;
                    if (name == "Email2") (*this)[activeGroupName].email[1] = value;
                    if (name == "WWW") (*this)[activeGroupName].www = value;
                    if (name == "IrcNick") (*this)[activeGroupName].irc = value;
                    if (name == "WP") (*this)[activeGroupName].wp = value;
                    if (name == "ICQ") (*this)[activeGroupName].icq = value;
                    if (name == "TlenN") (*this)[activeGroupName].tlen = value;
                    if (name == "DrugGG") (*this)[activeGroupName].alt_gg = value;
                    if (name == "MemoEx") (*this)[activeGroupName].memo = value.replace("~~", "\n");
                    if (name == "Zdjecie") (*this)[activeGroupName].photo_path = value;
                    if (name == "ImgRozciag") (*this)[activeGroupName].photo_scaled = (value == "0" ? false : true);
                }
            }
        }
        file.close();
        //FileName = filename;
    }
    kdebugf2();
    return fileOpened;
}

void ExtList::renameItem(const QString& oldName, const QString& newName)
{
    (*this)[newName] = (*this)[oldName];
    remove(oldName);
}

ExtList ExtList::getCommingBirthday(int maxdays)
{
    kdebugf();
    ExtList extlist;
    for (iterator i = begin(); i != end(); i++)
    {
        int daysToBirthday = (*i).daysToBirthday();
        if ((maxdays >= daysToBirthday) && (daysToBirthday >= 0))
            extlist.insert(i.key(),(*i));
    }
    kdebugf2();
    return extlist;
}

ExtList ExtList::getCommingNameDay(int maxdays)
{
    kdebugf();
    ExtList extlist;
    for (iterator i = begin(); i != end(); i++)
    {
        int daysToNameDay = (*i).daysToNameDay();
        if ((maxdays >= daysToNameDay) && (daysToNameDay >= 0))
            extlist.insert(i.key(),(*i));
    }
    kdebugf2();
    return extlist;
}

