#ifndef EXTINFOITEM_H
#define EXTINFOITEM_H

#define EXTINFO_VERSION "1.5.0"

#include <qobject.h>
#include <qstring.h>
/**
@author Micha³ "D¼wiedziu" Nied¼wiecki
*/

class ExtListElement
{
    private:
        bool know_birthday;
        bool know_name_day;
        static int daysToAnniversary(const QString& date);
    public:
        bool modified;

        QString nickname;
        QString last_name;
        QString first_name;
        QString birthday;
        QString name_day;
        QString phone;
        QString mobile;
        QString city;
        QString street;
        QString interests;
        QString email[2];
        QString www;
        QString irc;
        QString wp;
        QString icq;
        QString tlen;
        QString alt_gg;
        QString memo;
        QString photo_path;
        bool photo_scaled;

        ExtListElement();
        ExtListElement(const ExtListElement& src);

        int daysToBirthday() const;
        int daysToNameDay() const;
        void setKnowBirthday() {know_birthday = true;}
        void setKnowNameDay() {know_name_day = true;}
        ExtListElement& operator=(const ExtListElement& src);
        bool operator==(const ExtListElement& src) const;
        bool operator!=(const ExtListElement& src) const {return (!(*this == src));}
};

class ExtList : public QMap<QString,ExtListElement>
{
    public:
        ExtList(const QString& file = "");
        ExtList(const ExtList &src);

        bool saveToFile(const QString fileName = "");
        bool loadFromFile(const QString fileName = "");
        void setFileName(const QString fileName) {FileName = fileName;}

        void renameItem(const QString& oldName, const QString& newName);

        ExtList getCommingBirthday(int maxdays);
        ExtList getCommingNameDay(int maxdays);

        ExtList& operator=(const ExtList& src);
    private:
        QString FileName;
};

#endif
