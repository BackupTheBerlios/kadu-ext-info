#ifndef FRMSIZE_H
#define FRMSIZE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QButtonGroup;
class QRadioButton;
class QSpinBox;
class QLabel;
class QPushButton;

class frmSize : public QDialog
{
    Q_OBJECT

public:
    frmSize(int w, int h, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~frmSize();

    QButtonGroup* groupSize;
    QRadioButton* rbgG;
    QRadioButton* rbSkype;
    QRadioButton* rbEpuls;
    QRadioButton* rbUser;
    QSpinBox* width;
    QLabel* separator;
    QSpinBox* height;
    QPushButton* pbOk;
    QPushButton* pbCancel;

    int getWidth();
    int getHeight();

protected:
    QVBoxLayout* frmSizeLayout;
    QVBoxLayout* groupSizeLayout;
    QHBoxLayout* layoutUser;
    QSpacerItem* spacerSize;
    QHBoxLayout* layoutButtons;
    QSpacerItem* spacerButtons;

protected slots:
    virtual void languageChange();

private:
    void getSize(int &width, int &height);

private slots:
    void onButtonGroupClicked(int id);
};

#endif // FRMSIZE_H
