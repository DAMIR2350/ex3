#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <list>
#include <QtGui>
#include <QStringList>
#include <QRegularExpression>
#include <QStack>
#include <QMessageBox>
QString str;
QList <double> list;
QString n = "";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->Button0,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->Button1,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->Button2,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->Button3,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->Button4,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->Button5,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->Button6,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->Button7,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->Button8,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->Button9,SIGNAL(clicked()),this,SLOT(numbers()));
    connect(ui->ButtonPlus,SIGNAL(clicked()),this,SLOT(actions()));
    connect(ui->ButtonMinus,SIGNAL(clicked()),this,SLOT(actions()));
    connect(ui->ButtonSlash,SIGNAL(clicked()),this,SLOT(actions()));
    connect(ui->ButtonAsterisk,SIGNAL(clicked()),this,SLOT(actions()));
    connect(ui->ButtonDot,SIGNAL(clicked()),this,SLOT(actions()));
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    if (event->key()==Qt::Key_0){
        ui->Button0->click();
    }
    if (event->key()==Qt::Key_1){
        ui->Button1->click();
    }
    if (event->key()==Qt::Key_2){
        ui->Button2->click();
    }
    if (event->key()==Qt::Key_3){
        ui->Button3->click();
    }
    if (event->key()==Qt::Key_4){
        ui->Button4->click();
    }
    if (event->key()==Qt::Key_5){
        ui->Button5->click();
    }
    if (event->key()==Qt::Key_6){
        ui->Button6->click();
    }
    if (event->key()==Qt::Key_7){
        ui->Button7->click();
    }
    if (event->key()==Qt::Key_8){
        ui->Button8->click();
    }
    if (event->key()==Qt::Key_9){
        ui->Button9->click();
    }
    if (event->key()==Qt::Key_Backspace){
        ui->ButtonDel->click();
    }
    if (event->key()==Qt::Key_Minus){
        ui->ButtonMinus->click();
    }

    if (event->key()==Qt::Key_Plus){
        ui->ButtonPlus->click();
    }

    if (event->key()==Qt::Key_Asterisk){
        ui->ButtonAsterisk->click();
    }

    if (event->key()==Qt::Key_Slash){
        ui->ButtonSlash->click();
    }
    if (event->key()==Qt::Key_Enter){
        ui->ButtonEquals->click();
    }
    if (event->key()==Qt::Key_Equal){
        ui->ButtonEquals->click();
    }


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::numbers()
{
    QPushButton *button = (QPushButton *)sender();
    if (str == "inf"){
        str = "";
        ui->resultLine->setText(str);
    }
    if (str.isEmpty()){
        str = str + button->text();
        ui->resultLine->setText(str);
    }else{
        if (QString(str[str.size()-1])!="%"){
            str =str + button->text();
            ui->resultLine->setText(str);
        }
    }


}
void MainWindow::actions()
{
    QPushButton *button = (QPushButton *)sender();
    if (!(str.isEmpty())){
        if (str[str.size()-1].isDigit()||QString(str[str.size()-1])=="%"){
            str += button->text();
        }else if (str.last(1)=="+"||str.last(1)=="-"||str.last(1)=="/"||str.last(1)=="*"||str.last(1)=="."){
            str.remove(-1,1);
            str += button->text();
        }
        ui->resultLine->setText(str);
    }
}



void MainWindow::on_ButtonDel_clicked()
{
     ui->resultLine->setText(str.remove(-1,1));
}

void MainWindow::on_ButtonCE_clicked()
{
    str.clear();
    ui->resultLine->setText(str);
}


void MainWindow::on_ButtonSign_clicked()
{
    QString expression = str;
    QRegularExpression re(R"(([-+]?)(\s*[0-9]*\.?[0-9]+)(?!.*\d))");
    QRegularExpressionMatch match = re.match(expression);

    if (match.hasMatch()) {
        QString sign = match.captured(1);
        QString lastNumber = match.captured(2).trimmed();
        if (!expression.endsWith("*") && !expression.endsWith("/")) {
            if (sign == "-") {
                expression.replace(match.capturedStart(1), match.capturedLength(1) + match.capturedLength(2), "+" + lastNumber);
            } else {
                expression.replace(match.capturedStart(1), match.capturedLength(1) + match.capturedLength(2), "-" + lastNumber );
            }
        }
    }
    expression.replace("*+","*");
    expression.replace("/+","*");
    str = expression;
    if (str.startsWith("+")){
        str.remove(0,1);
    }


    ui->resultLine->setText(str);

}


void MainWindow::on_ButtonProcent_clicked()
{
    if (!(str.isEmpty())){

        if (str[str.size()-1].isDigit()){
            str += "%";
            ui->resultLine->setText(str);
        }
    }

}


void MainWindow::on_ButtonEquals_clicked() {
    if (str!="inf"){
        str.replace("%","*0.01");
        if (!(str[str.size()-1].isDigit())){
            str.remove(-1,1);
        }
        QString expr = str;
        expr.replace(QRegularExpression("(\\d)\\s*\\*-"), "\\1*-1*");
        double result = eval(expr);
        ui->resultLine->setText(QString::number(result));
        str = QString::number(result);
    }
}

double MainWindow::eval(const QString &expr) {
    QStringList tokens = expr.split(QRegularExpression("([\\+\\-\\*/])"), Qt::SkipEmptyParts);
    QStack<double> values;
    QStack<QChar> ops;

    auto applyOp = [](double a, double b, QChar op) {
        switch (op.toLatin1()) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        default: return 0.0;
        }
    };

    bool isLastTokenOperator = true;

    for (int i = 0; i < expr.length(); ) {
        if (expr[i].isSpace()) {
            i++;
            continue;
        }

        if (expr[i].isDigit() || (expr[i] == '-' && isLastTokenOperator)) {
            QString num;
            if (expr[i] == '-') {
                num.append(expr[i++]);
            }
            while (i < expr.length() && (expr[i].isDigit() || expr[i] == '.')) {
                num.append(expr[i++]);
            }
            values.push(num.toDouble());
            isLastTokenOperator = false; // Последний токен не оператор
        } else {
            while (!ops.isEmpty() && precedence(ops.top()) >= precedence(expr[i])) {
                double b = values.pop();
                double a = values.pop();
                values.push(applyOp(a, b, ops.pop()));
            }
            ops.push(expr[i++]);
            isLastTokenOperator = true; // Последний токен оператор
        }
    }

    // Применяем оставшиеся операторы
    while (!ops.isEmpty()) {
        double b = values.pop();
        double a = values.pop();
        values.push(applyOp(a, b, ops.pop()));
    }

    return values.pop();
}

int MainWindow::precedence(QChar op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

