#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void numbers();
    void actions();
    void keyPressEvent(QKeyEvent *event);


    void on_ButtonDel_clicked();

    void on_ButtonCE_clicked();

    void on_ButtonSign_clicked();

    void on_ButtonProcent_clicked();

    void on_ButtonEquals_clicked();

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();



private slots:
    void calculate();

private:
    Ui::MainWindow *ui;
    double eval(const QString &expr);
    int precedence(QChar op);



};
#endif // MAINWINDOW_H
