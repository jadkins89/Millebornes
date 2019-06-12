#ifndef NAMEDIALOG_H
#define NAMEDIALOG_H

#include <QDialog>

namespace Ui {
class NameDialog;
}

class NameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NameDialog(QWidget *parent = nullptr, int num = 0);
    ~NameDialog();

    QString get_name() { return name_; }
    bool get_ai() { return ai_; }

private slots:
    void on_buttonBox_accepted();
    void on_checkBox_toggled(bool checked);

private:
    Ui::NameDialog *ui;

    QString name_;
    int player_number_;
    bool ai_;
};

#endif // NAMEDIALOG_H
